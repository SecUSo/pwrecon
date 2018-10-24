/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef EVALUATIONPAGE_H
#define EVALUATIONPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "attackpage.h"

class EvaluationPage  : public QWizardPage
{
    Q_OBJECT

public:
    explicit EvaluationPage(QWidget *parent = 0);
    ~EvaluationPage();
    int nextId() const override;
    void initializePage() override;
    bool checkFieldByName(QString);


private:
    QTextBrowser *ResultTextBrowser;
    QProgressBar * resultsProgressBar;
    executeWorker *eWorker;
    QStringList currentResults;
    QLineEdit *htmlTextEdit;

    QThread workerThread;

    QStringList getArguments();
    QStringList parseOutput(QStringList);
    void disableButtons(bool bol);
    void triggerEstimation();
    void possibleOutputs();
    void changeEvent(QEvent *event) override;

signals:
    void startEstemation();
public slots:
    void onEstimationFinished(const QStringList& output);
    void onEstimationStarted();
};

#endif // EVALUATIONPAGE_H
