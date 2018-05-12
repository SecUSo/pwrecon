#include "WizardPages/loadhashespage.h"

LoadHashesPage::LoadHashesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Chose a hash list file"));
    setSubTitle(tr("Please chose the hash list file you would like to test."));

    pathLabel = new QLabel();
    pathLabel->setWordWrap(true);
    loadPushButton = new QPushButton(tr("Select Hash File"));
    // To prevent the button to get the wrong size
    loadPushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pathLabel->setBuddy(loadPushButton);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(loadPushButton, 0, 0);
    layout->addWidget(pathLabel, 0, 1);
    setLayout(layout);

     QObject::connect(loadPushButton, SIGNAL(clicked()),this, SLOT(load()));
}
int LoadHashesPage::nextId() const
{
    return Page_Attack;
}

void LoadHashesPage::load()
{
    currentFile = QFileDialog::getOpenFileName(this,
    tr("Select a file containing the hashes"), "", "*");
    //tr("Image Files (*.png *.jpg *.bmp)")
    qInfo(qPrintable(currentFile));
    pathLabel->setText(currentFile);
}
