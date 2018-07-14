#ifndef ENTERPASSWORDPAGE_H
#define ENTERPASSWORDPAGE_H

#include <QWidget>
#include <QWizardPage>

#include "pwrecon_global.h"
class EnterPasswordPage  : public QWizardPage
{
    Q_OBJECT

public:
    EnterPasswordPage(QWidget *parent = 0);
    void changeEvent(QEvent *event) override;
    void initializePage() override;

    int nextId() const override;
    bool validatePage() override;
private:
    QGroupBox *hideGroupBox;
    QGroupBox *actionGroupBox;
    QRadioButton *showPasswordRadioButton;
    QRadioButton *hidePasswordRadioButton;
    QRadioButton *selectEnterPasswordRadioButton;
    QRadioButton *selectLoadPasswordRadioButton;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loadPushButton;
    QLabel *pathLabel;

    QString currentFile;

public slots:
    void load();
    void setHide();
    void setMode();
};

#endif // ENTERPASSWORDPAGE_H
