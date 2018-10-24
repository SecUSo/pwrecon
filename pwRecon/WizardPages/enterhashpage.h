/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef ENTERHASHPAGE_H
#define ENTERHASHPAGE_H
#include <QWizardPage>
#include <QWidget>

#include "pwrecon_global.h"

class EnterHashPage  : public QWizardPage
{
    Q_OBJECT

public:
    explicit EnterHashPage(QWidget *parent = 0);

    int nextId() const override;
    bool validatePage() override;
    void changeEvent(QEvent *event) override;
private:
    QGroupBox *hideGroupBox;
    QGroupBox *actionGroupBox;
    QComboBox *selectHashModeCombo;
    QLabel *selectHashModeLabel;
    QRadioButton *selectEnterHashRadioButton;
    QRadioButton *selectLoadHashRadioButton;
    QLabel *hashLabel;
    QLineEdit *hashLineEdit;
    QPushButton *loadPushButton;
    QLabel *pathLabel;

    QString currentFile;

public slots:
    void load();
    void setMode();
};

#endif // ENTERHASHPAGE_H
