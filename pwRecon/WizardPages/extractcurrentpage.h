#ifndef EXTRACTCURRENTPAGE_H
#define EXTRACTCURRENTPAGE_H

#include <QWizardPage>
#include <QWidget>
#include "pwrecon_global.h"
#include "pwRecon.h"
#ifdef Q_OS_MACX
#include <QtXml>
#include <QDomDocument>
#endif

class ExtractCurrentPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExtractCurrentPage(QWidget *parent = 0);

    int nextId() const override;
    bool validatePage() override;
    void changeEvent(QEvent *event) override;

protected:
    void initializePage() override;


private:
    QProgressBar *extractProgressBar;
    QPushButton *extractPushButton;
    QPushButton *changePushButton;
    QLabel *extractPathLabel;
    bool valid;
    bool isRunning;

    QTextBrowser *extractResultTextBrowser;

    QString currentFile;
    QString samdumpfilepath;

    void disableButtons(bool bol);
signals:

public slots:
    void change();
    void startExtraction();
};

#endif // EXTRACTCURRENTPAGE_H
