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

private:
    QLabel *topLabel;
    QRadioButton *checkPasswordRadioButton;
    QRadioButton *checkHashRadioButton;
    QRadioButton *checkLocalPwFileRadioButton;
};

#endif // SELECTMODEPAGE_H
