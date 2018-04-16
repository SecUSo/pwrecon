#ifndef PWRECONGLOBAL_H
#define PWRECONGLOBAL_H


#include <QWizard>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QDebug>
#include <QTextBrowser>
#include <QProgressBar>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>

#include "auxiliary/sam_dialog.h"

enum {Page_Intro, Page_SelectMode, Page_EnterPassword, Page_EnterHash, Page_ExtractCurrent, Page_AttackSettings, Page_Attack, Page_Results};
enum {Path_Hash, Path_Password, Path_Sam};
//QString dictfile = QString(QDir::currentPath() + "/tools/pwrecon_dict.lst");
//QString potfile = QString(binarydir + "/hashcat.pot");
//QString tempfilepath= QString(QDir::currentPath() + "/tools/tempfile.txt");
//QString testpwdfilepath= QString(QDir::currentPath() + "/tools/testpwdfile.txt");
//QString samdumpfilepath= QString(QDir::currentPath() + "/tools/samdumpfile.txt");
#endif // PWRECONGLOBAL_H
