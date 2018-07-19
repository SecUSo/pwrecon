#include "WizardPages/extractcurrentpage.h"

ExtractCurrentPage::ExtractCurrentPage(QWidget *parent)
    : QWizardPage(parent)
{
    // TODO: Disable when on mac
    // TODO: Implement Linux variant
    // TODO: If expert dont do next directly
    // TODO: Dont display Hashes in layman mode but write some information (Extraction started ...)
    setTitle("");
    setSubTitle("");

    samdumpfilepath = QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    extractPathLabel = new QLabel(samdumpfilepath);
    changePushButton = new QPushButton();
    extractPushButton = new QPushButton();
    extractPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    changePushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //extractLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    // Connenct the label with the text field
    extractPathLabel->setBuddy(extractPushButton);
    extractPathLabel->setWordWrap(true);

    QHBoxLayout *entryLayout = new QHBoxLayout();
    entryLayout->addWidget(extractPushButton);
    entryLayout->addWidget(extractPathLabel);
    entryLayout->addWidget(changePushButton);

    extractResultTextBrowser = new QTextBrowser();
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(extractResultTextBrowser);

    extractProgressBar = new QProgressBar;
    entryLayout->addWidget(extractProgressBar);
    extractProgressBar->setVisible(true);
    registerField("EXTRACTPATHLABEL",extractPathLabel,"text", "changeEvent");


    // This is the ceck which will run on that field
    //emailLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp), this));

    // registerField("evaluate.name*", passwordLineEdit);

    QVBoxLayout *extractLayout = new QVBoxLayout;
    extractLayout->addLayout(entryLayout);
    extractLayout->addWidget(extractProgressBar);
    extractLayout->addLayout(displayLayout);

    setLayout(extractLayout);
    valid = false;


    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);

    QObject::connect(changePushButton, SIGNAL(clicked()),this, SLOT(change()));
    QObject::connect(extractPushButton, SIGNAL(clicked()),this, SLOT(startExtraction()));
#ifdef Q_OS_WIN
    QString programPath = QString(QDir::currentPath() + "/tools/pwdump7/PwDump7.exe");
    qDebug() << programPath << endl;

    eWorker = new executeWorker(programPath);
    eWorker->moveToThread(&workerThread);
    QObject::connect(&workerThread, &QThread::finished, eWorker, &QObject::deleteLater);
    QObject::connect(this,&ExtractCurrentPage::onStartExtraction, eWorker, &executeWorker::startWorker);
    QObject::connect(eWorker, &executeWorker::sendOutput, this, &ExtractCurrentPage::onExtractionFinished);
    QObject::connect(eWorker, &executeWorker::onWorkerStarted, this, &ExtractCurrentPage::workerStarted);
    QObject::connect(eWorker, &executeWorker::sendErrorOutput, this, &ExtractCurrentPage::getError);
    workerThread.start();
#endif
}

int ExtractCurrentPage::nextId() const
{
    if(field("EXPERTMODE").toBool()){
        return Page_AttackSettings;
    } else{
        return Page_Attack;
    }

}

void ExtractCurrentPage::initializePage()
{
    QString samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    SAMDialog *samDialog = new SAMDialog(samdumpfilepath);
    samDialog->exec();

    if(field("EXPERTMODE").toBool()){
        extractResultTextBrowser->setText(trUtf8("Die Passwörter des lokalen Systems können ausgelesen werden.\n"
                                                 "Dieser Vorgang kann einige Minuten dauern.\n"
                                                 "Sie können den Speicherort der extrahierten Daten ändern."));
        disableButtons(false);
    }else{
        extractResultTextBrowser->setText(trUtf8("Die Passwörter des lokalen Systems werden ausgelesen.\n"
                                                 "Dieser Vorgang kann einige Minuten dauern."));
        emit ExtractCurrentPage::onStartExtraction();
    }
}

void ExtractCurrentPage::change()
{
    QString tmpFile = "";
    tmpFile = QFileDialog::getOpenFileName(this,
                                           tr("Choose a file to store the results"), "", "*");
    //tr("Image Files (*.png *.jpg *.bmp)")
    // if equal return 0
    if(QString::compare(tmpFile, "", Qt::CaseInsensitive))
    {
        currentFile = tmpFile;
    }else{
        //ignore
    }
    qInfo(qPrintable(currentFile));
    extractPathLabel->setText(currentFile);
}

#ifdef Q_OS_WIN
void ExtractCurrentPage::workerStarted()
{
    extractProgressBar->setMaximum(0);
    disableButtons(true);
}

void ExtractCurrentPage::startExtraction()
{
    emit ExtractCurrentPage::onStartExtraction();
}

