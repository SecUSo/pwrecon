#include "WizardPages\enterpasswordpage.h"

EnterPasswordPage::EnterPasswordPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Passwörter prüfen"));
    setSubTitle(trUtf8("Bitte geben Sie ein Passwort oder eine Passwortliste zum testen an."));

    QGroupBox *hideGroupBox = new QGroupBox(trUtf8("&Möchten Sie die Passwörter verbergen?"));
    QGroupBox *actionGroupBox = new QGroupBox(trUtf8("&Was möchten Sie tun?"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hideLayout = new QHBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *enterOuterLayout = new QHBoxLayout;
    QHBoxLayout *loadOuterLayout = new QHBoxLayout;

    // layout->addLayout(enterOuterLayout);
    //layout->addLayout(loadOuterLayout);

    // The hide Radio buttons
    showPasswordRadioButton = new QRadioButton(trUtf8("&Passwörter zeigen"));
    hidePasswordRadioButton = new QRadioButton(trUtf8("&Passwörter verbergen"));
    hideLayout->addWidget(showPasswordRadioButton);
    hideLayout->addWidget(hidePasswordRadioButton);
    hideGroupBox->setLayout(hideLayout);
    layout->addWidget(hideGroupBox);
    hidePasswordRadioButton->setChecked(true);

    selectEnterPasswordRadioButton = new QRadioButton(trUtf8("&Ein einzelnes Passwort angeben."));
    actionLayout->addWidget(selectEnterPasswordRadioButton);

    passwordLabel = new QLabel(trUtf8("Passwort:"));
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    // Connenct the label with the text field
    passwordLabel->setBuddy(passwordLineEdit);

    enterOuterLayout->addWidget(passwordLabel);
    enterOuterLayout->addWidget(passwordLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadPasswordRadioButton = new QRadioButton(trUtf8("&Eine Passwortliste laden."));
    actionLayout->addWidget(selectLoadPasswordRadioButton);

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton(trUtf8("Passwortliste auswählen"));
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
                                           trUtf8("Wählen Sie eine Passwoetliste aus."), "", "*");
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

bool EnterPasswordPage::validatePage()
{
    if(selectEnterPasswordRadioButton->isChecked())
    {
        if(passwordLineEdit->text() == "")
        {
            QMessageBox::warning(this, trUtf8("pwRecon"),
                                 trUtf8("Bitte geben Sie ein Passwort ein."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        pathLabel->setText("");
    } else if(selectLoadPasswordRadioButton->isChecked()){
        if(pathLabel->text() == "")
        {
            QMessageBox::warning(this, trUtf8("pwRecon"),
                                 trUtf8("Bitte wählen Sie eine Passwortliste aus."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        passwordLineEdit->setText("");
    }
    return true;
}
