#include "WizardPages/resultspage.h"

ResultsPage::ResultsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");

    ResultTextBrowser = new QTextBrowser();

    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->addWidget(ResultTextBrowser);
    setLayout(displayLayout);
}
int ResultsPage::nextId() const
{
    return -1;
}
void ResultsPage::initializePage()
{
    ResultTextBrowser->setText(trUtf8("Hier könnte Ihre Werbung stehen."));
}

ResultsPage::~ResultsPage()
{
}

void ResultsPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);

    if (visible) {
        wizard()->setButtonText(QWizard::CustomButton1, tr("&Neuen Test/Wiederherstellung Starten"));
        wizard()->setOption(QWizard::HaveCustomButton1, true);
        connect(wizard(), &QWizard::customButtonClicked,
                wizard(), &QWizard::restart);
    } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
        disconnect(wizard(), &QWizard::customButtonClicked,
                   wizard(), &QWizard::restart);
    }
}

void ResultsPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Ergebnise"));
        setSubTitle(trUtf8(" "));

    } else
        QWidget::changeEvent(event);
}