void ExtractCurrentPage::onExtractionFinished(const QStringList &output)
{
    qDebug() << "Finished!!!" << endl;

    if(!errorOutput.empty())
        foreach (const QString &line, errorOutput) {
            if(line.contains("Error",Qt::CaseInsensitive))
            {
                printError();
                return;
            }
        }

    QStringList parsedOutput = parseOutput(output);

    qDebug() << endl << "PWDUMP SUCCESS" << endl;

    QFile tempfile(samdumpfilepath);
    if (tempfile.exists())
        tempfile.remove();
    if (tempfile.open(QIODevice::ReadWrite)) {
        QTextStream stream(&tempfile);
        foreach (const QString &line, parsedOutput) {
            stream << line << endl;
            extractResultTextBrowser->append(line);
        }
    }
    tempfile.close();
    valid = true;


    disableButtons(false);

    extractProgressBar->setMaximum(23);

    // Experts want to see the results
    if(!field("EXPERTMODE").toBool()){
        wizard()->next();
    }
}

QStringList ExtractCurrentPage::parseOutput(QStringList output)
{
    currentResults.clear();

    currentResults << "----------------------------------------------------------";

    qDebug() << endl << "PWDUMP OUTPUT LOG: " << endl;
    QString line;
    for(int itk = 0; itk < output.length(); itk++) {
        line = output.at(itk);
        qDebug() << line;

        QStringList splitted = line.split(":");

        if (splitted.size() == 7) {

            QString username(splitted[0]);
            QString hash(splitted[3]);

            if (hash.contains(QString("NO PASSWORD"), Qt::CaseInsensitive)) {
                hash = QString("31D6CFE0D16AE931B73C59D7E0C089C0");
            }

            currentResults << QString(username + ":" + hash.toLower());

        } else {
            // TODO: Check if it is needed!
            breakup = true;
        }

    }

    currentResults << "----------------------------------------------------------";
    return currentResults;
}

void ExtractCurrentPage::getError(const QStringList& errorOutput)
{
    this->errorOutput = errorOutput;
}

void ExtractCurrentPage::printError()
{
    if (breakup) {
        qDebug() << endl << "PWDUMP NO SUCCESS" << endl;

        extractResultTextBrowser->setText(trUtf8("Fehler:\nVorgang nicht möglich.\nStellen Sie sicher, dass Sie Administrator Rechte besitzen."));
        disableButtons(true);
        extractProgressBar->setMaximum(23);
        wizard()->button(QWizard::BackButton)->setDisabled(false);
        valid = false;

        for(int itk = 0; itk < errorOutput.length(); itk++)
        {
            qDebug() << errorOutput.at(itk) << endl;
           // extractResultTextBrowser->append(errorOutput.at(itk));
        }
    }
}

ExtractCurrentPage::~ExtractCurrentPage()
{
    workerThread.quit();
    workerThread.wait();
}

#elif Q_OS_MACX

