#ifndef LOADPASSWORDSPAGE_H
#define LOADPASSWORDSPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"

class LoadPasswordsPage : public QWizardPage
{
    Q_OBJECT

public:
    LoadPasswordsPage(QWidget *parent = 0);

    int nextId() const override;
    QString currentFile;

private:
    QPushButton *loadPushButton;
    QLabel *pathLabel;

public slots:
    void load(void);
};

#endif // LOADPASSWORDSPAGE_H
