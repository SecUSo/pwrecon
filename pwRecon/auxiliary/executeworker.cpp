#include "executeworker.h"

executeWorker::executeWorker(QString programPath, QStringList arguments, QObject *parent) : QObject(parent)
{
    this->programPath = programPath;
    this->arguments = arguments;
}
executeWorker::executeWorker(QString programPath, QObject *parent) : QObject(parent)
{
    this->programPath = programPath;
}

void executeWorker::startWorker()
{
    qDebug() << "Started Process in eWorker" << endl;
    binaryProcess = new QProcess(this);
    connect(binaryProcess, &QProcess::started, this, &executeWorker::onStart);
    connect(binaryProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &executeWorker::onFinish);
    binaryProcess->setProgram(programPath);
    //binaryProcess->setWorkingDirectory(binarydir);
    binaryProcess->setArguments(arguments);
    binaryProcess->start();

}

void executeWorker::stop()
{

}

void executeWorker::onStart()
{
   // start();
    qDebug() << "Debug Output From Started process" << endl;
}

void executeWorker::onFinish()
{

    QString line;
    QStringList output;// = new QStringList();
    QByteArray outputData = binaryProcess->readAllStandardOutput();
    QTextStream outputStream(outputData);
    QByteArray errorData = binaryProcess->readAllStandardError();
    QTextStream errorStream(errorData);

    qDebug() << endl << "- BINARY OUTPUT:" << endl;

    line = outputStream.readLine();
    output << line;
    while(!line.isNull()) {
        qDebug() << line;
        line = outputStream.readLine();
        output << line;
        //output << outputStream.readLine();
    }

    line = errorStream.readLine();
    qDebug() << endl << "- BINARY ERRORS:" << endl;
    while(!line.isNull()) {
        qDebug() << line;
        line = errorStream.readLine();
    }

    emit sendOutput(output);
}

void executeWorker::setArguments(QStringList arguments)
{
    qDebug() << "setArguments()" << endl;
    this->arguments = arguments;
}
