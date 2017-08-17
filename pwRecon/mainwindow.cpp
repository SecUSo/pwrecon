/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "type_password_dialog.h"
#include "sam_dialog.h"
#include "question_dialog.h"

// OpenCL
#include "CL/cl.hpp"

#include <QFile>
#include <QFileDialog>

// Strings for hash type dropdown menu
const QString MainWindow::hashname_win = QString("Windows (NTLM)");   // --hash-type=1000
const QString MainWindow::hashname_md5 = QString("MD5");              // --hash-type=0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SAM file button is not visible by default
    ui->or2Label->setVisible(false);
    ui->SAM_Button->setVisible(false);


    // Fill hash type dropdown menu
    ui->hashSelectBox->addItem(hashname_win);
    ui->hashSelectBox->addItem(hashname_md5);


    // Check if OpenCL is installed, fallback to older Hashcat version if not


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
        ui->textBrowser->append(msg_de);
    }
    }
    catch (...)
    {
        hc2_fallback=true;
    }
    }

    // Define environment variables
    if (hc2_fallback) {
        // Current Hashcat version: 2.00
        binarydir = QString(QDir::currentPath() + "/tools/Hashcat2");
        binaryfile = QString(binarydir + "/hashcat-cli");

#ifdef Q_PROCESSOR_X86_64
        binaryfile.append("64");
#endif
#ifdef Q_PROCESSOR_X86_32
        binaryfile.append("32");
#endif

    } else {
        // Current Hashcat version: 3.10
        binarydir = QString(QDir::currentPath() + "/tools/Hashcat3");
        binaryfile = QString(binarydir + "/hashcat");

#ifdef Q_PROCESSOR_X86_64
        binaryfile.append("64");
#endif
#ifdef Q_PROCESSOR_X86_32
        binaryfile.append("32");
#endif
    }

#ifdef Q_OS_WIN
    binaryfile.append(".exe");
    // Scan SAM file function only possible in Windows
    ui->or2Label->setVisible(true);
    ui->SAM_Button->setVisible(true);
#endif
#ifdef Q_OS_OSX // No support yet
    binaryfile.append(".app");
#endif
#ifdef Q_OS_LINUX // No support yet
    binaryfile.append(".bin");
#endif

    dictfile = QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");
    potfile = QString(binarydir + "/hashcat.pot");
    tempfilepath= QString(QDir::currentPath() + "/tools/tempfile.txt");
    testpwdfilepath= QString(QDir::currentPath() + "/tools/testpwdfile.txt");
    samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");


    // Check if Hashcat files exist
    QFile file(binaryfile);
    if (!file.exists()) {
        QString msg_en("No Hashcat files found!\n");
        QString msg_de("Keine Hashcat Dateien gefunden!\n");
        ui->textBrowser->setText(msg_de);
        ui->startButton->setDisabled(true);
        ui->selectFileButton->setDisabled(true);
        ui->testPasswordButton->setDisabled(true);
        ui->stopProcessButton->setDisabled(true);
        return;
    }


    // Setup new thread for Hashcat binary
    rWorker = new RecoveryWorker(hc2_fallback, binarydir, binaryfile, dictfile, potfile, tempfilepath);
    rWorker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, rWorker, &QObject::deleteLater);

    connect(this, &MainWindow::startRecovery, rWorker, &RecoveryWorker::onRecoveryStarted);
    connect(rWorker, &RecoveryWorker::finishRecovery, this, &MainWindow::onRecoveryFinished);
    connect(rWorker, &RecoveryWorker::txtBrowserSet, this, &MainWindow::onTxtBrowserSet);
    connect(rWorker, &RecoveryWorker::txtBrowserAppend, this, &MainWindow::onTxtBrowserAppend);
    connect(this, &MainWindow::stopProcess, rWorker, &RecoveryWorker::onProcessStopped);

    workerThread.start();


    // Ask if the recovered password should be shown in plain text
    QTimer::singleShot(200, this, SLOT(onAskQuestion()));
}

MainWindow::~MainWindow()
{
    deleteTemporaryFiles();

    workerThread.quit();
    workerThread.wait();

    delete ui;
}

void MainWindow::onAnswerQuestion(bool bol)
{
    show_plain_pwds = bol;
}

void MainWindow::onTxtBrowserSet(const QString& str)
{
    ui->textBrowser->setText(str);
}

