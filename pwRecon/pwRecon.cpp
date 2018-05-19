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
    setTitle(trUtf8("Einleitung"));
    topLabel = new QLabel();

    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *innerLayout = new QHBoxLayout;
    layout->addWidget(topLabel, 0, Qt::AlignTop);

    languageComboBox = new QComboBox();
    languageComboBox->addItem("Deutsch");
    languageComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    innerLayout->addWidget(languageComboBox, 0, Qt::AlignLeft);

    setLanguagesComboBox();
    layout->addLayout(innerLayout, 100);
    setLayout(layout);

    QObject::connect(languageComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(setLanguage()));

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
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
        //...
        //okPushButton->setText(tr("&OK"));
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
    QString qtTranslateFile = pwReconTranslateFile;
    qtTranslateFile.replace("pwRecon", "qt");
    qDebug() << "###TEST###" << endl << pwReconTranslateFile << endl << qtTranslateFile << endl;

    qApp->removeTranslator(pwReconTranslator);
    qApp->removeTranslator(qtTranslator);

    if( languageComboBox->currentText() != "Deutsch")
    {
    pwReconTranslator = new QTranslator(qApp);
    qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
    if(pwReconTranslator->load(pwReconTranslateFile))
        qApp->installTranslator(pwReconTranslator);

    qtTranslator = new QTranslator(qApp);
    if (qtTranslator->load(qtTranslateFile))
        qApp->installTranslator(qtTranslator);
    }
}
