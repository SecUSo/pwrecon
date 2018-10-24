#include "WizardPages/attackpage.h"

AttackPage::AttackPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");

    startPushButton = new QPushButton("");
    stopPushButton = new QPushButton("");
    startPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    stopPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    attackProgressBar = new QProgressBar();
    htmlOutput = new QLineEdit();
    htmlOutput->setVisible(false);


    QHBoxLayout *startStopLayout = new QHBoxLayout();
    startStopLayout->addWidget(startPushButton);
    startStopLayout->addWidget(attackProgressBar);
    startStopLayout->addWidget(stopPushButton);

    attackResultTextBrowser = new QTextBrowser();
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(attackResultTextBrowser);

    QVBoxLayout *extractLayout = new QVBoxLayout;
    extractLayout->addLayout(startStopLayout);
    extractLayout->addLayout(displayLayout);

    setLayout(extractLayout);

    // Set up Hashcat
#ifdef Q_OS_OSX
    hc2_fallback = false;

    if (!hc2_fallback)
    {
        std::vector<cl::Platform> all_platforms;

        try {
            cl::Platform::get(&all_platforms);
            if (all_platforms.size() == 0) {
                hc2_fallback = true;
                QString msg_en("(OpenCL is not installed on this system.\nUsing older Hashcat version.)\n");
                QString msg_de("(OpenCL ist auf diesem System nicht installiert.\nÄltere Version von Hashcat wird verwendet.)\n");
                attackResultTextBrowser->append(msg_de);
            }else
            {
                qDebug() << "OpenCl is installed" << endl;
            }
        }
        catch (...)
        {
            hc2_fallback=true;
        }
    }


#endif // ifndef Q_MV_OSX
    binarydir = QString(QDir::currentPath() + "/tools/Hashcat4");
    binaryfile = QString(binarydir + "/hashcat");

#ifdef Q_PROCESSOR_X86_64

    binaryfile.append("64");
#endif

#ifdef Q_PROCESSOR_X86_32
    binaryfile.append("32");
#endif


#ifdef Q_OS_WIN
    binaryfile.append(".exe");
#endif
#ifdef Q_OS_OSX // No support yet
    binarydir = QString(QDir::currentPath() + "/tools/HashcatMAC");
    binaryfile = QString(binarydir + "/hashcat");
    binaryfile.append(".ppa");

#endif
#ifdef Q_OS_LINUX // No support yet
    binarydir = QString(QDir::currentPath() + "/tools/Hashcat4");
    binaryfile = QString(binarydir + "/hashcat64");
    binaryfile.append(".bin");
#endif
    if(field("EXPERTMODE").toBool()){
        dictfile = field("DICTIONARY").toString();
    } else{
        dictfile = QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");
        show_plain_pwds = false;

    }
    qDebug() << binaryfile << endl;
    potfile = QString(binarydir + "/hashcat.potfile");
    tempfilepath= QString(QDir::currentPath() + "/tools/tempfile.txt");
    testpwdfilepath= QString(QDir::currentPath() + "/tools/testpwdfile.txt");
    samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");


    // Check if Hashcat files exist
    QFile file(binaryfile);
    qDebug() << binarydir << endl << binaryfile << endl;
    if (!file.exists()) {
        attackResultTextBrowser->setText(trUtf8("Keine Hashcat Dateien gefunden!\n"));
        qDebug() << trUtf8("Keine Hashcat Dateien gefunden!\n");
        startPushButton->setDisabled(true);
        stopPushButton->setDisabled(true);
    }
    // Setup new thread for Hashcat binary
    rWorker = new RecoveryWorker(hc2_fallback, binarydir, binaryfile, dictfile, potfile, tempfilepath);
    rWorker->moveToThread(&workerThread);

    qRegisterMetaType<QMultiHash<QString, QString> >("QMultiHash<QString, QString>");

    connect(&workerThread, &QThread::finished, rWorker, &QObject::deleteLater);
    QObject::connect(startPushButton, SIGNAL(clicked()),this, SLOT(start()));
    QObject::connect(stopPushButton, SIGNAL(clicked()),this, SLOT(stop()));
    QObject::connect(this, &AttackPage::startRecovery, rWorker, &RecoveryWorker::onRecoveryStarted);
    QObject::connect(this, &AttackPage::stopProcess, rWorker, &RecoveryWorker::onProcessStopped);
    connect(rWorker, &RecoveryWorker::finishRecovery, this, &AttackPage::onRecoveryFinished);
    connect(rWorker, &RecoveryWorker::txtBrowserSet, this, &AttackPage::onTxtBrowserSet);
    connect(rWorker, &RecoveryWorker::txtBrowserAppend, this, &AttackPage::onTxtBrowserAppend);
    connect(rWorker, &RecoveryWorker::setResultData, this, &AttackPage::setResultData);


    workerThread.start();
    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
    isValid = false;

    registerField("HTMLATTACK", htmlOutput);
}

