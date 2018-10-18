/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#include "WizardPages/enterhashpage.h"

EnterHashPage::EnterHashPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");

    hideGroupBox = new QGroupBox();
    actionGroupBox = new QGroupBox();

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

    selectEnterHashRadioButton = new QRadioButton();
    actionLayout->addWidget(selectEnterHashRadioButton);

    hashLabel = new QLabel();
    hashLineEdit = new QLineEdit;
    hashLineEdit->setEnabled(false);
    // Connenct the label with the text field
    hashLabel->setBuddy(hashLineEdit);

    enterOuterLayout->addWidget(hashLabel);
    enterOuterLayout->addWidget(hashLineEdit);
    actionLayout->addLayout(enterOuterLayout);

    selectLoadHashRadioButton = new QRadioButton();
    actionLayout->addWidget(selectLoadHashRadioButton);

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton();
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

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

    qDebug() << "EnterHashPage()" << endl;
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

void EnterHashPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Hash wiederherstellen"));
        setSubTitle(trUtf8("Bitte geben Sie einen Hash oder eine Hashliste zur Wiederherstellung an."));
        hideGroupBox->setTitle(trUtf8("&Wählen Sie den verwendeten Hash Algorithmus."));
        actionGroupBox->setTitle(trUtf8("&Was möchten Sie tun?"));
        selectEnterHashRadioButton->setText(trUtf8("&Einen einzelnen Hash Wiederherstellen"));
        hashLabel->setText(trUtf8("Hash:"));
        selectLoadHashRadioButton->setText(trUtf8("&Eine Hashliste wiederherstellen"));
        loadPushButton->setText(trUtf8("Hashliste auswählen"));
    } else
        QWidget::changeEvent(event);
}




