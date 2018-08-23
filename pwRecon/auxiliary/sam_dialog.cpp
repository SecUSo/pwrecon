/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#include "auxiliary/sam_dialog.h"
#include "ui_sam_dialog.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QFile>
#include <QProcess>
#include <QTextStream>

SAMDialog::SAMDialog(QString filepath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAMDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
    //connect(wizard(), &QWizard::back, this, &SAMDialog::reject);
    samdumpfilepath = filepath;

    // TODO: Add Cancle button
    this->setWindowTitle(trUtf8("Passwörter auslesen"));
    ui->infoLabel->setText(trUtf8("Diese Funktion wird die Passwörter dieses Computers auslesen um sie wiederherzustellen.\n\nHINWEIS:\nZum Auslesen der Passwörter dieses Computers werden Administratorrechte benötigt.\nDiese Funktion könnte als Schadsoftware erkannt werden, weil ähnliche Funktionen in Schadsoftware verwendet werden. Antiviren-Programm können nicht zwischen gewollter und ungewollter Anwendung solcher Funktionen unterscheiden. Deswegen ist die Nutzung auf eigene Gefahr."));
    ui->startButton->setText(trUtf8("Weiter"));
}

SAMDialog::~SAMDialog()
{
    delete ui;
}
