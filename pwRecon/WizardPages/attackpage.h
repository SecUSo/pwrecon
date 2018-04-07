#ifndef ATTACKPAGE_H
#define ATTACKPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "auxiliary/recovery_worker.h"
#include <QThread>
// OpenCL
#include "opencl\include\CL\cl.hpp"

class AttackPage : public QWizardPage
{
    Q_OBJECT

public:
    AttackPage(QWidget *parent = 0);

    int nextId() const override;

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

    QString binarydir;
    QString binaryfile;
    QString dictfile;
    QString potfile;
    QString hashfilepath;
    QString hashtype;
    QString tempfilepath;
    QString testpwdfilepath;
    QString samdumpfilepath;

public slots:
    void start();
    void stop();

signals:

    void startRecovery(const bool& showplain, const QString& newhashfilepath, const QString& newhashtype);
    void stopProcess();

};

#endif // ATTACKPAGE_H
