#include "WizardPages/extractcurrentpage.h"

ExtractCurrentPage::ExtractCurrentPage(QWidget *parent)
    : QWizardPage(parent)
{
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
    QObject::connect(changePushButton, SIGNAL(clicked()),this, SLOT(change()));
    QObject::connect(extractPushButton, SIGNAL(clicked()),this, SLOT(startExtraction()));

    QHBoxLayout *entryLayout = new QHBoxLayout();
    entryLayout->addWidget(extractPushButton);
    entryLayout->addWidget(extractPathLabel);
    entryLayout->addWidget(changePushButton);

    extractResultTextBrowser = new QTextBrowser();
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(extractResultTextBrowser);

    registerField("EXTRACTPATHLABEL",extractPathLabel,"text", "changeEvent");


    // This is the ceck which will run on that field
    //emailLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp), this));

    // registerField("evaluate.name*", passwordLineEdit);

    QVBoxLayout *extractLayout = new QVBoxLayout;
    extractLayout->addLayout(entryLayout);
    extractLayout->addLayout(displayLayout);

    setLayout(extractLayout);
    valid = false;

    // Set the Texts
    QEvent languageChangeEvent(QEvent::LanguageChange);
    QCoreApplication::sendEvent(this, &languageChangeEvent);
}

int ExtractCurrentPage::nextId() const
{
    return Page_AttackSettings;

}

void ExtractCurrentPage::initializePage()
{
    QString samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    SAMDialog *samDialog = new SAMDialog(samdumpfilepath);
    samDialog->exec();
    disableButtons(false);
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

#ifndef Q_OS_MACX
void ExtractCurrentPage::startExtraction()
{
    // Maybe do the dialog here
    //disableButtons(true);
    extractPushButton->setEnabled(false);
    changePushButton->setEnabled(false);
    //QThread::msleep(5000);
    QProcess pwdumpProcess;
    pwdumpProcess.setProgram(QString(QDir::currentPath() + "/tools/pwdump7/PwDump7.exe"));
    pwdumpProcess.setWorkingDirectory(QString(QDir::currentPath() + "/tools/pwdump7"));

    pwdumpProcess.start();

    while (!pwdumpProcess.waitForFinished()) {
        QThread::msleep(10);
    }

    QByteArray data1 = pwdumpProcess.readAllStandardOutput();
    QTextStream outputStream1(data1);
    QByteArray data2 = pwdumpProcess.readAllStandardError();
    QTextStream outputStream2(data2);

    bool breakup = false;
    QStringList tempfilestring;

    QString line;

    line = outputStream1.readLine();
    qDebug() << endl << "PWDUMP OUTPUT LOG: " << endl;
    while(!line.isNull()) {
        qDebug() << line;

        QStringList splitted = line.split(":");

        if (splitted.size() == 7) {

            QString username(splitted[0]);
            QString hash(splitted[3]);

            if (hash.contains(QString("NO PASSWORD"), Qt::CaseInsensitive)) {
                hash = QString("31D6CFE0D16AE931B73C59D7E0C089C0");
            }

            tempfilestring.append(QString(username + ":" + hash.toLower()));

        } else {
            breakup = true;
        }

        line = outputStream1.readLine();
    }

    line = outputStream2.readLine();
    qDebug() << endl << "PWDUMP ERROR OUTPUT LOG: " << endl;
    while(!line.isNull()) {
        qDebug() << line;
        line = outputStream2.readLine();
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
                extractResultTextBrowser->append(line);
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
#else

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
    qDebug() << "Print the disable bool: " << bol << endl;
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
        extractPushButton->setText(trUtf8("Datenbank extrahieren\n und weiter"));
    } else
        QWidget::changeEvent(event);
}
