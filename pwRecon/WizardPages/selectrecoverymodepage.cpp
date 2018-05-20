#include "selectrecoverymodepage.h"

SelectRecoveryModePage::SelectRecoveryModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel();
    topLabel->setWordWrap(true);

    recoverHashRadioButton = new QRadioButton();
    recoverLocalPwFileRadioButton = new QRadioButton();
    recoverLocalPwFileRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(recoverLocalPwFileRadioButton);
    layout->addWidget(recoverHashRadioButton);
    setLayout(layout);

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

}

int SelectRecoveryModePage::nextId() const
{
    if (recoverHashRadioButton->isChecked()) {
        return Page_EnterHash;
    } else if (recoverLocalPwFileRadioButton->isChecked()) {
        return Page_ExtractCurrent;
    }
    return Page_ExtractCurrent;

}

void SelectRecoveryModePage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Art der Wiederherstellung auswählen."));
        topLabel->setText(trUtf8("Sie können auswählen welche Art von Passwort wiedergestellt werden soll."));
        recoverHashRadioButton->setText(trUtf8("&Ein Passwort von einem Hash wiederherstellen"));
        recoverLocalPwFileRadioButton->setText(trUtf8("&Die Passwörter der Benutzerkonten dieses Computers wiederherstellen."));
    } else
        QWidget::changeEvent(event);
}

void SelectRecoveryModePage::initializePage()
{
     qDebug() << "TMP = " << field("EXPERTMODE").toBool() << endl;
}