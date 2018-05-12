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
    setPage(Page_EnterPassword, new EnterPasswordPage);
    setPage(Page_EnterHash, new EnterHashPage);
   // setPage(Page_LoadPasswords, new LoadPasswordsPage);
   // setPage(Page_LoadHashes, new LoadHashesPage);
    setPage(Page_ExtractCurrent, new ExtractCurrentPage);
    setPage(Page_AttackSettings, new AttackSettingsPage);
    setPage(Page_Attack, new AttackPage);
    setPage(Page_Results, new ResultsPage);

    setStartId(Page_Intro);

#ifdef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif

    QTranslator *translator2 = new QTranslator(qApp);
    qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
    if(translator2->load("pwRecon_en_US.qm", "C:/Users/Christoph/Documents/TU/Thesis/pwReconGIT/pwRecon"))
        qApp->installTranslator(translator2);

     // remove the old translator
     //qApp->removeTranslator(&m_translatorQt);

    m_translatorQt = new QTranslator(qApp);
   /* if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);*/

     // load the new translator
//     QFileInfo check_file("C:/Users/Christoph/Documents/TU/Thesis/pwReconGIT/pwRecon/pwRecon_en_US.qm");
//     qDebug() << "File check: " << check_file.exists() << endl;
//     if(m_translatorQt->load("pwRecon_en_US.qm", "C:/Users/Christoph/Documents/TU/Thesis/pwReconGIT/pwRecon"))
//     {
//        qDebug() << "It works!" << endl;
//        qApp->installTranslator(m_translatorQt);

//     }else
//     {

//         qDebug() << "Something went Wrong!\n" << m_translatorQt->isEmpty() << endl;
//     }

   //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

    connect(this, &QWizard::helpRequested, this, &pwRecon::showHelp);

    setWindowTitle(trUtf8("pwRecon"));
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

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
//    setTitle(trUtf8("Introduction"));
    setTitle(trUtf8("Einleitung"));
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    /*topLabel = new QLabel(trUtf8("This wizard will guide you through the process of recovering your lost password.\n"
                             "You can choose between different kinds of Passwords to recover or test.\n"
                             "It is also possible to test the security of existing passwords."));*/
    topLabel = new QLabel(trUtf8("Dieser Assistent wird Sie durch den Prozess der Wiederherstellung verlorener Passwörter führen.\n"
                             "Sie können sich zwischen zwei verschiedenen Wiederherstellungsarten entscheiden.\n"
                             "Es besteht auch die Möglichkeit die Sicherheit bereits bestehender Passwörter zu testen."));

    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    setLayout(layout);

//    WORKAROUNDLineEdit = new QLabel();
//    WORKAROUNDLineEdit->setVisible(false);
//    registerField("PathHash",WORKAROUNDLineEdit);
//    WORKAROUNDLineEdit = new QLabel();
//    WORKAROUNDLineEdit->setVisible(false);
//    registerField("PathPassword",WORKAROUNDLineEdit);
//    WORKAROUNDLineEdit = new QLabel();
//    WORKAROUNDLineEdit->setVisible(false);
//    registerField("PathSam",WORKAROUNDLineEdit);
}

int IntroPage::nextId() const
{

        return Page_SelectMode;
}

void IntroPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        topLabel->setText(trUtf8("Dieser Assistent wird Sie durch den Prozess der Wiederherstellung verlorener Passwörter führen.\n"
                                 "Sie können sich zwischen zwei verschiedenen Wiederherstellungsarten entscheiden.\n"
                                 "Es besteht auch die Möglichkeit die Sicherheit bereits bestehender Passwörter zu testen."));
        //...
        //okPushButton->setText(tr("&OK"));
    } else
        QWidget::changeEvent(event);
}
