#ifndef EXTRACTCURRENTPAGE_H
#define EXTRACTCURRENTPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "pwRecon.h"
#ifdef Q_OS_MACX
#include <QtXml>
#include <QDomDocument>
#endif

class ExtractCurrentPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExtractCurrentPage(QWidget *parent = 0);
#ifdef Q_OS_WIN
    ~ExtractCurrentPage();
#endif

    int nextId() const override;
    bool validatePage() override;
    void changeEvent(QEvent *event) override;

protected:
    void initializePage() override;




private:
    QProgressBar *extractProgressBar;
    QPushButton *extractPushButton;
    QPushButton *changePushButton;
    QLabel *extractPathLabel;
    bool valid;
    bool isRunning;
    bool breakup;

    QTextBrowser *extractResultTextBrowser;

    QString currentFile;
    QString samdumpfilepath;
    void disableButtons(bool bol);

#ifdef Q_OS_WIN
    executeWorker *eWorker;
    QStringList currentResults;
    QStringList errorOutput;

    QThread workerThread;

    QStringList getArguments();
    QStringList parseOutput(QStringList);
    void printError();
#endif

signals:

#ifdef Q_OS_WIN
    void onStartExtraction();
#endif

public slots:
    void startExtraction();
    void change();

#ifdef Q_OS_WIN
    void workerStarted();
    void onExtractionFinished(const QStringList& output);
    void getError(const QStringList& errorOutput);
#endif
};

#endif // EXTRACTCURRENTPAGE_H