int AttackPage::nextId() const
{
    return Page_Results;
}

void AttackPage::start()
{
    isValid = true;
    qDebug() << "Start button has been pressed!" << endl;
    bool showPlain;
    if(field("WORKAROUND").toString() == "true")
    {
        showPlain = true;
    }else if(field("WORKAROUND").toString() == "false")
    {
        showPlain = false;
    }
    qDebug() << "Show plain: " << showPlain << endl;
    tmpHashFile = getHashFilePath();
    qDebug() << "FilePath: " << tmpHashFile << endl;
    qDebug() << "hash type" << hashtype << endl;
    disableButtons(true);
    attackResultTextBrowser->clear();
    attackResultTextBrowser->setText(trUtf8("Die Widerherstellen wurde gestartet.\n"
                                            "Dieser Vorgang kann einige Zeit dauern."));
    emit startRecovery(!field("SHOWHIDEPASSWORD").toBool(), field("EXPERTMODE").toBool(), tmpHashFile, hashtype);
}

void AttackPage::stop()
{
    qDebug() << "Stop button has been pressed!" << endl;
    emit stopProcess();
}

AttackPage::~AttackPage()
{
    workerThread.quit();
    workerThread.wait();
    deleteTemporaryFiles();
}

QString AttackPage::getHashFilePath()
{
    samdumpfilepath = QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    /* qDebug() << "Workaround: \t" << field("WORKAROUND") << endl;
    int len = wizard()->visitedPages().length();
    qDebug() << "Length of visitedPages: " << len << endl;
    for(int i = 0; i < len; i++)
        qDebug() << "Value of visitedPages at " << i << ": " << wizard()->visitedPages().at(i) << endl;
    */

    if(wizard()->visitedPages().contains(Page_EnterPassword))
    {
        if(checkFieldByName("PATHLABEL"))
        {
            qDebug() << "Field PATHLABEL" << endl;
            QStringList passwordList;
            passwordList = passwordListFromFile(field("PATHLABEL").toString());
            QString path = passwordListToTmpFile(passwordList);
            hashtype = QString("--hash-type=0");
            return path;

        }else if(checkFieldByName("PASSWORDLINEEDIT"))
        {
            qDebug() << "Field PASSWORDLINEEDIT " << endl;
            QStringList password;
            password << field("PASSWORDLINEEDIT").toString();
            QString passwordFile = passwordListToTmpFile(password);
            hashtype = QString("--hash-type=0");
            return passwordFile;
        }
    }else if(wizard()->visitedPages().contains(Page_EnterHash))
    {
        if(checkFieldByName("HASHLINEEDIT"))
        {
            qDebug() << "Field HASHLINEEDIT" << endl;
            QFile file(testpwdfilepath);
            if (file.exists())
            {
                file.remove();
            }
            if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);
                stream << "pwRecon:" << field("HASHLINEEDIT").toString() << endl;
            }
            file.close();
            hashtype = getHashType();
            return testpwdfilepath;
        }else if(checkFieldByName("HASHPATHLABEL"))
        {
            hashtype = getHashType();
            qDebug() << "Field HASHPATHLABEL" << endl;
            if (QFile::exists(testpwdfilepath))
            {
                QFile::remove(testpwdfilepath);
            }
            QFile::copy(field("HASHPATHLABEL").toString(), testpwdfilepath);
            return testpwdfilepath;
        }
    }else if(wizard()->visitedPages().contains(Page_ExtractCurrent))
    {
        qDebug() << "Field EXTRACTPATHLABEL" << endl;
#ifdef Q_OS_WIN
        hashtype = QString("--hash-type=1000");
#endif
#ifdef Q_OS_OSX
        hashtype = QString("--hash-type=7100");
#endif
#ifdef Q_OS_LINUX
        hashtype = QString("--hash-type=1800");
#endif
        return field("EXTRACTPATHLABEL").toString();
    }

    return "";
}

