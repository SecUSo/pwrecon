#include "WizardPages\extractcurrentpage.h"

ExtractCurrentPage::ExtractCurrentPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Extract the password files of the current system"));
    setSubTitle(tr("This function will scan the SAM file of your Windows system and extract all password hashes."));

    QString samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");
    extractPathLabel = new QLabel(samdumpfilepath);
    changePushButton = new QPushButton(tr("Change save file"));
    extractPushButton = new QPushButton(tr("Extract SAM file"));
    extractPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    changePushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //extractLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    // Connenct the label with the text field
    extractPathLabel->setBuddy(extractPushButton);
    extractPathLabel->setWordWrap(true);
    QObject::connect(changePushButton, SIGNAL(clicked()),this, SLOT(change()));
    QObject::connect(extractPushButton, SIGNAL(clicked()),this, SLOT(startExtraction()));

    QHBoxLayout *entryLayout = new QHBoxLayout();
    entryLayout->addWidget(changePushButton);
    entryLayout->addWidget(extractPathLabel);
    entryLayout->addWidget(extractPushButton);

    extractResultTextBrowser = new QTextBrowser();
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(extractResultTextBrowser);



    // This is the ceck which will run on that field
    //emailLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp), this));

    // registerField("evaluate.name*", passwordLineEdit);

    QVBoxLayout *extractLayout = new QVBoxLayout;
    extractLayout->addLayout(entryLayout);
    extractLayout->addLayout(displayLayout);

    setLayout(extractLayout);
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
    qDebug() << "The extract button has just been clicked!" << endl;
}
