#include "testpage.h"
#include "ui_testpage.h"
#include <QWidget>

TestPage::TestPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TestPage)
{
    ui->setupUi(this);
}

TestPage::~TestPage()
{
    delete ui;
}
