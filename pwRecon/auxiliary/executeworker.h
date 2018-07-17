#ifndef EXECUTEWORKER_H
#define EXECUTEWORKER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDebug>

#include <QFileInfo>

class executeWorker : public QObject
{
    Q_OBJECT
public:
    explicit executeWorker(QString, QStringList, QObject *parent = nullptr);
    executeWorker(QString, QObject *parent = nullptr);
    void setArguments(QStringList);

private:
    QString programPath;
    QStringList arguments;
    QProcess *binaryProcess;

    void stop();
    QString execute();

signals:
    void sendOutput(const QStringList& output);
    void sendErrorOutput(const QStringList& errorOutput);
    void onWorkerStarted(); // Just for disabling buttons

public slots:
    void startWorker();
    void onStart();
    void onFinish();


};

#endif // EXECUTEWORKER_H
