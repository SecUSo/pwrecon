#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWizardPage>
#include <QWidget>

namespace Ui {
class TestPage;
}

class TestPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();

private:
    Ui::TestPage *ui;
};

#endif // TESTPAGE_H
