#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"

class ResultsPage  : public QWizardPage
{
    Q_OBJECT

public:
    explicit ResultsPage(QWidget *parent = 0);

    int nextId() const override;

private:
    QTextBrowser *ResultTextBrowser;


signals:

public slots:
};

#endif // RESULTSPAGE_H
