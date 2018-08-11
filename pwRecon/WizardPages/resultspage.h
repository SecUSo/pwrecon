#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "attackpage.h"

class ResultsPage  : public QWizardPage
{
    Q_OBJECT

public:
    explicit ResultsPage(QWidget *parent = 0);
    ~ResultsPage();
    int nextId() const override;
    void initializePage() override;


private:
    QTextBrowser *ResultTextBrowser;
    QStringList currentResults;

    void setVisible(bool visible) override;
    void changeEvent(QEvent *event) override;

signals:
public slots:
};

#endif // RESULTSPAGE_H
