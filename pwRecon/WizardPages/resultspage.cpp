#include "WizardPages/resultspage.h"

ResultsPage::ResultsPage(QWidget *parent)
    : QWizardPage(parent)
{
    // TODO: Change Caption
    // TODO: Disable buttons when started
    // TODO: Add text to textbrowser when page opens to tell what is going to happen.
    // TODO: Implement Language changed
    setTitle(trUtf8("Ergebnisse"));
    setSubTitle(trUtf8("Das sind Ihre Ergebnisse"));

    timerClock = new QTimer(this);
    connect(timerClock, SIGNAL(timeout()), this, SLOT(onTickTimer()));
    ResultTextBrowser = new QTextBrowser();

    resultsProgressBar = new QProgressBar();
    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->addWidget(resultsProgressBar);
    displayLayout->addWidget(ResultTextBrowser);
    setLayout(displayLayout);



    QString programPath = QString(QDir::currentPath() + "/tools/Test/jre1.8.0_172/bin/java.exe");
    qDebug() << programPath << endl;

    eWorker = new executeWorker(programPath);
    eWorker->moveToThread(&workerThread);
    QObject::connect(&workerThread, &QThread::finished, eWorker, &QObject::deleteLater);
    QObject::connect(this,&ResultsPage::startEstemation, eWorker, &executeWorker::startWorker);
    //QObject::connect(eWorker, SIGNAL(sendOutput(const QStringList&)), this, SLOT(onEstimationFinished(const QStringList&)));
    QObject::connect(eWorker, &executeWorker::sendOutput, this, &ResultsPage::onEstimationFinished);
    workerThread.start();

}
int ResultsPage::nextId() const
{
    return -1;
}

// TODO: Make Passwords hidable

void ResultsPage::initializePage()
{
    resultsProgressBar->setMaximum(0);
    eWorker->setArguments(getArguments());
    emit ResultsPage::startEstemation();
}

bool ResultsPage::checkFieldByName(QString fieldName)
{
    if(!field(fieldName).isValid())
    {
        return false;
    }
    if(field(fieldName).isNull())
    {
        return false;
    }
    if(field(fieldName).toString() == "")
    {
        return false;
    }
    return true;
}

QStringList ResultsPage::getArguments()
{
    QStringList args;
    args << "-jar";
    args << QString(QDir::currentPath() + "/tools/Test/nbvcxz-1.4.1.jar");

    QStringList passwordList;

    if(wizard()->visitedPages().contains(Page_EnterPassword))
    {
        if(checkFieldByName("PATHLABEL"))
        {
            qDebug() << "Field PATHLABEL" << endl;
            passwordList = AttackPage::passwordListFromFile(field("PATHLABEL").toString());
        }else if(checkFieldByName("PASSWORDLINEEDIT"))
        {
            qDebug() << "Field PASSWORDLINEEDIT " << endl;
            passwordList << field("PASSWORDLINEEDIT").toString();
        }
    }

    args << passwordList;

    return args;

}

// TODO: Kann Weg
void ResultsPage::onTickTimer()
{
    timerClock->stop();
    getArguments();
}

void ResultsPage::onEstimationFinished(const QStringList &output)
{
    qDebug() << "Finished!!!" << endl;
//    ResultTextBrowser->setText("output Length: " + output.length());
    //currentResults = output;
    QStringList parsedOutput = parseOutput(output);
    for(int itk = 0; itk < parsedOutput.length(); itk++)
    {
        ResultTextBrowser->append(parsedOutput.at(itk));
        qDebug() << parsedOutput.at(itk);
    }

    resultsProgressBar->setMaximum(23);
}

// TODO: Make Translatable
QStringList ResultsPage::parseOutput(QStringList output)
{
    bool begin = false;
    currentResults.clear();
    for(int itk = 0; itk < output.length(); itk++)
    {
        QString currentLine = output.at(itk);

        if(currentLine.contains("Benötigte Zeit für die Berechnung:"))
        {
            begin = true;
            continue;
        }

        // Find Start
        if(currentLine.contains("Passwort: ") && begin)
        {
            currentResults << "----------------------------------------------------------";
            currentResults << currentLine;
            begin = false;
            continue;
        }
        // Get Hints

        if(currentLine.contains("Ihr Passwort entspricht"))
        {
            //QStringList values = currentLine.split(":");
            //currentResults << "Online (eingeschränkt): " + values.last();
            currentResults << currentLine;
            continue;
        }

        if(currentLine.contains("Warnung: "))
        {
            QStringList values = currentLine.split(":");
            currentResults << "Warnung:" + values.last();
            continue;
        }

        if(currentLine.contains("Empfehlung: "))
        {
            QStringList values = currentLine.split(":");
            currentResults << "Empfehlung:" + values.last();
            continue;
        }

        // Get time estimate
        if(currentLine.contains("Benötigte Zeit um das Passwort zu knacken:"))
        {
            if(currentLine.contains("ONLINE_THROTTLED: "))
            {
                QStringList values = currentLine.split(":");
                currentResults << "Online (eingeschränkt): " + values.last();
            } else if(currentLine.contains("ONLINE_UNTHROTTLED:"))
            {
                QStringList values = currentLine.split(":");
                currentResults << "Online (uneingeschränkt):" + values.last();
            }else if(currentLine.contains("OFFLINE_BCRYPT_14:"))
            {
                QStringList values = currentLine.split(":");
                currentResults << "Offline (langsam): " + values.last();
            }else if(currentLine.contains("OFFLINE_MD5:"))
            {
                QStringList values = currentLine.split(":");
                currentResults << "Offline (schnell):" + values.last();

            }
            continue;
        }

        // Expert Output??
        if(currentLine.contains("Entspricht Typ:"))
        {
            currentResults << "-----------------------------------";
            QStringList values = currentLine.split(":");
            currentResults << "Art des Treffers:" + values.last();

            itk = itk + 2; // Skip entropie
            currentLine = output.at(itk);
            values = currentLine.split(":");
            currentResults << "Bestandteil des Passworts:" + values.last();

            //currentResults << "-----------------------------------";
        }

    }
    currentResults << "----------------------------------------------------------";
    return currentResults;
}

ResultsPage::~ResultsPage()
{
    workerThread.quit();
    workerThread.wait();
}
