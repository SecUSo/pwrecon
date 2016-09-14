/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "recovery_worker.h"

#include <QMainWindow>
#include <QThread>
#include <QString>

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QThread workerThread;
    RecoveryWorker *rWorker;

    static const QString hashname_win;
    static const QString hashname_md5;

    bool hc2_fallback;
    bool show_plain_pwds;

    QString binarydir;
    QString binaryfile;
    QString dictfile;
    QString potfile;
    QString hashfilepath;
    QString hashtype;
    QString tempfilepath;
    QString testpwdfilepath;
    QString samdumpfilepath;

    void disableButtons(bool bol);
    void deleteTemporaryFiles();

signals:

    void startRecovery(const bool& showplain, const QString& newhashfilepath, const QString& newhashtype);
    void stopProcess();

public slots:

    void onAnswerQuestion(bool bol);
    void onTxtBrowserSet(const QString& str);
    void onTxtBrowserAppend(const QString& str);
    void onSetPassword(const QString& pwd);
    void onSAMImport();
    void onRecoveryFinished();

private slots:

    void onAskQuestion();
    void on_selectFileButton_clicked();
    void on_testPasswordButton_clicked();
    void on_SAM_Button_clicked();
    void on_startButton_clicked();
    void on_stopProcessButton_clicked();
    void on_hashSelectBox_currentIndexChanged(const QString &arg1);

};

#endif // MAINWINDOW_H
