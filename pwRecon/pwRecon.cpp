#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#endif


#include "pwRecon.h"

QString emailRegExp = QStringLiteral(".+@.+");

pwRecon::pwRecon(QWidget *parent)
    : QWizard(parent)
{
    setPage(Page_Intro, new IntroPage);
    setPage(Page_AttackSettings, new AttackSettingsPage);
    setPage(Page_SelectMode, new SelectModePage);
    setPage(Page_EnterPassword, new EnterPasswordPage);
    setPage(Page_EnterHash, new EnterHashPage);
    setPage(Page_LoadPasswords, new LoadPasswordsPage);
    setPage(Page_LoadHashes, new LoadHashesPage);
    setPage(Page_ExtractCurrent, new ExtractCurrentPage);
    setPage(Page_Attack, new AttackPage);
    setPage(Page_Results, new ResultsPage);

    setStartId(Page_Intro);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif
    setOption(HaveHelpButton, true);
   //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

    connect(this, &QWizard::helpRequested, this, &pwRecon::showHelp);

    setWindowTitle(tr("pwRecon"));
}

void pwRecon::showHelp()
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
    case Page_Intro:
        message = tr("Select the mode you want to use during the Wizard");
        break;

    }
/*
    if (lastHelpMessage == message)
        message = tr("Sorry, I already gave what help I could. "
                     "Maybe you should try asking a human?");*/

    QMessageBox::information(this, tr("pwRecon Help"), message);

    lastHelpMessage = message;
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel(tr("This wizard will guide you through the process of recovering your lost password.\n"
                             "You can choose between different kinds of Passwords to recover or test.\n"
                             "It is also possible to test the security of existing passwords."));
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
