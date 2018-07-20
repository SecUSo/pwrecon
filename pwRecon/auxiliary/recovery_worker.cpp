/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#include "recovery_worker.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QSet>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTime>

RecoveryWorker::RecoveryWorker(const bool& fallback,
                               const QString& newbinarydir,
                               const QString& newbinaryfile,
                               const QString& newdictfile,
                               const QString& newpotfile,
                               const QString& newtempfilepath,
                               QObject *parent
                               ) : QObject(parent)
{
    hc2_fallback = fallback;

    binarydir = newbinarydir;
    binaryfile = newbinaryfile;
    dictfile = newdictfile;
    potfile = newpotfile;
    tempfilepath = newtempfilepath;
}

// TODO: Make messages translatable
void RecoveryWorker::onRecoveryStarted(const bool& showplain, const bool& expertMode, const QString& newhashfilepath, const QString& newhashtype)
{
    this->expertMode = expertMode;
    qDebug() << "RecoveryWorker: \n\t onRecoverStarted: \n\t\t show_plain_pwds: " << showplain << endl;
    show_plain_pwds = showplain;
    hashfilepath = newhashfilepath;
    hashtype = newhashtype;

    resetData();

    if (!importHashfile()) {
        emit txtBrowserAppend(trUtf8("- FEHLER: Ungültige Hash-Datei ausgewählt.\n"));
        emit finishRecovery();
        return;
    }

    setupBinaryArguments();

    // Setup QProcess for binary
    binaryProcess = new QProcess(this);
    connect(binaryProcess, &QProcess::started, this, &RecoveryWorker::onProcessStarted);
    connect(binaryProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &RecoveryWorker::onProcessFinished);
    binaryProcess->setProgram(binaryfile);
    binaryProcess->setWorkingDirectory(binarydir);

    emit txtBrowserAppend(trUtf8(" - WIEDERHERSTELLUNG GESTARTET -\n\nBitte warten...\n\n"));

    // Setup timer clock
    timerClock = new QTimer(this);
    connect(timerClock, &QTimer::timeout, this, &RecoveryWorker::onTickTimer);
    timerClock->start(1000);

    startNextRound();
}

void RecoveryWorker::onProcessStopped()
{
    finish = true;
    binaryProcess->kill();
}

void RecoveryWorker::resetData()
{
    finish = false;
    usernames_table.clear();
    pwd_amount = 0;
    minutes = 0;
    seconds = 0;
    interval_count = 0;
}

/**
 * Copies given hashfile to a temporary file, with hashes only (without usernames), to use with Hashcat.
 * Hashes and corresponding usernames will be stored in a table.
 *
 * @return Returns true if successful.
 */
bool RecoveryWorker::importHashfile()
{
    QStringList tempfilestring;

    QFile hashfile(hashfilepath);
    if (!hashfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    bool breakup = false;

    QTextStream in(&hashfile);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QString username;
        QString hash;
        QStringList splitted;

        if (!line.isEmpty()) {
            splitted = line.split(":");
        }

        if (splitted.size() == 1) {
            hash = splitted[0];
        }

        if (splitted.size() == 2) {
            username = splitted[0];
            hash = splitted[1];
        }

        if(splitted.size() < 1 || splitted.size() > 2 || !isHex(hash) || hash.isEmpty()) {
            breakup = true;
            break;
        }

        tempfilestring.append(QString(username + ":" + hash.toLower())); // Hashcat works with lower case
        pwd_amount++;
    }
    hashfile.close();

    if (breakup || tempfilestring.isEmpty())
        return false;

    QFile tempfile(tempfilepath);
    if (tempfile.exists())
        tempfile.remove();
    if (tempfile.open(QIODevice::ReadWrite)) {
        QTextStream stream(&tempfile);
        foreach (const QString &line, tempfilestring) {

            QStringList splitted = line.split(":");
            QString username(splitted[0]);
            QString hash(splitted[1]);

            if(!usernames_table.contains(hash))
                stream << hash << endl;

            if (username.isEmpty()) {
                username = trUtf8("(leer)");
            }

            if (!usernames_table.contains(hash, username))
                usernames_table.insert(hash, username);
        }
    }
    tempfile.close();

    return true;
}

