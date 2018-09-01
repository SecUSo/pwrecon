#include "WizardPages/evaluationpage.h"
#include <qglobal.h>

EvaluationPage::EvaluationPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("");
    setSubTitle("");

    ResultTextBrowser = new QTextBrowser();
    htmlTextEdit = new QLineEdit();
    htmlTextEdit->setVisible(false);

    registerField("HTMLEVAL",htmlTextEdit);

    resultsProgressBar = new QProgressBar();
    QVBoxLayout *displayLayout = new QVBoxLayout();
    displayLayout->addWidget(resultsProgressBar);
    displayLayout->addWidget(ResultTextBrowser);
    setLayout(displayLayout);



    QString programPath = QString(QDir::currentPath() + "/tools/nbvcxz/");

#ifdef Q_OS_MACX
    programPath = programPath + "MAC_jre1.8.0_172/bin/java";
#endif
#ifdef Q_OS_WIN
    programPath = programPath + "WIN_jre1.8.0_172/bin/java.exe";
#endif

#ifdef Q_OS_LINUX
    programPath = programPath + "LIN_jre1.8.0_181/bin/java";
#endif
    qDebug() << programPath << endl;

    eWorker = new executeWorker(programPath);
    eWorker->moveToThread(&workerThread);
    QObject::connect(&workerThread, &QThread::finished, eWorker, &QObject::deleteLater);
    QObject::connect(this,&EvaluationPage::startEstemation, eWorker, &executeWorker::startWorker);
    //QObject::connect(eWorker, SIGNAL(sendOutput(const QStringList&)), this, SLOT(onEstimationFinished(const QStringList&)));
    QObject::connect(eWorker, &executeWorker::sendOutput, this, &EvaluationPage::onEstimationFinished);
    QObject::connect(eWorker, &executeWorker::onWorkerStarted, this, &EvaluationPage::onEstimationStarted);
    workerThread.start();

}
int EvaluationPage::nextId() const
{
    return Page_Results;
}

void EvaluationPage::initializePage()
{
    ResultTextBrowser->setText(trUtf8("Die Einschätzung wurde gestartet. \n"
                                      "Dieser Vorgang kann einige Minuten dauern."));
    resultsProgressBar->setMaximum(0);
    eWorker->setArguments(getArguments());
    emit EvaluationPage::startEstemation();
}

void EvaluationPage::onEstimationStarted()
{
    disableButtons(true);
}

bool EvaluationPage::checkFieldByName(QString fieldName)
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

