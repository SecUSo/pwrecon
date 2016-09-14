/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#include "type_password_dialog.h"
#include "ui_type_password_dialog.h"

#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>

TypePasswordDialog::TypePasswordDialog(QString filepath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TypePasswordDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);

    testpwdfilepath = filepath;
    ui->lineEdit->setFocus();
}

TypePasswordDialog::~TypePasswordDialog()
{
    delete ui;
}

void TypePasswordDialog::on_buttonBox_accepted()
{
    QString str = ui->lineEdit->text();

    QByteArray hashstr(QCryptographicHash::hash(QByteArray(str.toUtf8()), QCryptographicHash::Md5).toHex());

    QFile file(testpwdfilepath);
    if (file.exists())
        file.remove();

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "pwRecon:" << hashstr << endl;
    }
    file.close();


    emit setPassword(str);
}