void RecoveryWorker::setupBinaryArguments()
{
    // Dictionary attack
    args_dict_attack.clear();
    args_dict_attack << QString("--attack-mode=0") << hashtype;
    args_dict_attack << QString("--remove");
    if (!hc2_fallback) // Hashcat 3 only
        args_dict_attack << QString("--restore-disable") << QString("--logfile-disable") << QString("--gpu-temp-disable");
    args_dict_attack << tempfilepath << dictfile;

    // Mangling rules "best64"
    args_mang1_attack.clear();
    args_mang1_attack << QString("--attack-mode=0") << hashtype << QString("--rules-file=rules/best64.rule");
    args_mang1_attack << QString("--remove");
    if (!hc2_fallback) // Hashcat 3 only
        args_mang1_attack << QString("--restore-disable") << QString("--logfile-disable") << QString("--gpu-temp-disable");
    args_mang1_attack << tempfilepath << dictfile;

    // Mangling rules "generated2"
    args_mang2_attack.clear();
    args_mang2_attack << QString("--attack-mode=0") << hashtype << QString("--rules-file=rules/generated2.rule");
    args_mang2_attack << QString("--remove");
    if (!hc2_fallback) // Hashcat 3 only
        args_mang2_attack << QString("--restore-disable") << QString("--logfile-disable") << QString("--gpu-temp-disable");
    args_mang2_attack << tempfilepath << dictfile;

    // Brute-force attack
    args_brute_attack.clear();
    args_brute_attack << QString("--attack-mode=3") << QString("--increment") << hashtype;
    args_brute_attack << QString("--remove");
    if (!hc2_fallback) // Hashcat 3 only
        args_brute_attack << QString("--restore-disable") << QString("--logfile-disable") << QString("--gpu-temp-disable");
    args_brute_attack << tempfilepath << QString("?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a");
}

/*
* Hashcat will be started 4 times in a row.
*/
void RecoveryWorker::startNextRound()
{
    QThread::msleep(10);

    if (finish) {
        fileChanged();
        showResults();
        endProcess();
        emit finishRecovery();
    }
    else if (binaryProcess->arguments().isEmpty()) {
        startDictAttack();
    }
    else if (binaryProcess->arguments() == args_dict_attack) {
        startMang1Attack();
    }
    else if (binaryProcess->arguments() == args_mang1_attack) {
        startMang2Attack();
    }
    else if (binaryProcess->arguments() == args_mang2_attack) {
        fileChanged();
        showResults();
        startBruteAttack();
    }
    else if (binaryProcess->arguments() == args_brute_attack) {
        fileChanged();
        showResults();
        endProcess();
        emit finishRecovery();
    }
    else {
        endProcess();
        emit finishRecovery();
    }
}

void RecoveryWorker::startDictAttack()
{
    binaryProcess->setArguments(args_dict_attack);
    binaryProcess->start();
}

void RecoveryWorker::startMang1Attack()
{
    binaryProcess->setArguments(args_mang1_attack);
    binaryProcess->start();
}

void RecoveryWorker::startMang2Attack()
{
    binaryProcess->setArguments(args_mang2_attack);
    binaryProcess->start();
}

void RecoveryWorker::startBruteAttack()
{
    binaryProcess->setArguments(args_brute_attack);
    binaryProcess->start();
}

