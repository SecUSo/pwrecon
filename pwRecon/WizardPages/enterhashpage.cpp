#include "WizardPages\enterhashpage.h"

EnterHashPage::EnterHashPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Test a Hash"));
    setSubTitle(tr("Please enter a hash or a hash list to test."));

    QGroupBox *hideGroupBox = new QGroupBox(tr("&Do you want to hide your Password?"));
    QGroupBox *actionGroupBox = new QGroupBox(tr("&What do you want to do?"));

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
    selectHashModeLabel = new QLabel(tr("Select the Hash Algorithm"));
    selectHashLayout->addWidget(selectHashModeLabel);
    selectHashLayout->addWidget(selectHashModeCombo);
    hideGroupBox->setLayout(selectHashLayout);
    layout->addWidget(hideGroupBox);

    selectEnterHashRadioButton = new QRadioButton(tr("&Enter a single Hash"));
    actionLayout->addWidget(selectEnterHashRadioButton);

    hashLabel = new QLabel(tr("Hash:"));
    hashLineEdit = new QLineEdit;
    // Connenct the label with the text field
    hashLabel->setBuddy(hashLineEdit);

    enterOuterLayout->addWidget(hashLabel);
    enterOuterLayout->addWidget(hashLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadHashRadioButton = new QRadioButton(tr("&Load a list of Passwords"));
    actionLayout->addWidget(selectLoadHashRadioButton);

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton(tr("Select Password File"));
    // To prevent the button to get the wrong size
    loadPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    loadPushButton->setEnabled(false);
    pathLabel->setBuddy(loadPushButton);

    loadOuterLayout->addWidget(loadPushButton);
    loadOuterLayout->addWidget(pathLabel);
    actionLayout->addLayout(loadOuterLayout);
    actionGroupBox->setLayout(actionLayout);
    selectEnterHashRadioButton->setChecked(true);

    layout->addWidget(actionGroupBox);
    setLayout(layout);

    QObject::connect(selectEnterHashRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(selectLoadHashRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));
}
int EnterHashPage::nextId() const
{
    return Page_AttackSettings;
}

void EnterHashPage::load()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           tr("Choose a file containing a Hash list"), "", "*");
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
