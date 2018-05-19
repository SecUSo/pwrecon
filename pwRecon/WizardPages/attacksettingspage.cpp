#include "attacksettingspage.h"

AttackSettingsPage::AttackSettingsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle(" ");

    currentDictFile =  QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");

    hideGroupBox = new QGroupBox();
    actionGroupBox = new QGroupBox();

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hideLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *dictionaryOuterLayout = new QHBoxLayout;
    QHBoxLayout *bruteOuterLayout = new QHBoxLayout;

    // The hide Radio buttons
    showPasswordRadioButton = new QRadioButton();
    hidePasswordRadioButton = new QRadioButton();
    hideLayout->addWidget(showPasswordRadioButton);
    hideLayout->addWidget(hidePasswordRadioButton);
    hideGroupBox->setLayout(hideLayout);
    layout->addWidget(hideGroupBox);
    hidePasswordRadioButton->setChecked(true);
    setField("WORKAROUND", QString("false"));

    selectDictAttackRadioButton = new QRadioButton();
    actionLayout->addWidget(selectDictAttackRadioButton);

    selectDictionaryButton = new QPushButton();
    selectDictionaryButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    selectDictionaryLabel = new QLabel(currentDictFile);
    selectDictionaryLabel->setWordWrap(true);
    dictionaryOuterLayout->addWidget(selectDictionaryButton);
    dictionaryOuterLayout->addWidget(selectDictionaryLabel);
    actionLayout->addLayout(dictionaryOuterLayout);

    selectBruteAttackRadioButton = new QRadioButton();
    actionLayout->addWidget(selectBruteAttackRadioButton);
    actionLayout->addLayout(bruteOuterLayout);
    actionGroupBox->setLayout(actionLayout);
    selectDictAttackRadioButton->setChecked(true);
    layout->addWidget(actionGroupBox);

    QObject::connect(selectDictionaryButton, SIGNAL(clicked()),this, SLOT(selectDictionary()));
    QObject::connect(showPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setHide()));
    QObject::connect(hidePasswordRadioButton, SIGNAL(clicked()),this, SLOT(setHide()));
    QObject::connect(selectBruteAttackRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(selectDictAttackRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));

    setLayout(layout);

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

}

int AttackSettingsPage::nextId() const
{
    return Page_Attack;
}

void AttackSettingsPage::selectDictionary()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           trUtf8("Wörterbuch auswählen"), "", "*");
    //tr("Image Files (*.png *.jpg *.bmp)")
    // if equal return 0
    if(QString::compare(tmpFile, "", Qt::CaseInsensitive))
    {
        currentDictFile = tmpFile;
    }else{
        //ignore
    }
    qInfo(qPrintable(currentDictFile));
    selectDictionaryLabel->setText(currentDictFile);
}

void AttackSettingsPage::setHide()
{
    if(showPasswordRadioButton->isChecked())
    {
        setField("WORKAROUND", "true");
    }else if(hidePasswordRadioButton->isChecked()){
        qDebug() << "hide password" << endl;
        setField("WORKAROUND", "false");
    }
}

void AttackSettingsPage::setMode()
{
    if(selectDictAttackRadioButton->isChecked())
    {
        selectDictionaryButton->setEnabled(true);
    } else if(selectBruteAttackRadioButton->isChecked()){
        selectDictionaryButton->setEnabled(false);
    }
}

void AttackSettingsPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Angriffseinstellungen"));
        hideGroupBox->setTitle(trUtf8("&Möchten Sie die Passwörter verbergen?"));
        actionGroupBox->setTitle(trUtf8("&Was möchten Sie tun?"));
        showPasswordRadioButton->setText(trUtf8("&Passwörter zeigen"));
        hidePasswordRadioButton->setText(trUtf8("&Passwörter verbergen"));
        selectDictAttackRadioButton->setText(trUtf8("&Wörterbuch Angriff verwenden"));
        selectDictionaryButton->setText(trUtf8("Wörterbuch wechseln"));
        selectBruteAttackRadioButton->setText(trUtf8("&Brute-Force Angriff verwenden"));
    } else
        QWidget::changeEvent(event);
}

