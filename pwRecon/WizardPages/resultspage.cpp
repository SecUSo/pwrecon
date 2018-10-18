/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

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
    if(wizard()->visitedPages().contains(Page_Evaluation))
    {
        ResultTextBrowser->setText(field("HTMLEVAL").toString());
    }else if(wizard()->visitedPages().contains(Page_Attack))
    {
        qDebug() << "Attack HTML: \n" <<  field("HTMLATTACK").toString() << endl;
        ResultTextBrowser->setText(field("HTMLATTACK").toString());
    }
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
        setTitle(trUtf8("Ergebnisse"));
        setSubTitle(trUtf8("Das sind Ihre Resultate"));

    } else
        QWidget::changeEvent(event);
}
