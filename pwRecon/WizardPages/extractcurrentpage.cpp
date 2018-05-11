#include "WizardPages\extractcurrentpage.h"

ExtractCurrentPage::ExtractCurrentPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(trUtf8("Passwörter von diesem Computer wiederherstellen"));
    setSubTitle(trUtf8("Die Passwort Datenbank dieses Computers wird extrahiert um sie zu testen."));

    samdumpfilepath = QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    extractPathLabel = new QLabel(samdumpfilepath);
    changePushButton = new QPushButton(trUtf8("Speicherort ändern"));
    extractPushButton = new QPushButton(trUtf8("Datenbank extrahieren\n und weiter"));
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
