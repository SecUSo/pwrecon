/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#include "sam_dialog.h"
#include "ui_sam_dialog.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QFile>
#include <QProcess>
#include <QTextStream>

SAMDialog::SAMDialog(QString filepath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAMDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);

    samdumpfilepath = filepath;
}

SAMDialog::~SAMDialog()
{
    delete ui;
}

void SAMDialog::on_startButton_clicked()
{
    ui->startButton->setDisabled(true);
    ui->buttonBox->setDisabled(true);

    QProcess pwdumpProcess;
    pwdumpProcess.setProgram(QString(QDir::currentPath() + "/tools/pwdump7/PwDump7.exe"));
    pwdumpProcess.setWorkingDirectory(QString(QDir::currentPath() + "/tools/pwdump7"));

    pwdumpProcess.start();

    while (!pwdumpProcess.waitForFinished()) {
        QThread::msleep(10);
    }

    QByteArray data1 = pwdumpProcess.readAllStandardOutput();
    QTextStream outputStream1(data1);
    QByteArray data2 = pwdumpProcess.readAllStandardError();
    QTextStream outputStream2(data2);

    bool breakup = false;
    QStringList tempfilestring;

    QString line;

    line = outputStream1.readLine();
    qDebug() << endl << "PWDUMP OUTPUT LOG: " << endl;
    while(!line.isNull()) {
        qDebug() << line;

        QStringList splitted = line.split(":");

        if (splitted.size() == 7) {

            QString username(splitted[0]);
            QString hash(splitted[3]);

            if (hash.contains(QString("NO PASSWORD"), Qt::CaseInsensitive)) {
                hash = QString("31D6CFE0D16AE931B73C59D7E0C089C0");
            }

            tempfilestring.append(QString(username + ":" + hash.toLower()));

        } else {
            breakup = true;
        }

        line = outputStream1.readLine();
    }

    line = outputStream2.readLine();
    qDebug() << endl << "PWDUMP ERROR OUTPUT LOG: " << endl;
    while(!line.isNull()) {
        qDebug() << line;
        line = outputStream2.readLine();
    }


    if (breakup) {
        qDebug() << endl << "PWDUMP NO SUCCESS" << endl;

        ui->errorLabel->setText("Error:\nProcess not possible.");
        ui->buttonBox->setEnabled(true);
    }
    else {
        qDebug() << endl << "PWDUMP SUCCESS" << endl;

        QFile tempfile(samdumpfilepath);
        if (tempfile.exists())
            tempfile.remove();
        if (tempfile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&tempfile);
            foreach (const QString &line, tempfilestring) {
                stream << line << endl;
            }
        }
        tempfile.close();

        emit sendImport();
        close();
    }
}
