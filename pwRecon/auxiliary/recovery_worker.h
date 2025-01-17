/*
 * Copyright (c) 2018 Mustafa Kargi <mustafa.kargi@arcor.de> and Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef RECOVERYWORKER_H
#define RECOVERYWORKER_H

#include <QString>
#include <QProcess>
#include <QTimer>
#include <QMultiHash>

class RecoveryWorker : public QObject
{
    Q_OBJECT

public:

    explicit RecoveryWorker(const bool& fallback,
                            const QString& newbinarydir,
                            const QString& newbinaryfile,
                            const QString& newdictfile,
                            const QString& newpotfile,
                            const QString& newtempfilepath,
                            QObject *parent = 0
            );

private:

    QProcess *binaryProcess;
    QTimer *timerClock;

    QString binarydir;
    QString binaryfile;
    QString dictfile;
    QString potfile;
    QString hashfilepath;
    QString hashtype;
    QString tempfilepath;

    QStringList args_dict_attack;
    QStringList args_mang1_attack;
    QStringList args_mang2_attack;
    QStringList args_brute_attack;

    QMultiHash<QString, QString> usernames_table;

    bool hc2_fallback;
    bool expertMode;
    bool show_plain_pwds;
    bool finish;

    int pwd_amount;
    int minutes;
    int seconds;
    int interval_count;

    QStringList foundPasswords;

    void resetData();
    bool importHashfile();
    void setupBinaryArguments();
    void startNextRound();
    void startDictAttack();
    void startMang1Attack();
    void startMang2Attack();
    void startBruteAttack();
    void showResults();
    void endProcess();
    void checkEmptyHashfile();
    bool isHex(QString str);
    bool fileChanged();

public slots:

    void onRecoveryStarted(const bool& showplain, const bool& expertMode, const QString& newhashfilepath, const QString& newhashtype);
    void onProcessStopped();

private slots:

    void onProcessStarted();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onTickTimer();

signals:

    void txtBrowserSet(const QString& str);
    void txtBrowserAppend(const QString& str);
    void finishRecovery();
    void setResultData(const QMultiHash<QString, QString> usernames_table, const int pwd_amount);

};

#endif // RECOVERYWORKER_H
