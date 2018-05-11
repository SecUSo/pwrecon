#include "WizardPages\resultspage.h"

ResultsPage::ResultsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Ergebnisse"));
    setSubTitle(trUtf8("Das sind Ihre Ergebnisse"));

    ResultTextBrowser = new QTextBrowser();
    ResultTextBrowser->setText("Hier könnte Ihre Werbung stehen!");
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(ResultTextBrowser);
    setLayout(displayLayout);

}
int ResultsPage::nextId() const
{
    return -1;
}
