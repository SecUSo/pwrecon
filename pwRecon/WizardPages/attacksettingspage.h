/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QWizardPage>

#include "pwrecon_global.h"

class AttackSettingsPage : public QWizardPage
{
    Q_OBJECT
public:
    AttackSettingsPage(QWidget *parent = 0);

    int nextId() const override;
    void changeEvent(QEvent *event) override;

private:
    QGroupBox *hideGroupBox;
    QGroupBox *actionGroupBox;
    QRadioButton *showPasswordRadioButton;
    QRadioButton *hidePasswordRadioButton;
    QRadioButton *selectDictAttackRadioButton;
    QRadioButton *selectBruteAttackRadioButton;
    QPushButton *selectDictionaryButton;
    QLabel *selectDictionaryLabel;

    QString currentDictFile;
public slots:
    void selectDictionary();
    void setHide();
    void setMode();
};

#endif // SETTINGSPAGE_H