QStringList EvaluationPage::getArguments()
{
    QStringList args;
    args << "-jar";
    args << QString(QDir::currentPath() + "/tools/nbvcxz/nbvcxz-1.4.1.jar");

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

void EvaluationPage::onEstimationFinished(const QStringList &output)
{
    qDebug() << "Finished!!!" << endl;
    //    ResultTextBrowser->setText("output Length: " + output.length());
    //currentResults = output;
    QStringList parsedOutput = parseOutput(output);
    ResultTextBrowser->clear();
    for(int itk = 0; itk < parsedOutput.length(); itk++)
    {
        ResultTextBrowser->append(parsedOutput.at(itk));
        qDebug() << parsedOutput.at(itk);
    }
    ResultTextBrowser->verticalScrollBar()->setValue(0);
    disableButtons(false);
    resultsProgressBar->setMaximum(23);
}

QStringList EvaluationPage::parseOutput(QStringList output)
{
    QString htmlResults = "";
    QString password = "";
    QString htmlPassword = "";
    QStringList suggestions;
    QStringList warnings;
    QStringList times;
    QStringList expertOutput;
    int passwordCount = 1;
    QString indent = "        ";
    bool begin = false;
    currentResults.clear();
    output << "---ENDE DER AUSGABE---";
    for(int itk = 0; itk < output.length(); itk++)
    {
        QString currentLine = output.at(itk);

        if(currentLine.contains("Benötigte Zeit für die Berechnung:") || currentLine.contains("---ENDE DER AUSGABE---"))
        {
            begin = true;
            if(suggestions.isEmpty() && warnings.isEmpty() && times.isEmpty())
            {
                continue;
            } else{

                if(!warnings.isEmpty()){
                    currentResults << "<font style=\"color:Black;\"><b><center>" + trUtf8("Warnung:") + "</center></b></font>";
                    currentResults = currentResults + warnings;
                }
                currentResults << "<p style=\"color:Black; text-align:center;\"><b>" + trUtf8("Benötigte Zeit zum Berechnen der Passwörter: ") + "</b></p>";
                currentResults = currentResults + times;
                if(!suggestions.isEmpty()){
                    currentResults << "<b>" + trUtf8("Empfehlung:") + "</b>";
                    currentResults = currentResults + suggestions;
                }
                if(!expertOutput.isEmpty())
                {
                    currentResults = currentResults + expertOutput;
                    expertOutput.clear();
                }

                suggestions.clear();
                warnings.clear();
                times.clear();

                continue;
            }
        }

        // Find Start
        if(currentLine.contains("Passwort: ") && begin)
        {
            currentResults << "----------------------------------------------------------";
            if(field("SHOWHIDEPASSWORD").toBool())
            {
                password = trUtf8("Passwort") + " " + QString::number(passwordCount) + ": *****";
                passwordCount++;
            }else{
                QStringList values = currentLine.split(": ");
                htmlPassword = "<font style=\"color:Black;\"><b>"+ values.last() + "</b></font>";
                password = trUtf8("Passwort:") + " " + htmlPassword;
            }
            currentResults << password;
            begin = false;
            continue;
        }
        // Get Hints

        if(currentLine.contains("Ihr Passwort entspricht"))
        {
            QString tmpString = "";
            const QByteArray& latinName = currentLine.toUtf8();
            const char* c = latinName.data(); // Convert it to char* to make it translatable
            tmpString = trUtf8(c);
            QString tmpHtml = "";

            if(currentLine.contains("Ihr Passwort entspricht nicht"))
            {
                htmlResults += "<font style=\"color:Red;\">"+ password + "  (" + tmpString + ")</font><br>";
                tmpHtml +="<font style=\"color:Red;\"><b>" + tmpString + "</b></font>";
            }else{
                htmlResults += "<font style=\"color:Green;\">"+ password + "  (" + tmpString + ")</font><br>";
                tmpHtml +="<font style=\"color:Green;\"><b>" + tmpString + "</b></font><br>";
            }
            currentResults << " ";
            currentResults << tmpHtml;
            currentResults << " ";
            continue;
        }

        if(currentLine.contains("Warnung: "))
        {
            QStringList values = currentLine.split(": ");
            QString x = values.last();
            const QByteArray& latinName = x.toUtf8();
            const char* c = latinName.data(); // Convert it to char* to make it translatable
            //currentResults << "<font style=\"color:Orange;\"><b>" + trUtf8("Warnung:") + "</b></font>" + " " + trUtf8(c);
            warnings << indent + trUtf8(c);
            continue;
        }

        if(currentLine.contains("Empfehlung: "))
        {
            QStringList values = currentLine.split(": ");
            QString x = values.last();
            const QByteArray& latinName = x.toUtf8();
            const char* c = latinName.data(); // Convert it to char* to make it translatable
            //currentResults << "<i><b>" + trUtf8("Empfehlung:") + "</i></b>" + " " + trUtf8(c);
            suggestions << indent + trUtf8(c);
            continue;
        }

        // Get time estimate
        if(currentLine.contains("Benötigte Zeit um das Passwort zu knacken:"))
        {
            //QString outputLine = trUtf8("Benötigte Zeit um das Passwort zu knacken:");
            QStringList values = currentLine.split(":");
            QString tmpTimeValue = values.last();
            QString timeValue = "";
            if(tmpTimeValue.contains("Unendlich (>100000 Jahrhunderte)"))
            {
                timeValue = trUtf8("Unendlich (>100000 Jahrhunderte)");
            }else if(tmpTimeValue.contains("Sofort")){
                timeValue = trUtf8("Sofort");
            }else
            {
                QStringList tmpList = tmpTimeValue.split(" ");
                if(tmpList.length() == 3)
                {
                    QString x = tmpList[2];
                    //const char *c = x.toLatin1();
                    const QByteArray& latinName = x.toUtf8();
                    const char* c = latinName.data(); // Convert it to char* to make it translatable
                    timeValue = tmpList[1] + " " + trUtf8(c);
                }else{
                    timeValue = tmpTimeValue;
                }
            }
            if(currentLine.contains("ONLINE_THROTTLED: "))
            {
                times << indent + trUtf8("Online (bis zu 2 Versuchen/sec):") + "              " + timeValue;
            } else if(currentLine.contains("ONLINE_UNTHROTTLED:"))
            {
                times << indent + trUtf8("Online (bis zu 100 Versuchen/sec):") + "          " + timeValue;
            }else if(currentLine.contains("OFFLINE_BCRYPT_14:"))
            {
                times << indent + trUtf8("Offline (bis zu 259 Versuchen/sec):") + "         " + timeValue;
            }else if(currentLine.contains("OFFLINE_MD5:"))
            {
                times << indent + trUtf8("Offline (bis zu 250 Mrd. Versuchen/sec):") + " " + timeValue;

            }
            continue;
        }

        // Expert Output??
        if(field("EXPERTMODE").toBool()){
            if(currentLine.contains("Entspricht Typ:"))
            {
                expertOutput << "-----------------------------------";
                QStringList values = currentLine.split(":");
                expertOutput << trUtf8("Art des Treffers:") + values.last();

                // Don't display information about parts of the password if it shall be hidden.
                if(!field("SHOWHIDEPASSWORD").toBool())
                {
                    itk = itk + 2; // Skip entropie
                    currentLine = output.at(itk);
                    values = currentLine.split(":");
                    expertOutput << trUtf8("Bestandteil des Passworts:") + values.last();

                    if(output.at(itk + 4).contains("Wörterbuch:"))
                    {
                        itk = itk + 4; // Skip entropie
                        currentLine = output.at(itk);
                        values = currentLine.split(":");
                        expertOutput << trUtf8("Wörterbuch:") + values.last();
                        itk++; // Skip entropie
                        currentLine = output.at(itk);
                        values = currentLine.split(":");
                        expertOutput << trUtf8("Wörterbuch Eintrag:") + values.last();
                    }
                }

                //currentResults << "-----------------------------------";
            }
        }

    }

    currentResults << "----------------------------------------------------------";
    htmlTextEdit->setText(htmlResults);
    return currentResults;
}

void EvaluationPage::disableButtons(bool bol)
{
    qDebug() << "Print the disable bool: " << bol << endl;
    wizard()->button(QWizard::NextButton)->setDisabled(bol);
    wizard()->button(QWizard::BackButton)->setDisabled(bol);
    //wizard()->button(QWizard::CustomButton1)->setDisabled(bol);
}

EvaluationPage::~EvaluationPage()
{
    workerThread.quit();
    workerThread.wait();
}

void EvaluationPage::possibleOutputs()
{
    //# Main text translations
    trUtf8("Benötigte Zeit für die Berechnung:");
    trUtf8("Passwort:");
    trUtf8("Ihr Passwort entspricht den Minimalanforderungen.");
    trUtf8("Ihr Passwort entspricht nicht den Minimalanforderungen.");

    //# Calculation Time
    trUtf8("Unendlich (>100000 Jahrhunderte)");
    trUtf8("Jahrhunderte");
    trUtf8("Jahre");
    trUtf8("Monate");
    trUtf8("Tage");
    trUtf8("Stunden");
    trUtf8("Minuten");
    trUtf8("Sekunden");
    trUtf8("Sofort");

    //# Feedback
    //## Default
    trUtf8("Gib einige Wörter ein. Vermeide Übliche Worte und bekannte Phrasen.");
    trUtf8("Die Eingabe von Symbolen, Zahlen oder Grossbuchstaben ist nicht nötig.");
    //## Extra
    trUtf8("F\u00FCge ein oder zwei zusätzliche Worte hinzu. Unübliche Worte sind besser.");
    //## Dictionary
    trUtf8("Dieses Passwort kann nicht verwendet werden.");
    trUtf8("Dieses Passwort ist in den Top 10 der meist verwendeten Passwörter.");
    trUtf8("Dieses Passwort ist in den Top 100 der meist verwendeten Passwörter.");
    trUtf8("Dieses Passwort wird sehr oft verwendet.");
    trUtf8("Grossbuchstaben am Anfang sind nicht sehr sicher.");
    trUtf8("Ein Passwort in Grossschrift ist fast so einfach zu erraten, wie ein Passwort in Kleinschrift.");
    trUtf8("Invertierte Worte sind nicht viel schwerer zu erraten.");
    trUtf8("Ersetzungen wie zum Beispiel '@' anstelle von 'a' sind einfach zu erraten.");
    trUtf8("Bitte verwende ein Passwort, welches nicht auf der Ausschlussliste ist. Füge zusätzliche Worte oder Zahlen hinzu.");
    //## Spatial
    trUtf8("Aufeinanderfolgende Zeichen auf der Tastatur sind einfach zu erraten.");
    trUtf8("Kurze Eingabemuster sind einfach zu erraten.");
    trUtf8("Verwende ein Eingabemuster das Länger ist und mehr Umdrehungen beinhaltet.");
    //## Repeat
    trUtf8("Wiederholungen wie zum Beispiel \"aaa\" sind einfach zu erraten.");
    trUtf8("Wiederholungen wie zum Beispiel \"abcabcabc\" sind nur leicht schwerer zu erraten als \"abc\".");
    trUtf8("Vermeide die Wiederholung von Worten und Zeichen.");
    //## Sequence
    trUtf8("Reihenfolgen wie zum Beispiel \"abc\" oder \"6543\" sind einfach zu erraten.");
    trUtf8("Vermeide Reihenfolgen.");
    //## Year
    trUtf8("Jahreszahlen die nicht weit zur\u00FCckliegen sind einfach zu erraten.");
    trUtf8("Vermeide eine Jahreszahl, die nicht weit zurückliegt oder mit ihrer Person assoziiert werden könnte.");
    //## Date
    trUtf8("Ein Datum ist einfach zu erraten.");
    trUtf8("Vermeide ein Datum oder Jahreszahlen das mit ihrer Person assoziiert werden könnte.");

    // Maybe needed for the expert output
    /*
    trUtf8("Entropie:");
    trUtf8("Ben\u00F6tigte Zeit um das Passwort zu knacken:");
    trUtf8("Entspricht Typ:");
    trUtf8("Wortteil:");
    trUtf8("Start-Index:");
    trUtf8("End-Index:");
    trUtf8("L\u00E4nge:");
    trUtf8("W\u00F6rterbuch:");
    trUtf8("W\u00F6rterbuch Eintrag:");
    trUtf8("Rang:");
    trUtf8("Leet Zeichenersetzung:");
    trUtf8("Invertiert:");
    trUtf8("Distanz:");
    trUtf8("Wiederholte Zeichen:");
    trUtf8("Wiederholungen:");
    trUtf8("Erstes Zeichen:");
    trUtf8("Tastatur Typ:");
    trUtf8("Drehung:");
    trUtf8("Verschiebung:");
    trUtf8("Jahr:");
    trUtf8("Monat:");
    trUtf8("Tag:");
    trUtf8("Trennzeichen:");
    */
}

void EvaluationPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Die Sicherheit von Passwörtern einschätzen."));
        setSubTitle(trUtf8("Die angegebenen Passwörter werden untersucht und eine Einschätzung ihrer Sicherheit wird erstellt."));

    } else
        QWidget::changeEvent(event);
}
