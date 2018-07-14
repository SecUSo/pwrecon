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
    bool checkFieldByName(QString);


private:
    QTextBrowser *ResultTextBrowser;
    QProgressBar * resultsProgressBar;
    QTimer *timerClock;
    executeWorker *eWorker;
    QStringList currentResults;

    QThread workerThread;

    QStringList getArguments();
    QStringList parseOutput(QStringList);


signals:
    void startEstemation();
public slots:
    void onTickTimer();
    void onEstimationFinished(const QStringList& output);
};

#endif // RESULTSPAGE_H
