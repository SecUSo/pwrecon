#ifndef ATTACKPAGE_H
#define ATTACKPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"

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
public slots:
    void start();
    void stop();

};

#endif // ATTACKPAGE_H
