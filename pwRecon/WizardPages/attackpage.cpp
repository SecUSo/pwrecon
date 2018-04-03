#include "WizardPages\attackpage.h"

AttackPage::AttackPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select the attack mode for this test."));
    setSubTitle(tr("Please chose between Dictionary Arrack and Brute Force attack"));

    startPushButton = new QPushButton(tr("Start"));
    stopPushButton = new QPushButton(tr("Stop"));
    startPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    stopPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    attackProgressBar = new QProgressBar();
    QObject::connect(startPushButton, SIGNAL(clicked()),this, SLOT(start()));
    QObject::connect(stopPushButton, SIGNAL(clicked()),this, SLOT(stop()));

    QHBoxLayout *startStopLayout = new QHBoxLayout();
    startStopLayout->addWidget(startPushButton);
    startStopLayout->addWidget(attackProgressBar);
    startStopLayout->addWidget(stopPushButton);

    attackResultTextBrowser = new QTextBrowser();
    QHBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(attackResultTextBrowser);



    // This is the ceck which will run on that field
    //emailLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp), this));

    // registerField("evaluate.name*", passwordLineEdit);

    QVBoxLayout *extractLayout = new QVBoxLayout;
    extractLayout->addLayout(startStopLayout);
    extractLayout->addLayout(displayLayout);

    setLayout(extractLayout);
}

int AttackPage::nextId() const
{
    return Page_Results;
}

void AttackPage::start()
{
    qDebug() << "Start button has been pressed!" << endl;
}

void AttackPage::stop()
{
    qDebug() << "Stop button has been pressed!" << endl;
}
