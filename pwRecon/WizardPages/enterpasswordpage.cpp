#include "WizardPages\enterpasswordpage.h"

EnterPasswordPage::EnterPasswordPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Test a password"));
    setSubTitle(tr("Please enter a password or a password list to test."));

    QGroupBox *hideGroupBox = new QGroupBox(tr("&Do you want to hide your Password?"));
    QGroupBox *actionGroupBox = new QGroupBox(tr("&What do you want to do?"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hideLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *enterOuterLayout = new QHBoxLayout;
    QHBoxLayout *loadOuterLayout = new QHBoxLayout;

    // layout->addLayout(enterOuterLayout);
    //layout->addLayout(loadOuterLayout);

    // The hide Radio buttons
    showPasswordRadioButton = new QRadioButton(tr("&Show Password"));
    hidePasswordRadioButton = new QRadioButton(tr("&Hide Password"));
    hideLayout->addWidget(showPasswordRadioButton);
    hideLayout->addWidget(hidePasswordRadioButton);
    hideGroupBox->setLayout(hideLayout);
    layout->addWidget(hideGroupBox);
    showPasswordRadioButton->setChecked(true);

    selectEnterPasswordRadioButton = new QRadioButton(tr("&Enter a single Password"));
    actionLayout->addWidget(selectEnterPasswordRadioButton);

    passwordLabel = new QLabel(tr("Password:"));
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    // Connenct the label with the text field
    passwordLabel->setBuddy(passwordLineEdit);

    enterOuterLayout->addWidget(passwordLabel);
    enterOuterLayout->addWidget(passwordLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadPasswordRadioButton = new QRadioButton(tr("&Load a list of Passwords"));
    actionLayout->addWidget(selectLoadPasswordRadioButton);

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
    selectEnterPasswordRadioButton->setChecked(true);

    layout->addWidget(actionGroupBox);
    setLayout(layout);

    QObject::connect(showPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setHide()));
    QObject::connect(hidePasswordRadioButton, SIGNAL(clicked()),this, SLOT(setHide()));
    QObject::connect(selectEnterPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(selectLoadPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));

    registerField("PATHLABEL",pathLabel,"text", "changeEvent");
    registerField("PASSWORDLINEEDIT",passwordLineEdit);
}
int EnterPasswordPage::nextId() const
{
    return Page_AttackSettings;
}

void EnterPasswordPage::load()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           tr("Choose a file containing a password list"), "", "*");
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
    qDebug() << pathLabel->text() << endl;
}

void EnterPasswordPage::setHide()
{
    if(showPasswordRadioButton->isChecked())
    {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
    }else if(hidePasswordRadioButton->isChecked()){
        passwordLineEdit->setEchoMode(QLineEdit::Password);
    }
}

void EnterPasswordPage::setMode()
{
    if(selectEnterPasswordRadioButton->isChecked())
    {
        loadPushButton->setEnabled(false);
        passwordLineEdit->setEnabled(true);
    } else if(selectLoadPasswordRadioButton->isChecked()){
        loadPushButton->setEnabled(true);
        passwordLineEdit->setEnabled(false);
    }
}
