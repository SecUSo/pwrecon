#include "WizardPages\attackpage.h"

AttackPage::AttackPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select the attack mode for this test."));
    setSubTitle(tr("Please chose between Dictionary Arrack and Brute Force attack"));

    startPushButton = new QPushButton(tr("Start"));
    stopPushButton = new QPushButton(tr("Stop"));
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
    }

#ifdef Q_OS_WIN
    binaryfile.append(".exe");
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
        attackResultTextBrowser->setText(msg_de);
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

}

int AttackPage::nextId() const
{
    return Page_Results;
}

void AttackPage::start()
{
    qDebug() << "Start button has been pressed!" << endl;
    emit startRecovery(show_plain_pwds, hashfilepath, hashtype);
    qDebug() << field("WORKAROUND") << endl;
}

void AttackPage::stop()
{
    qDebug() << "Stop button has been pressed!" << endl;
}
