#include "WizardPages\resultspage.h"

ResultsPage::ResultsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Results"));
    setSubTitle(tr("These are your testresults"));

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