void ExtractCurrentPage::startExtraction()
{
    extractPushButton->setEnabled(false);
    changePushButton->setEnabled(false);
    //extractResultTextBrowser->setText("Test");
    QProcess listing;
    // List all directories to enumerate the users
    listing.start("ls", QStringList() << "/Users/");

    while (!listing.waitForFinished()) {
        QThread::msleep(10);
    }
    bool breakup = false;
    QByteArray data1 = listing.readAllStandardOutput();
    QTextStream outputStream1(data1);
    QByteArray data2 = listing.readAllStandardError();
    QTextStream outputStream2(data2);

    QString listingLines = outputStream1.readAll();

    //extractResultTextBrowser->append(listingLine);
    qDebug() << listingLines << endl;

    QStringList users = listingLines.split("\n");
    QStringList tempfilestring;
    QString toCrack;

    for(int i = 0; i<users.length()-1; i++)
    {
        if(users.at(i) != "Shared" && users.at(i) != ".localized")
        {
            // defaults read /var/db/dslocal/nodes/Default/users/Lilian.plist ShadowHashData|tr -dc 0-9a-f|xxd -r -p|plutil -convert xml1 - -o -
            QProcess defaultProc;

            // Get the password file of the specific users
            defaultProc.start("sh", QStringList() << "-c" << "defaults read /var/db/dslocal/nodes/Default/users/" + users.at(i) + ".plist ShadowHashData|tr -dc 0-9a-f|xxd -r -p|plutil -convert xml1 - -o -");
            while (!defaultProc.waitForFinished()) {
                QThread::msleep(10);
            }

            // get the command outputs
            QByteArray defaultProcData1 = defaultProc.readAllStandardOutput();
            QTextStream defaultOutputStream1(defaultProcData1);
            QByteArray defaultProcDataERROR = defaultProc.readAllStandardError();
            QTextStream defaultOutputStreamERROR(defaultProcDataERROR);

            QString defaultLines = defaultOutputStream1.readAll();

            QString defaultLinesERROR = defaultOutputStreamERROR.readAll();


            // The process returns xml data which we need to parse
            /*
            qDebug() << users.at(i) << endl;
            qDebug() << "###################################################################" << endl;
            qDebug() << defaultLines << endl;
            extractResultTextBrowser->append(defaultLines);
            qDebug() << "ENDE#############ENDE#######################ENDE###############################ENDE" << endl;
            */
            QByteArray xmlText = defaultLines.toUtf8();
            QDomDocument doc;
            doc.setContent(xmlText);

            // Get the first key which tells us which crypto is used
            QDomNodeList xmlDoc = doc.elementsByTagName("dict");
            QDomNode tmp1 = xmlDoc.item(0);
            QDomElement type = tmp1.firstChildElement("key"); // The type ( only support SaltedSHA512PBKDF2)
            qDebug() << "COUNT (8): " <<  xmlDoc.count() << type.text() << endl;
            if(type.text() != "SALTED-SHA512-PBKDF2")
            {
                QString msg = "INFO: User uses not supported Crypto (" + type.text() + ")";
                qDebug() << users.at(i) << ":" << endl << msg << endl;
                extractResultTextBrowser->append(users.at(i) + ":");
                extractResultTextBrowser->append(msg);
                continue;
            }

            // If it is SaltedSHA512PBKDF2 we can go on with parsing the xml
            QDomNode n = xmlDoc.item(1);
            QDomElement tmp = n.toElement();
            QDomNodeList x = tmp.childNodes();

            qDebug() << x.count() << endl;
            QString tmpEntropy;
            QString entropy;
            QString iterations;
            QString tmpSalt;
            QString salt;


            // We need exact 6 elements (entropy + data, iterations + data, salt + data)
            if(x.count() == 6)
            {
                // Get the data from the elements as text
                // Clean the text (smimplified -> remove stuff like \t etc.) and replace the created spaces with nothing.
                tmpEntropy = x.item(1).toElement().text();
                tmpEntropy = tmpEntropy.simplified().replace(" ", "");
                iterations = x.item(3).toElement().text();
                tmpSalt = x.item(5).toElement().text();
                tmpSalt = tmpSalt.simplified().replace(" ", "");

                // Decode the base64 string to HEX strings which ca be read from Hashcat
                entropy = QString(QByteArray::fromBase64(tmpEntropy.toUtf8()).toHex());
                salt = QString(QByteArray::fromBase64(tmpSalt.toUtf8()).toHex());

                // $ml$<iterations>$<salt>$<entropy>
                toCrack  = "$ml$" + iterations + "$" + salt + "$" + entropy;
                tempfilestring.append(toCrack);
                qDebug() << toCrack << endl;

                extractResultTextBrowser->append(users.at(i) + ":");
                extractResultTextBrowser->append(toCrack);

            }else{
                continue;
            }

            //            for(int j = 0; j<x.count(); j++)
            //            {
            //                QDomNode tmpKey = x.item(j);
            //                j++;
            //                QDomNode tmpData = x.item(j);
            //                qDebug() << "KEY: " <<  tmpKey.toElement().text()  << " DATA: " << tmpData.toElement().text() << endl;
            //            }

        }


    }

    if(tempfilestring.length() == 0)
    {
        breakup = true;
    }


    if (breakup) {
        qDebug() << endl << "PWDUMP NO SUCCESS" << endl;

        QString en("Error:\nProcess not possible.");
        QString de("Fehler:\nVorgang nicht möglich.");
        extractResultTextBrowser->setText(de);
        // disableButtons(false);
        valid = false;
    }
    else {
        qDebug() << endl << "PWDUMP SUCCESS" << endl;

        QFile tempfile(samdumpfilepath);
        if (tempfile.exists())
            tempfile.remove();
        if (tempfile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&tempfile);
            foreach (const QString &line, tempfilestring) {
                stream << line << endl;
                //extractResultTextBrowser->append(line);
            }
        }
        tempfile.close();
        valid = true;
    }

    if(valid == false)
    {
        wizard()->button(QWizard::NextButton)->setDisabled(true);
    } else {
        disableButtons(false);
        wizard()->next();
    }
}
#endif

void ExtractCurrentPage::disableButtons(bool bol)
{
    wizard()->button(QWizard::BackButton)->setDisabled(bol);
    wizard()->button(QWizard::NextButton)->setDisabled(bol);
    this->changePushButton->setDisabled(bol);
    this->extractPushButton->setDisabled(bol);
}

bool ExtractCurrentPage::validatePage()
{
    if(valid == false)
    {
        QMessageBox::warning(this, trUtf8("pwRecon"),
                             trUtf8("Sie können ohne Passwörter nicht vortfahren."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
    return valid;
}

void ExtractCurrentPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setTitle(trUtf8("Passwörter von diesem Computer wiederherstellen"));
        setSubTitle(trUtf8("Die Passwort Datenbank dieses Computers wird extrahiert um sie zu testen."));
        changePushButton->setText(trUtf8("Speicherort ändern"));
        if(field("EXPERTMODE").toBool()){
            extractPushButton->setText(trUtf8("Datenbank extrahieren"));
        }else{
            extractPushButton->setText(trUtf8("Datenbank extrahieren\n und weiter"));
        }
        // Expert Mode CHanges
        changePushButton->setVisible(field("EXPERTMODE").toBool());
        extractPathLabel->setVisible(field("EXPERTMODE").toBool());
    } else
        QWidget::changeEvent(event);
}

