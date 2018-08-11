#ifndef ATTACKPAGE_H
#define ATTACKPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "auxiliary/recovery_worker.h"
#include <QThread>
#include <QCryptographicHash>
// OpenCL

    #include "opencl/include/CL/cl.hpp"

class AttackPage : public QWizardPage
{
    Q_OBJECT

public:
    AttackPage(QWidget *parent = 0);
    ~AttackPage();

    int nextId() const override;
    void changeEvent(QEvent *event) override;
    void initializePage() override;
    static QStringList passwordListFromFile(QString passwordListFilePath);
    void setVisible(bool visible) override;
    bool validatePage() override;

private:
    QProgressBar *attackProgressBar;
    QPushButton *startPushButton;
    QPushButton *stopPushButton;
    QTextBrowser *attackResultTextBrowser;
    QWizard     *pwReconWizard;
    RecoveryWorker *rWorker;
    QThread workerThread;
    bool hc2_fallback;
    bool show_plain_pwds;
    bool isValid;

    QString binarydir;
    QString binaryfile;
    QString dictfile;
    QString potfile;
    QString hashfilepath;
    QString hashtype;
    QString tempfilepath;
    QString tmpHashFile;
    QString testpwdfilepath;
    QString samdumpfilepath;
    QString basePath;

    QString getHashFilePath();
    QString passwordListToTmpFile(QStringList passwordList);
    QString getHashType();
    void disableButtons(bool bol);
    bool checkFieldByName(QString fieldName);

public slots:
    void start();
    void stop();
    void onTxtBrowserSet(const QString& str);
    void onTxtBrowserAppend(const QString& str);
    void onRecoveryFinished();
    void deleteTemporaryFiles();

signals:

    void startRecovery(const bool& showplain, const bool& expertMode, const QString& newhashfilepath, const QString& newhashtype);
    void stopProcess();

};

#endif // ATTACKPAGE_H
