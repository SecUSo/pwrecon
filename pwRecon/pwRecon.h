/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef LICENSEWIZARD_H
#define LICENSEWIZARD_H

#include <QWizard>
#include <QFileSystemWatcher>

#include "pwrecon_global.h"
#include "WizardPages/attackpage.h"
#include "WizardPages/enterhashpage.h"
#include "WizardPages/enterpasswordpage.h"
#include "WizardPages/extractcurrentpage.h"
#include "WizardPages/resultspage.h"
#include "WizardPages/selectmodepage.h"
#include "WizardPages/selectrecoverymodepage.h"
#include "WizardPages/selecttestmodepage.h"
#include "WizardPages/attacksettingspage.h"
#include "WizardPages/evaluationpage.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;

class pwRecon : public QWizard
{
    Q_OBJECT

public:
    pwRecon(QWidget *parent = 0);
    QTranslator *m_translatorQt;
    QTranslator *translator2;
    void changeEvent(QEvent *event) override;

private slots:
    void showHelp();
};


class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);
    int count = 0;

    int nextId() const override;
    void changeEvent(QEvent *event) override;
    void setLanguagesComboBox();

private:
    QLabel *topLabel;
    QComboBox *languageComboBox;
    QTranslator *pwReconTranslator;
    QCheckBox *expertModeCheckBox;
    QCheckBox *showHidePasswordCheckBox;

public slots:
    void setLanguage();
    void setExpertModeTexts();
};
#endif
