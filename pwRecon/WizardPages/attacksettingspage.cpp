#include "attacksettingspage.h"

AttackSettingsPage::AttackSettingsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Angriffseinstellungen"));
    setSubTitle(trUtf8(" "));

    currentDictFile =  QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");

    QGroupBox *hideGroupBox = new QGroupBox(trUtf8("&Möchten Sie die Passwörter verbergen?"));
    QGroupBox *actionGroupBox = new QGroupBox(trUtf8("&Was möchten Sie tun?"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hideLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *dictionaryOuterLayout = new QHBoxLayout;
    QHBoxLayout *bruteOuterLayout = new QHBoxLayout;

    // The hide Radio buttons
    showPasswordRadioButton = new QRadioButton(trUtf8("&Passwörter zeigen"));
    hidePasswordRadioButton = new QRadioButton(trUtf8("&Passwörter verbergen"));
    hideLayout->addWidget(showPasswordRadioButton);
    hideLayout->addWidget(hidePasswordRadioButton);
    hideGroupBox->setLayout(hideLayout);
    layout->addWidget(hideGroupBox);
    hidePasswordRadioButton->setChecked(true);
    setField("WORKAROUND", QString("false"));

    selectDictAttackRadioButton = new QRadioButton(trUtf8("&Wörterbuch Angriff verwenden"));
    actionLayout->addWidget(selectDictAttackRadioButton);

    selectDictionaryButton = new QPushButton(trUtf8("Wörterbuch wechseln"));
    selectDictionaryButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    selectDictionaryLabel = new QLabel(currentDictFile);
    selectDictionaryLabel->setWordWrap(true);
    dictionaryOuterLayout->addWidget(selectDictionaryButton);
    dictionaryOuterLayout->addWidget(selectDictionaryLabel);
    actionLayout->addLayout(dictionaryOuterLayout);

    selectBruteAttackRadioButton = new QRadioButton(trUtf8("&Brute-Force Angriff verwenden"));
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
