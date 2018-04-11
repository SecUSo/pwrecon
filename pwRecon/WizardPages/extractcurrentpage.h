#ifndef EXTRACTCURRENTPAGE_H
#define EXTRACTCURRENTPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "pwRecon.h"

class ExtractCurrentPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExtractCurrentPage(QWidget *parent = 0);

    int nextId() const override;

protected:
    void initializePage() override;

private:
    QPushButton *extractPushButton;
    QPushButton *changePushButton;
    QLabel *extractPathLabel;

    QTextBrowser *extractResultTextBrowser;

    QString currentFile;
    QString samdumpfilepath;
signals:

public slots:
    void change();
    void startExtraction();
};

#endif // EXTRACTCURRENTPAGE_H
