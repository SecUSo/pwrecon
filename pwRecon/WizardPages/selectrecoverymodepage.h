#ifndef SELECTRECOVERYCMODEPAGE_H
#define SELECTRECOVERYCMODEPAGE_H

#include <QWidget>
#include <QWizardPage>

#include "pwrecon_global.h"

class SelectRecoveryModePage : public QWizardPage
{
    Q_OBJECT

public:
    SelectRecoveryModePage(QWidget *parent = 0);

    int nextId() const override;
    void changeEvent(QEvent *event) override;

private:
    QLabel *topLabel;
    QRadioButton *recoverHashRadioButton;
    QRadioButton *recoverLocalPwFileRadioButton;

protected:
    void initializePage() override;

public slots:
};

#endif // SELECTMODEPAGE_H