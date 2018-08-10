#include "WizardPages/enterpasswordpage.h"

EnterPasswordPage::EnterPasswordPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");

    hideGroupBox = new QGroupBox();
    actionGroupBox = new QGroupBox();

    QVBoxLayout *layout = new QVBoxLayout;
    QVBoxLayout *actionLayout = new QVBoxLayout;
    QHBoxLayout *enterOuterLayout = new QHBoxLayout;
    QHBoxLayout *loadOuterLayout = new QHBoxLayout;

    // layout->addLayout(enterOuterLayout);
    //layout->addLayout(loadOuterLayout);

    selectEnterPasswordRadioButton = new QRadioButton();
    actionLayout->addWidget(selectEnterPasswordRadioButton);

    passwordLabel = new QLabel();
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    // Connenct the label with the text field
    passwordLabel->setBuddy(passwordLineEdit);

    enterOuterLayout->addWidget(passwordLabel);
    enterOuterLayout->addWidget(passwordLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadPasswordRadioButton = new QRadioButton();
    actionLayout->addWidget(selectLoadPasswordRadioButton);

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton();
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

    QObject::connect(selectEnterPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(selectLoadPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));

    registerField("PATHLABEL",pathLabel,"text", "changeEvent");
    registerField("PASSWORDLINEEDIT",passwordLineEdit);

    qDebug() << "EnterPasswordPage() 1" << endl;
    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

    qDebug() << "EnterPasswordPage()" << endl;
}
int EnterPasswordPage::nextId() const
{
    if(field("EXPERTMODE").toBool()){
        return Page_Results;
    } else{
        return Page_Results;
    }
}

void EnterPasswordPage::load()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           trUtf8("Wählen Sie eine Passwortliste aus."), "", "*");
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
    if(field("SHOWHIDEPASSWORD").toBool())
    {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
    }else if(hidePasswordRadioButton->isChecked()){
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
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
            QMessageBox::warning(this, "pwRecon",
                                 trUtf8("Bitte geben Sie ein Passwort ein."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        pathLabel->setText("");
    } else if(selectLoadPasswordRadioButton->isChecked()){
        if(pathLabel->text() == "")
        {
            QMessageBox::warning(this, "pwRecon",
                                 trUtf8("Bitte wählen Sie eine Passwortliste aus."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
            return false;
        }
        passwordLineEdit->setText("");
    }
    return true;
}

void EnterPasswordPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Passwörter prüfen"));
        setSubTitle(trUtf8("Bitte geben Sie ein Passwort oder eine Passwortliste zum testen an."));
        actionGroupBox->setTitle(trUtf8("&Was möchten Sie tun?"));
        selectEnterPasswordRadioButton->setText(trUtf8("&Ein einzelnes Passwort angeben."));
        passwordLabel->setText(trUtf8("Passwort:"));
        selectLoadPasswordRadioButton->setText(trUtf8("&Eine Passwortliste laden."));
        loadPushButton->setText(trUtf8("Passwortliste auswählen"));

    } else
        QWidget::changeEvent(event);
}

void EnterPasswordPage::initializePage()
{
    qDebug() << "PW INIT: " << field("SHOWHIDEPASSWORD").toBool() << endl;

    if(field("SHOWHIDEPASSWORD").toBool())
    {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
    }else {
        passwordLineEdit->setEchoMode(QLineEdit::Normal);
    }
}
