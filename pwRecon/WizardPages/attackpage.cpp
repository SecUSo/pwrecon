#include "WizardPages/attackpage.h"

AttackPage::AttackPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");
//    setTitle(trUtf8"Select the attack mode for this test."));
//    setSubTitle(tr("Please chose between Dictionary Arrack and Brute Force attack"));

    startPushButton = new QPushButton("");
    stopPushButton = new QPushButton("");
    startPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    stopPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    attackProgressBar = new QProgressBar();


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
#ifndef Q_OS_MACX
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


        binarydir = QString(QDir::currentPath() + "/tools/Hashcat4");
        binaryfile = QString(binarydir + "/hashcat");

#ifdef Q_PROCESSOR_X86_64

        binaryfile.append("64");
#endif

#ifdef Q_PROCESSOR_X86_32
        binaryfile.append("32");
#endif

#endif // ifndef Q_OS_MACX

#ifdef Q_OS_WIN
    binaryfile.append(".exe");
#endif
#ifdef Q_OS_MACX // No support yet
    binarydir = QString(QDir::currentPath() + "/tools/HashcatMAC");
    binaryfile = QString(binarydir + "/hashcat");
    binaryfile.append(".ppa");

#endif
#ifdef Q_OS_LINUX // No support yet
    binaryfile.append(".bin");
#endif
    if(field("EXPERTMODE").toBool()){
        dictfile = field("DICTIONARY").toString();
    } else{
        dictfile = QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");
        show_plain_pwds = false;

    }
    potfile = QString(binarydir + "/hashcat.potfile");
    tempfilepath= QString(QDir::currentPath() + "/tools/tempfile.txt");
    testpwdfilepath= QString(QDir::currentPath() + "/tools/testpwdfile.txt");
    samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");


    // Check if Hashcat files exist
    QFile file(binaryfile);
    qDebug() << binarydir << endl << binaryfile << endl;
    if (!file.exists()) {
        attackResultTextBrowser->setText(trUtf8("Keine Hashcat Dateien gefunden!\n"));
        startPushButton->setDisabled(true);
        stopPushButton->setDisabled(true);
    }
    // Setup new thread for Hashcat binary
    rWorker = new RecoveryWorker(hc2_fallback, binarydir, binaryfile, dictfile, potfile, tempfilepath);
    rWorker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, rWorker, &QObject::deleteLater);
    QObject::connect(startPushButton, SIGNAL(clicked()),this, SLOT(start()));
    QObject::connect(stopPushButton, SIGNAL(clicked()),this, SLOT(stop()));
    QObject::connect(this, &AttackPage::startRecovery, rWorker, &RecoveryWorker::onRecoveryStarted);
    QObject::connect(this, &AttackPage::stopProcess, rWorker, &RecoveryWorker::onProcessStopped);
    connect(rWorker, &RecoveryWorker::finishRecovery, this, &AttackPage::onRecoveryFinished);
    connect(rWorker, &RecoveryWorker::txtBrowserSet, this, &AttackPage::onTxtBrowserSet);
    connect(rWorker, &RecoveryWorker::txtBrowserAppend, this, &AttackPage::onTxtBrowserAppend);


    workerThread.start();
    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
}

// TODO: Start attack directly?

int AttackPage::nextId() const
{
    // TODO: Is estimation still wanted???
    // TODO: Seperate Result Page?
    return -1;
}

void AttackPage::start()
{
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
    qDebug() << "FilePath: " << getHashFilePath() << endl;
    qDebug() << "hash type" << hashtype << endl;
    disableButtons(true);

    emit startRecovery(!field("SHOWHIDEPASSWORD").toBool(), getHashFilePath(), hashtype);
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
    samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");
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
            return field("HASHPATHLABEL").toString();
        }
    }else if(wizard()->visitedPages().contains(Page_ExtractCurrent))
    {
        qDebug() << "Field EXTRACTPATHLABEL" << endl;
#ifdef Q_OS_WIN
        hashtype = QString("--hash-type=1000");
#endif
#ifdef Q_OS_MACX
        hashtype = QString("--hash-type=7100");
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

    //    QFile file2(tempfilepath);
    //    if (file2.exists())
    //        file2.remove();

    //    QFile file3(testpwdfilepath);
    //    if (file3.exists())
    //        file3.remove();

    //    QFile file4(samdumpfilepath);
    //    if (file4.exists())
    //        file4.remove();

    QFile file5(QString(binarydir + "/hashcat.log"));
    if (file5.exists())
        file5.remove();
}

void AttackPage::disableButtons(bool bol)
{
    startPushButton->setDisabled(bol);
    wizard()->button(QWizard::BackButton)->setEnabled(!bol);
    wizard()->button(QWizard::NextButton)->setEnabled(!bol);
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
    wizard()->button(QWizard::NextButton)->setEnabled(false);
    attackResultTextBrowser->clear();
    // TODO: Text for TextBrowser
}
