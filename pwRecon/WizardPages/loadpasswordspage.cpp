#include "WizardPages\loadpasswordspage.h"

LoadPasswordsPage::LoadPasswordsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Chose a password list file"));
    setSubTitle(tr("Please chose the password list file you would like to test."));

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton(tr("Select Password File"));
    // To prevent the button to get the wrong size
    loadPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pathLabel->setBuddy(loadPushButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(loadPushButton, 0, 0);
    layout->addWidget(pathLabel, 0, 1);
    setLayout(layout);

     QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));
}
int LoadPasswordsPage::nextId() const
{
    return Page_Attack;
}

void LoadPasswordsPage::load()
{
    currentFile = QFileDialog::getOpenFileName(this,
    tr("Open Image"), "", "*");
    //tr("Image Files (*.png *.jpg *.bmp)")
    qInfo(qPrintable(currentFile));
    pathLabel->setText(currentFile);
}
