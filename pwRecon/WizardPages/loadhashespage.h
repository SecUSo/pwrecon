#ifndef LOADHASHESPAGE_H
#define LOADHASHESPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"

class LoadHashesPage : public QWizardPage
{
    Q_OBJECT

public:
    LoadHashesPage(QWidget *parent = 0);

    int nextId() const override;
    QString currentFile;

private:
    QPushButton *loadPushButton;
    QLabel *pathLabel;

public slots:
    void load(void);

};

#endif // LOADHASHESPAGE_H
