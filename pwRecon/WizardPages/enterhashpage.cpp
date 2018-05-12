#include "WizardPages/enterhashpage.h"

EnterHashPage::EnterHashPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Hash wiederherstellen"));
    setSubTitle(trUtf8("Bitte geben Sie einen Hash oder eine Hashliste zur Wiederherstellung an."));

    QGroupBox *hideGroupBox = new QGroupBox(trUtf8("&Wählen Sie den verwendeten Hash Algorithmus."));
    QGroupBox *actionGroupBox = new QGroupBox(trUtf8("&Was möchten Sie tun?"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *selectHashLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *enterOuterLayout = new QHBoxLayout;
    QHBoxLayout *loadOuterLayout = new QHBoxLayout;

    // The Select hash mode
    selectHashModeCombo = new QComboBox(); //Qt::DisplayRole
    selectHashModeCombo->addItem("MD5",Qt::DisplayRole);
    selectHashModeCombo->addItem("NTML",Qt::DisplayRole);
    selectHashModeCombo->addItem("SHA-1",Qt::DisplayRole);
    selectHashModeCombo->addItem("SHA-256",Qt::DisplayRole);
    selectHashModeCombo->addItem("SHA-512",Qt::DisplayRole);
    selectHashModeLabel = new QLabel(trUtf8("Hashmode: "));
    selectHashLayout->addWidget(selectHashModeLabel);
    selectHashLayout->addWidget(selectHashModeCombo);
    hideGroupBox->setLayout(selectHashLayout);
    layout->addWidget(hideGroupBox);

    selectEnterHashRadioButton = new QRadioButton(trUtf8("&Einen einzelnen Hash Wiederherstellen"));
    actionLayout->addWidget(selectEnterHashRadioButton);

    hashLabel = new QLabel(trUtf8("Hash:"));
    hashLineEdit = new QLineEdit;
    hashLineEdit->setEnabled(false);
    // Connenct the label with the text field
    hashLabel->setBuddy(hashLineEdit);

    enterOuterLayout->addWidget(hashLabel);
    enterOuterLayout->addWidget(hashLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadHashRadioButton = new QRadioButton(trUtf8("&Eine Hashliste wiederherstellen"));
    actionLayout->addWidget(selectLoadHashRadioButton);

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton(trUtf8("Hashliste auswählen"));
    // To prevent the button to get the wrong size
    loadPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    loadPushButton->setEnabled(true);
    pathLabel->setBuddy(loadPushButton);

    loadOuterLayout->addWidget(loadPushButton);
    loadOuterLayout->addWidget(pathLabel);
    actionLayout->addLayout(loadOuterLayout);
    actionGroupBox->setLayout(actionLayout);
    selectLoadHashRadioButton->setChecked(true);

    layout->addWidget(actionGroupBox);
    setLayout(layout);

    QObject::connect(selectEnterHashRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(selectLoadHashRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));

    registerField("HASHPATHLABEL",pathLabel,"text", "changeEvent");
    registerField("HASHLINEEDIT",hashLineEdit);
    registerField("HASHTYPE", selectHashModeCombo, "currentText", "currentIndexChanged");
}
int EnterHashPage::nextId() const
{
    return Page_AttackSettings;
}

void EnterHashPage::load()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           trUtf8("Hashliste auswählen"), "", "*");
    //tr("Image Files (*.png *.jpg *.bmp)")
    // if equal return 0
    if(QString::compare(tmpFile, "", Qt::CaseInsensitive))
    {
        currentFile = tmpFile;
    }else{
        //ignore
    }
    qInfo(qPrintable(currentFile));
    pathLabel->setText(currentFile);
}

void EnterHashPage::setMode()
{
    if(selectEnterHashRadioButton->isChecked())
    {
        loadPushButton->setEnabled(false);
        hashLineEdit->setEnabled(true);
    } else if(selectLoadHashRadioButton->isChecked()){
        loadPushButton->setEnabled(true);
        hashLineEdit->setEnabled(false);
    }
}

bool EnterHashPage::validatePage()
{
    if(selectEnterHashRadioButton->isChecked())
    {
        if(hashLineEdit->text() == "")
        {
            QMessageBox::warning(this, trUtf8("pwRecon"),
                                 trUtf8("Bitte geben Sie einen Hash ein."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        pathLabel->setText("");
    } else if(selectLoadHashRadioButton->isChecked()){
        if(pathLabel->text() == "")
        {
            QMessageBox::warning(this, trUtf8("pwRecon"),
                                 trUtf8("Bitte geben Sie eine Hashliste an."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        hashLineEdit->setText("");
    }
    return true;
}



