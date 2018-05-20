#include "selectmodepage.h"

SelectModePage::SelectModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel();
    topLabel->setWordWrap(true);

    checkPasswordRadioButton = new QRadioButton();
    recoverPasswordRadioButton = new QRadioButton();
    checkPasswordRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(checkPasswordRadioButton);
    layout->addWidget(recoverPasswordRadioButton);

    setLayout(layout);
    WORKAROUNDLineEdit = new QLineEdit("true");
    WORKAROUNDLineEdit->setVisible(false);
    registerField("WORKAROUND",WORKAROUNDLineEdit);

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

}

int SelectModePage::nextId() const
{
    if (checkPasswordRadioButton->isChecked()) {
        return Page_EnterPassword;
    } else if (recoverPasswordRadioButton->isChecked()) {
        return Page_SelectRecoveryMode;
    }
    return Page_EnterPassword;

}

void SelectModePage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Vorgansweise Auswählen."));
        topLabel->setText(trUtf8("Sie können sich entscheiden, ob sie ein Passwort testen oder wiederherstellen möchten."));
        checkPasswordRadioButton->setText(trUtf8("&Ein Passwort prüfen"));
        recoverPasswordRadioButton->setText(trUtf8("&Ein Passwörter wiederherstellen"));
    } else
        QWidget::changeEvent(event);
}

