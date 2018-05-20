#ifndef SELECTMODEPAGE_H
#define SELECTMODEPAGE_H

#include <QWidget>
#include <QWizardPage>

#include "pwrecon_global.h"

class SelectModePage : public QWizardPage
{
    Q_OBJECT

public:
    SelectModePage(QWidget *parent = 0);

    int nextId() const override;
    void changeEvent(QEvent *event) override;

private:
    QLabel *topLabel;
    QRadioButton *checkPasswordRadioButton;
    QRadioButton *recoverPasswordRadioButton;
    QLineEdit *WORKAROUNDLineEdit;

public slots:
};

#endif // SELECTMODEPAGE_H
