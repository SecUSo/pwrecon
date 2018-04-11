#include "attacksettingspage.h"

AttackSettingsPage::AttackSettingsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Attack Settings"));
    setSubTitle(tr(" "));

    currentDictFile =  QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");

    QGroupBox *hideGroupBox = new QGroupBox(tr("&Do you want to hide your Password?"));
    QGroupBox *actionGroupBox = new QGroupBox(tr("&What do you want to do?"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hideLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *dictionaryOuterLayout = new QHBoxLayout;
    QHBoxLayout *bruteOuterLayout = new QHBoxLayout;

    // The hide Radio buttons
    showPasswordRadioButton = new QRadioButton(tr("&Show Password"));
    hidePasswordRadioButton = new QRadioButton(tr("&Hide Password"));
    hideLayout->addWidget(showPasswordRadioButton);
    hideLayout->addWidget(hidePasswordRadioButton);
    hideGroupBox->setLayout(hideLayout);
    layout->addWidget(hideGroupBox);
    showPasswordRadioButton->setChecked(true);
    setField("WORKAROUND", QString("true"));

    selectDictAttackRadioButton = new QRadioButton(tr("&Use a dictionary Attack"));
    actionLayout->addWidget(selectDictAttackRadioButton);

    selectDictionaryButton = new QPushButton(tr("Change Dictionary File"));
    selectDictionaryButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    selectDictionaryLabel = new QLabel(currentDictFile);
    selectDictionaryLabel->setWordWrap(true);
    dictionaryOuterLayout->addWidget(selectDictionaryButton);
    dictionaryOuterLayout->addWidget(selectDictionaryLabel);
    actionLayout->addLayout(dictionaryOuterLayout);

    selectBruteAttackRadioButton = new QRadioButton(tr("&Use a brute-force Attack"));
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
                                           tr("Choose a dictionary file"), "", "*");
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