void RecoveryWorker::showResults()
{
    QString res_str;
    // res_str.append(" - RECOVERY STATUS -\n");
    // res_str.append(" Duration:\n ");
    res_str.append(trUtf8(" - WIEDERHERSTELLUNG STATUS -\n"));
    res_str.append(trUtf8(" Dauer:\n "));
    res_str.append(QString::number(minutes));
    res_str.append(":");
    if (seconds < 10) res_str.append("0");
    res_str.append(QString::number(seconds));
    // res_str.append(" minutes\n\n");
    res_str.append(trUtf8(" Minuten\n\n"));

    // res_str.append(" - RECOVERED PASSWORDS:\n");
    res_str.append(trUtf8(" - WIEDERHERGESTELLTE PASSWÖRTER:\n"));
    res_str.append("\n");


    int pwd_count = 0;
    int count = 0;
    foreach (const QString &value, foundPasswords) {
        QString pw_line;

        QStringList splitted = value.split(":");
        QString hash = splitted[0];
        QString plaintext = splitted[1];
        if (plaintext.isEmpty()) {
            plaintext = trUtf8("(leer)");
        }

        QList<QString> users = usernames_table.values(hash);
        // pw_line.append("Password: " + hash);
        QString passwordString = trUtf8("Passwort");
        if(expertMode){
            pw_line.append(passwordString + ": " + hash);
        }else{
            count++;
            pw_line.append(passwordString + " #" + QString::number(count)  + ": ");
        }
        if (show_plain_pwds)
            pw_line.append("  -->  [" + plaintext + "]");
        pw_line.append("\n");

        // pw_line.append("- Username(s): { ");
        pw_line.append(trUtf8("- Benutzername(n)") + ": { ");

        for (int i = 0; i < users.size(); ++i) {
            pw_line.append(users[i]);

            if (users.size() > 1 && i < (users.size() - 1)) {
                pw_line.append(", ");
            }
        }
        pw_line.append(" }");

        pwd_count += users.size();
        res_str.append(pw_line + "\n\n");
    }

    float percent;
    if (pwd_amount > 0) {
        percent = ((float) pwd_count / (float) pwd_amount) * 100;
    } else {
        percent = 0;
    }

    QString de1(trUtf8("von"));
    QString de2(trUtf8("Passwörtern wiederhergestellt"));
    res_str.append("----- " + QString::number(pwd_count) + ' ' + de1 + ' ' + QString::number(pwd_amount) + ' ' + de2 + ". (" + QString::number(percent, 'f', 1) + "%)\n\n");

    emit txtBrowserAppend(res_str);
}

void RecoveryWorker::endProcess()
{
    timerClock->stop();

    delete binaryProcess;
    delete timerClock;

    finish = false;
}

/*
* Check if all hashes are already cracked, so that Hashcat is not needed to run again.
*/
void RecoveryWorker::checkEmptyHashfile()
{
    QFile tempfile(tempfilepath);
    if (tempfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&tempfile);

        if (stream.atEnd())
            finish = true;
    }
    tempfile.close();
}

bool RecoveryWorker::isHex(QString str)
{
    QRegularExpression re("[0-9a-fA-F]+");
    QRegularExpressionValidator v(re);

    int pos = 0;

    return (v.validate(str, pos) == QValidator::Acceptable);
}

void RecoveryWorker::onProcessStarted()
{
    qDebug() << endl << "- BINARY STARTED -\n- args: " << binaryProcess->arguments() << endl;
}

void RecoveryWorker::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString line;

    QByteArray outputData = binaryProcess->readAllStandardOutput();
    QTextStream outputStream(outputData);
    QByteArray errorData = binaryProcess->readAllStandardError();
    QTextStream errorStream(errorData);

    line = outputStream.readLine();
    qDebug() << endl << "- BINARY OUTPUT:" << endl;
    while(!line.isNull()) {
        qDebug() << line;
        line = outputStream.readLine();
    }

    line = errorStream.readLine();
    qDebug() << endl << "- BINARY ERRORS:" << endl;
    while(!line.isNull()) {
        qDebug() << line;
        line = errorStream.readLine();
    }

    checkEmptyHashfile();
    emit startNextRound();
}

void RecoveryWorker::onTickTimer()
{
    seconds++;
    interval_count++;

    if (seconds >= 60) {
        minutes++;
        seconds = 0;
    }

    // Show results every 5 minutes.
//    if (interval_count >= 5) {
//        interval_count = 0;
//        showResults();
//    }

    if (interval_count >= 5)
    {
        interval_count = 0;
        qDebug() << "Checking File " << QTime::currentTime().toString() << endl;
        if(fileChanged())
        {
            showResults();
        }
    }
}

bool RecoveryWorker::fileChanged()
{

    QStringList tmpFoundPasswords;

    // Hashcat pot-file contains already cracked hashes and their plain texts
    QFile file(potfile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            tmpFoundPasswords << in.readLine();
        }
    }
    file.close();

    if(foundPasswords==tmpFoundPasswords)
    {
        return false;
    }else{
        foundPasswords = tmpFoundPasswords;
        return true;
    }
}
