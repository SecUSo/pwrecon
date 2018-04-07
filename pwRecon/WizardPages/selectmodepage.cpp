#include "selectmodepage.h"

SelectModePage::SelectModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select a mode"));
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel(tr("You can choose which kind of password shall be recovered"));
    topLabel->setWordWrap(true);

    checkPasswordRadioButton = new QRadioButton(tr("&Test a Password"));
    checkHashRadioButton = new QRadioButton(tr("&Test a Hash"));
    checkLocalPwFileRadioButton = new QRadioButton(tr("&Test/Extract local Password File"));
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
    WORKAROUNDLineEdit = new QLineEdit(QString(Path_Password));
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
        setField("WORKAROUND", QString(Path_Password));
    }else if(checkHashRadioButton->isChecked())
    {
        setField("WORKAROUND", QString(Path_Hash));
    }else if(checkLocalPwFileRadioButton->isChecked())
    {
        setField("WORKAROUND", QString(Path_Sam));
    }
}
