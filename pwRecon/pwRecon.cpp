#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#endif


#include "pwRecon.h"

QString emailRegExp = QStringLiteral(".+@.+");

pwRecon::pwRecon(QWidget *parent)
    : QWizard(parent)
{
    translator2 = new QTranslator(qApp);
    setOption(HaveHelpButton, true);
    setPage(Page_Intro, new IntroPage);
    setPage(Page_SelectMode, new SelectModePage);
    setPage(Page_SelectRecoveryMode, new SelectRecoveryModePage);
    setPage(Page_EnterPassword, new EnterPasswordPage);
    setPage(Page_EnterHash, new EnterHashPage);
    setPage(Page_ExtractCurrent, new ExtractCurrentPage);
    setPage(Page_AttackSettings, new AttackSettingsPage);
    setPage(Page_Attack, new AttackPage);
    setPage(Page_Results, new ResultsPage);

    setStartId(Page_Intro);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);

#else
    setWizardStyle(MacStyle);
#endif
    //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

    connect(this, &QWizard::helpRequested, this, &pwRecon::showHelp);

    setWindowTitle("pwRecon");
    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
}

void pwRecon::showHelp()
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
    case Page_Intro:
        message = trUtf8("Wählen eine Widerherstellungsoption aus.");
        //        message = trUtf8("Select the mode you want to use during the Wizard");
        break;

    }
    /*
    if (lastHelpMessage == message)
        message = trUtf8("Sorry, I already gave what help I could. "
                     "Maybe you should try asking a human?");*/

    QMessageBox::information(this, trUtf8("pwRecon Hilfe"), message);

    lastHelpMessage = message;
}

void pwRecon::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        QString backText = trUtf8("Zurück");
        QString nextText = trUtf8("Weiter");
        this->setButtonText(WizardButton::BackButton, "< " + backText);
        this->setButtonText(WizardButton::NextButton, nextText + " >");
        this->setButtonText(WizardButton::CancelButton, trUtf8("Abbrechen"));
        this->setButtonText(WizardButton::HelpButton, trUtf8("Hilfe"));
        //...
        //okPushButton->setText(tr("&OK"));
    } else
        QWidget::changeEvent(event);
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Einleitung"));
    topLabel = new QLabel();

    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *innerLayout = new QHBoxLayout;
    QVBoxLayout *optionsLayout = new QVBoxLayout;
    layout->addWidget(topLabel, 0, Qt::AlignTop);

    languageComboBox = new QComboBox();
    languageComboBox->addItem("Deutsch");
    languageComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    innerLayout->addWidget(languageComboBox, 0, Qt::AlignLeft);

    expertModeCheckBox = new QCheckBox;
    showHidePasswordCheckBox = new QCheckBox;

    QPushButton * Test = new QPushButton("Test");
    QObject::connect(Test, SIGNAL(clicked()),this, SLOT(testSlot()));

    setLanguagesComboBox();
    optionsLayout->addLayout(innerLayout);
    optionsLayout->addWidget(showHidePasswordCheckBox);
    optionsLayout->addWidget(expertModeCheckBox);
    layout->addLayout(optionsLayout, 100);
    layout->addWidget(Test);
    setLayout(layout);

    QObject::connect(languageComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(setLanguage()));
    QObject::connect(expertModeCheckBox, SIGNAL(stateChanged(int)),this, SLOT(setExpertModeTexts()));
    /*qDebug() << "TMP1" << endl;
    QObject::connect(showHidePasswordCheckBox, SIGNAL(stateChanged(int)),wizard()->page(Page_EnterPassword), SLOT(setHide()));
    qDebug() << "TMP2" << endl;*/

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
    registerField("EXPERTMODE", expertModeCheckBox);
    registerField("SHOWHIDEPASSWORD", showHidePasswordCheckBox);

    dirWatcher = new QFileSystemWatcher();
    dirWatcher->addPath("C:/Users/Christoph/Documents/TU/Thesis/hashcat-4.1.0");
    fileWatcher = new QFileSystemWatcher();

    connect(fileWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(printStuff(const QString &)));
    connect(dirWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(dirChanged(const QString &)));
    timerClock = new QTimer(this);
    connect(timerClock, SIGNAL(timeout()), this, SLOT(onTickTimer()));
}

int IntroPage::nextId() const
{

    return Page_SelectMode;
}

void IntroPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Einleitung"));
        topLabel->setText(trUtf8("Dieser Assistent wird Sie durch den Prozess der Wiederherstellung verlorener Passwörter führen.\n"
                                 "Sie können sich zwischen zwei verschiedenen Wiederherstellungsarten entscheiden.\n"
                                 "Es besteht auch die Möglichkeit die Sicherheit bereits bestehender Passwörter zu testen."));
        expertModeCheckBox->setText(trUtf8("Experten Modus verwenden."));
        showHidePasswordCheckBox->setText(trUtf8("Die Passwörter verbergen."));

        QString tmp = field("EXPERTMODE").toBool();
        qDebug() << "TEST: \t" << tmp << endl;
    } else
        QWidget::changeEvent(event);
}

void IntroPage::setLanguagesComboBox()
{
    QDirIterator it("./translate");
    while (it.hasNext()) {
        QString actFile = it.next();
        QFileInfo fi(actFile);
        QString ext = fi.suffix();
        QString actFileName = fi.fileName();
        QString prefix = "pwRecon_";
        if(ext == "qm")
        {
            qDebug() << actFileName << " with ending: " << ext << endl;
            if(actFile.contains(prefix))
            {
                actFileName.replace(prefix, "");
                QString entry = actFileName.mid(0,actFileName.length()-3);
                languageComboBox->addItem(entry, actFile);
            }
        }
    }
}

void IntroPage::setLanguage()
{
    //    QString test = languageComboBox->currentData(Qt::DisplayRole);
    QString pwReconTranslateFile = languageComboBox->itemData(languageComboBox->currentIndex()).toString();
    //qDebug() << "###TEST###" << endl << pwReconTranslateFile << endl << qtTranslateFile << endl;

    qApp->removeTranslator(pwReconTranslator);

    if( languageComboBox->currentText() != "Deutsch")
    {
        pwReconTranslator = new QTranslator(qApp);
        qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
        if(pwReconTranslator->load(pwReconTranslateFile))
            qApp->installTranslator(pwReconTranslator);
    }
}

void IntroPage::setExpertModeTexts()
{
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(wizard(), &languageChangeEvent);
}

void IntroPage::testSlot()
{
    QProcess javaProcess;

    QString java = QString(QDir::currentPath() + "/tools/Test/jre1.8.0_172/bin/java.exe");
    qDebug() << java << endl;
    javaProcess.setProgram(java);
    QStringList args;
    args << "-jar";
    args << QString(QDir::currentPath() + "/tools/Test/nbvcxz-1.4.1.jar");
    args << "JohnSmith123";

    javaProcess.setArguments(args);
    javaProcess.setWorkingDirectory(QString(QDir::currentPath() + "/tools/Test"));
    javaProcess.start();
    while (!javaProcess.waitForFinished()) {
        QThread::msleep(10);
    }

    QByteArray dataJava1 = javaProcess.readAllStandardOutput();
    QTextStream outputStreamJava1(dataJava1);
    QByteArray dataJava2 = javaProcess.readAllStandardError();
    QTextStream outputStreamJava2(dataJava2);

    qDebug() << "### BEGIN Java ###" << endl;
    QString line =  outputStreamJava1.readLine();
    while(!line.isNull()) {
        qDebug() << line;
        line =  outputStreamJava1.readLine();
    }
    qDebug() << "### END Java ###" << endl;
}

//void IntroPage::printStuff(const QString &)
void IntroPage::printStuff()
{
    qDebug() << count++ << endl;
    QFile file("C:/Users/Christoph/Documents/TU/Thesis/hashcat-4.1.0/hashcat.potfile");
    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
        qDebug() << "2" << file.errorString() << endl;
        emit printStuff();
        return;
    }

    QTextStream in(&file);

//    while(!in.atEnd()) {
//        QString line = in.readLine();

//    }
    qDebug() << in.readAll() << endl;

    file.close();
}

void IntroPage::dirChanged(const QString &)
{
    QStringList tmp = fileWatcher->files();

    qDebug() << "TMP 123" << endl;
    qDebug() << tmp.isEmpty() << endl;
    if(tmp.isEmpty())
    {
        qDebug() << "Added file" << endl;
        fileWatcher->addPath("C:/Users/Christoph/Documents/TU/Thesis/hashcat-4.1.0/hashcat.potfile");
        timerClock->start(1000);
    }

}

void IntroPage::onTickTimer()
{
    QFile file("C:/Users/Christoph/Documents/TU/Thesis/hashcat-4.1.0/hashcat.potfile");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "1" <<file.errorString() << endl;
        return;
    }
    file.close();
    timerClock->start(1000);
}
