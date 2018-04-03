#ifndef LICENSEWIZARD_H
#define LICENSEWIZARD_H

#include <QWizard>

#include "pwrecon_global.h"
#include "testpage.h"
#include "WizardPages/attackpage.h"
#include "WizardPages/enterhashpage.h"
#include "WizardPages/enterpasswordpage.h"
#include "WizardPages/extractcurrentpage.h"
#include "WizardPages/loadhashespage.h"
#include "WizardPages/loadpasswordspage.h"
#include "WizardPages/resultspage.h"
#include "WizardPages/selectmodepage.h"
#include "WizardPages/attacksettingspage.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;

class pwRecon : public QWizard
{
    Q_OBJECT

public:
    pwRecon(QWidget *parent = 0);

private slots:
    void showHelp();
};



class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

    int nextId() const override;

private:
    QLabel *topLabel;
};
#endif
