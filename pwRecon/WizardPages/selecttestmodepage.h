/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef SELECTTESTMODEPAGE_H
#define SELECTTESTMODEPAGE_H

#include <QWidget>
#include <QWizardPage>

#include "pwrecon_global.h"

class SelectTestModePage : public QWizardPage
{
    Q_OBJECT

public:
    SelectTestModePage(QWidget *parent = 0);

    int nextId() const override;
    void changeEvent(QEvent *event) override;

private:
    QLabel *topLabel;
    QRadioButton *testPasswordRadioButton;
    QRadioButton *simulateAttackRadioButton;

protected:
    void initializePage() override;

public slots:
    void setMode();
};

#endif // SELECTMODEPAGE_H
