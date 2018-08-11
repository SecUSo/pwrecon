#include "selecttestmodepage.h"

SelectTestModePage::SelectTestModePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel();
    topLabel->setWordWrap(true);

    testPasswordRadioButton = new QRadioButton();
    simulateAttackRadioButton = new QRadioButton();
    testPasswordRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(testPasswordRadioButton);
    layout->addWidget(simulateAttackRadioButton);
    setLayout(layout);

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

    QObject::connect(testPasswordRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));
    QObject::connect(simulateAttackRadioButton, SIGNAL(clicked()),this, SLOT(setMode()));

    registerField("SIMULATE", simulateAttackRadioButton);
    registerField("EVALUATE", testPasswordRadioButton);

    qDebug() << "SelectTestModePage()" << endl;

}

int SelectTestModePage::nextId() const
{
    return Page_EnterPassword;
}

void SelectTestModePage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Art des Tests auswählen."));
        topLabel->setText(trUtf8("Sie können auswählen wie die Stärke der Passwörter bestimmt werden soll."));
        testPasswordRadioButton->setText(trUtf8("&Passwortstärke evaluieren."));
        simulateAttackRadioButton->setText(trUtf8("&Wiederherstellungs simulation Starten."));
    } else
        QWidget::changeEvent(event);
}

void SelectTestModePage::initializePage()
{
    //TODO: Kann weg?
}

void SelectTestModePage::setMode()
{
    qDebug() << "Field Eval: " << field("EVALUATE").toBool() << endl;
    qDebug() << "Field Eval: " << field("SIMULATE").toBool() << endl;
}

