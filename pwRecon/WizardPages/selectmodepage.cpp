#include "selectmodepage.h"

SelectModePage::SelectModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Art der Wiederherstellung auswählen."));
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel(trUtf8("Sie können auswählen welche Art von Passwort wiedergestellt werden soll."));
    topLabel->setWordWrap(true);

    checkPasswordRadioButton = new QRadioButton(trUtf8("&Ein Passwort prüfen"));
    checkHashRadioButton = new QRadioButton(trUtf8("&Ein Passwort von einem Hash wiederherstellen"));
    checkLocalPwFileRadioButton = new QRadioButton(trUtf8("&Die Passwörter der Benutzerkonten dieses Computers wiederherstellen."));
    checkPasswordRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(checkPasswordRadioButton);
    layout->addWidget(checkHashRadioButton);
    layout->addWidget(checkLocalPwFileRadioButton);
    setLayout(layout);
    QObject::connect(checkPasswordRadioButton, SIGNAL(clicked()),this, SLOT(workAroundSlot()));
    QObject::connect(checkHashRadioButton, SIGNAL(clicked()),this, SLOT(workAroundSlot()));
    QObject::connect(checkLocalPwFileRadioButton, SIGNAL(clicked()),this, SLOT(workAroundSlot()));
    WORKAROUNDLineEdit = new QLineEdit("true");
    WORKAROUNDLineEdit->setVisible(false);
    registerField("WORKAROUND",WORKAROUNDLineEdit);

}

int SelectModePage::nextId() const
{
    if (checkPasswordRadioButton->isChecked()) {
        return Page_EnterPassword;
    } else if (checkHashRadioButton->isChecked()) {
        return Page_EnterHash;
    } else if (checkLocalPwFileRadioButton->isChecked()) {
        return Page_ExtractCurrent;
    }
    return Page_AttackSettings;

}

void SelectModePage::workAroundSlot()
{
    if(checkPasswordRadioButton->isChecked())
    {
      //  setField("WORKAROUND", QString(Path_Password));
    }else if(checkHashRadioButton->isChecked())
    {
       // setField("WORKAROUND", QString(Path_Hash));
    }else if(checkLocalPwFileRadioButton->isChecked())
    {
       // setField("WORKAROUND", QString(Path_Sam));
    }
}