void MainWindow::onTxtBrowserAppend(const QString& str)
{
    ui->textBrowser->append(str);
}

void MainWindow::onSetPassword(const QString& pwd)
{
    QString str;
    if (pwd.isEmpty()) {
        QString en("(empty)");
        QString de("(leer)");
        str = de;
    } else {
        str = pwd;
    }

    QString shownstr(str);
    if (!show_plain_pwds)
        shownstr.fill(QChar('*'));
    QString en("PASSWORD");
    QString de("PASSWORT");
    ui->hashfilepathLine->setText(QString(de + ": [" + shownstr + "]"));

    hashfilepath = testpwdfilepath;

    ui->hashSelectBox->setCurrentText(hashname_md5);

    on_startButton_clicked();
}

void MainWindow::onSAMImport()
{
    QString en("SAM FILE");
    QString de("SAM-DATEI");
    ui->hashfilepathLine->setText(QString("--- " + de + " ---"));

    hashfilepath = samdumpfilepath;

    ui->hashSelectBox->setCurrentText(hashname_win);

    on_startButton_clicked();
}

void MainWindow::onRecoveryFinished()
{
    // Delete pot-file immediately after finish.
    QFile file(potfile);
    if (file.exists())
        file.remove();

    disableButtons(false);

    QString en("RECOVERY FINISHED");
    QString de("WIEDERHERSTELLUNG ABGESCHLOSSEN");
    ui->textBrowser->append(QString(" - " + de + " -\n------------------------------------------\n\n"));
}

void MainWindow::onAskQuestion()
{
    QuestionDialog qdialog;

    connect(&qdialog, &QuestionDialog::answerQuestion, this, &MainWindow::onAnswerQuestion);

    qdialog.exec();
}

void MainWindow::on_selectFileButton_clicked()
{
    QString en1("Select Hash File");
    QString de1("Hash-Datei auswählen");
    QString en2("Text Files (*.txt)");
    QString de2("Textdateien (*.txt)");
    hashfilepath = QFileDialog::getOpenFileName(
                this,
                de1,
                QDir::currentPath(),
                de2
                );

    ui->hashfilepathLine->setText(QDir::toNativeSeparators(hashfilepath));

    ui->hashSelectBox->setCurrentText(hashname_win);
    ui->hashSelectBox->setEnabled(true);

    if (hashfilepath.isEmpty()) {
        ui->startButton->setDisabled(true);
    } else {
        ui->startButton->setEnabled(true);
    }
}

void MainWindow::on_testPasswordButton_clicked()
{
    TypePasswordDialog tpdialog(show_plain_pwds, testpwdfilepath);

    connect(&tpdialog, &TypePasswordDialog::setPassword, this, &MainWindow::onSetPassword);

    tpdialog.exec();
}

void MainWindow::on_SAM_Button_clicked()
{
    SAMDialog samdialog(samdumpfilepath);

    connect(&samdialog, &SAMDialog::sendImport, this, &MainWindow::onSAMImport);

    samdialog.exec();
}

void MainWindow::on_startButton_clicked()
{
    disableButtons(true);

    emit startRecovery(show_plain_pwds, hashfilepath, hashtype);
}

void MainWindow::on_stopProcessButton_clicked()
{
    emit stopProcess();
}

void MainWindow::on_hashSelectBox_currentIndexChanged(const QString &arg1)
{
    // Map hash names to Hashcat arguments:

    if (arg1 == hashname_win) {
        hashtype = QString("--hash-type=1000");
    }
    else if (arg1 == hashname_md5) {
        hashtype = QString("--hash-type=0");
    }
}

/**
 * Disable buttons while process is running.
 *
 * @param bol: true to disable, false to revert.
 */
void MainWindow::disableButtons(bool bol)
{
    ui->startButton->setDisabled(bol);
    ui->selectFileButton->setDisabled(bol);
    ui->testPasswordButton->setDisabled(bol);
    ui->SAM_Button->setDisabled(bol);
    ui->hashSelectBox->setDisabled(bol);

    ui->stopProcessButton->setEnabled(bol);

    if (bol)
        ui->progressBar->setMaximum(0);
    else
        ui->progressBar->setMaximum(23);
}

void MainWindow::deleteTemporaryFiles()
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

    QFile file4(samdumpfilepath);
    if (file4.exists())
        file4.remove();

    QFile file5(QString(binarydir + "/hashcat.log"));
    if (file5.exists())
        file5.remove();
}