bool AttackPage::checkFieldByName(QString fieldName)
{
    if(!field(fieldName).isValid())
    {
        return false;
    }
    if(field(fieldName).isNull())
    {
        return false;
    }
    if(field(fieldName).toString() == "")
    {
        return false;
    }
    return true;
}

QString AttackPage::passwordListToTmpFile(QStringList passwordList)
{
    QFile file(testpwdfilepath);
    if (file.exists())
    {
        file.remove();
    }
    QByteArray hashstr;
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(int i = 0; i < passwordList.size(); i++)
        {
            hashstr = QCryptographicHash::hash(QByteArray(passwordList.at(i).toUtf8()), QCryptographicHash::Md5).toHex();
            stream << "pwRecon:" << hashstr << endl;
        }
    }
    file.close();
    return testpwdfilepath;
}
/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

QStringList AttackPage::passwordListFromFile(QString passwordListFilePath)
{
    QStringList passwordList;
    QFile file(passwordListFilePath);
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            int i = 0;
            while(!stream.atEnd()) {
                passwordList << stream.readLine();
                i++;
            }
        }
    }
    file.close();
    return passwordList;
}

QString AttackPage::getHashType()
{
    QString actHash = field("HASHTYPE").toString();
    qDebug() << "Actual Hash value: " << actHash << "\t" << field("HASHTYPE") << endl;

    if (actHash == "NTML") {
        return QString("--hash-type=1000");
    }
    else if (actHash == "MD5") {
        return QString("--hash-type=0");
    }
    else if (actHash == "SHA-1") {
        return QString("--hash-type=100");
    }
    else if (actHash == "SHA-256") {
        return QString("--hash-type=1400");
    }
    else if (actHash == "SHA-512") {
        return QString("--hash-type=1700");
    }
    return QString("--hash-type=0");
}

void AttackPage::onTxtBrowserSet(const QString& str)
{
    attackResultTextBrowser->setText(str);
}

void AttackPage::onTxtBrowserAppend(const QString& str)
{
    attackResultTextBrowser->append(str);
}

void AttackPage::onRecoveryFinished()
{
    qDebug() << "Recovery Finished" << endl;
    // Delete pot-file immediately after finish.
    writeHTML();
    QFile file(potfile);
    if (file.exists())
        file.remove();

    attackResultTextBrowser->append(QString(" - " + trUtf8("WIEDERHERSTELLUNG ABGESCHLOSSEN") + " -\n------------------------------------------\n\n"));
    disableButtons(false);
}

void AttackPage::deleteTemporaryFiles()
{
    QFile file1(potfile);
    if (file1.exists())
        file1.remove();

    QFile file2(tempfilepath);
    if (file2.exists())
        file2.remove();

    QFile file3(testpwdfilepath);
    if (file3.exists())
        file3.remove();

//    if(!field("EXTRACTPATHLABEL").isValid())
//    {
//        QFile file4(field("EXTRACTPATHLABEL").toString());
//        if (file4.exists())
//            file4.remove();
//    }

    QFile file5(QString(binarydir + "/hashcat.log"));
    if (file5.exists())
        file5.remove();
}

void AttackPage::disableButtons(bool bol)
{
    startPushButton->setDisabled(bol);
    wizard()->button(QWizard::BackButton)->setDisabled(bol);
    wizard()->button(QWizard::NextButton)->setDisabled(bol);
    stopPushButton->setEnabled(bol);

    if (bol)
        attackProgressBar->setMaximum(0);
    else
        attackProgressBar->setMaximum(23);
}

void AttackPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Wiederherstellung"));
        setSubTitle(trUtf8("Sie können die Wiederherstellung Ihrer Passwörter starten."));
        startPushButton->setText(trUtf8("Start"));
        stopPushButton->setText(trUtf8("Stop"));
    } else
        QWidget::changeEvent(event);
}

void AttackPage::initializePage()
{
    attackResultTextBrowser->setText(trUtf8("Sie können die Wiederherstellung mit dem Klicken auf Start starten."));
    attackResultTextBrowser->append(trUtf8("Dieser Prozess wird alle verfügbaren Ressourcen für die Wiederherstellung verwenden."));
    attackResultTextBrowser->append(trUtf8("Andere Prozesse könnten verzögert werden."));
}

void AttackPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);

    //    if (visible) {
    //        wizard()->button(QWizard::FinishButton)->setEnabled(false);
    //        wizard()->button(QWizard::CustomButton1)->setEnabled(false);
    //        wizard()->setButtonText(QWizard::CustomButton1, tr("&Neue Wiederherstellung Starten"));
    //        wizard()->setOption(QWizard::HaveCustomButton1, true);
    //        connect(wizard(), &QWizard::customButtonClicked,
    //                wizard(), &QWizard::restart);
    //    } else {
    //        wizard()->setOption(QWizard::HaveCustomButton1, false);
    //        disconnect(wizard(), &QWizard::customButtonClicked,
    //                   wizard(), &QWizard::restart);
    //    }
}

bool AttackPage::validatePage()
{
    if(!isValid)
    {
        QMessageBox::warning(this, "pwRecon",
                             trUtf8("Die Wiederherstellung wurde noch nicht gestartet."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
    return isValid;
}

void AttackPage::writeHTML()
{
    bool recover;
    bool show_plain_pwds = !field("SHOWHIDEPASSWORD").toBool();
    bool expertMode = field("EXPERTMODE").toBool();
    if(wizard()->visitedPages().contains(Page_ExtractCurrent) || wizard()->visitedPages().contains(Page_EnterHash))
    {
        recover = true;
    }else if(wizard()->visitedPages().contains(Page_EnterPassword))
    {
        recover = false;
    }
    QString htmlStr;

    if(recover)
    {
        htmlStr = "<font style=\"color:Green;\">";
    }else{
        htmlStr = "<font style=\"color:Red;\">";
    }
    htmlStr.append("<font  SIZE=\"+1\">");
    htmlStr.append(trUtf8(" - WIEDERHERGESTELLTE PASSWÖRTER:"));
    htmlStr.append("</font>");
    htmlStr.append("<br><br>");

    QStringList foundPasswords;

    // Hashcat pot-file contains already cracked hashes and their plain texts
    QFile file(potfile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            foundPasswords << in.readLine();
        }
    }

    file.close();
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
            pw_line.append(passwordString + ": " + hash + " ");
        }else{
            count++;
            pw_line.append(passwordString + " #" + QString::number(count)  + ": ");
        }
        QString userName = "";
        for (int i = 0; i < users.size(); ++i) {
            userName = users[i];

            if (users.size() > 1 && i < (users.size() - 1)) {
                userName.append(", ");
            }
        }
        if(userName.isEmpty())
        {
            userName = trUtf8("leer");
        }

        pw_line.append(trUtf8("eines Benutzers (%1) wiederhergestellt").arg(userName));
        if (show_plain_pwds)
            pw_line.append("  -->  [" + plaintext + "]");
        // else
        // pw_line.append("  -->  [*****]");
        //pw_line.append("<br>");

        // pw_line.append("- Username(s): { ");
        pwd_count += users.size();
        htmlStr.append(pw_line + "<br>");
    }


    if(!recover)
    {
        htmlStr.append("<font style=\"color:Green;\">");
    }else{
        htmlStr.append("<font style=\"color:Red;\">");
    }

    htmlStr.append("<br><br>");
    htmlStr.append("<font  SIZE=\"+1\">");
    htmlStr.append(trUtf8(" - NICHT WIEDERHERGESTELLTE PASSWÖRTER:"));
    htmlStr.append("</font>");
    htmlStr.append("<br><br>");

    QFile hashfile(tempfilepath);
    hashfile.open(QIODevice::ReadOnly | QIODevice::Text);

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

        if (username.isEmpty()) {
            QList<QString> users = usernames_table.values(hash);
            for (int i = 0; i < users.size(); ++i) {
                username = users[i];

                if (users.size() > 1 && i < (users.size() - 1)) {
                    username.append(", ");
                }
            }
        }

        if (username.isEmpty()) {
            username = trUtf8("leer");
        }

        if(splitted.size() < 1 || splitted.size() > 2 || hash.isEmpty()) {
            break;
        }

        htmlStr.append(trUtf8("Passwort eines Benutzers (%1) nicht wiederhergestellt!").arg(username));
        if(expertMode){
            htmlStr.append(" [" + hash.toLower() + "]<br>");
        }else
        {
            htmlStr.append("<br>");
        }

    }
    hashfile.close();
    htmlStr.append("</font><br>");

    htmlOutput->setText(htmlStr);
}

void AttackPage::setResultData(const QMultiHash<QString, QString> usernames_table, const int pwd_amount)
{
    this->usernames_table = usernames_table;
    this->pwd_amount = pwd_amount;
}
