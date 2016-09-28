/*
 * Copyright (c) 2016 Mustafa Kargi <mustafa.kargi@arcor.de>
 *
 * See LICENSE dist-file for details.
 */

#ifndef TYPE_PASSWORD_DIALOG_H
#define TYPE_PASSWORD_DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class TypePasswordDialog;
}

class TypePasswordDialog : public QDialog
{
    Q_OBJECT

public:

    explicit TypePasswordDialog(bool plainpw, QString filepath, QWidget *parent = 0);
    ~TypePasswordDialog();

private:

    Ui::TypePasswordDialog *ui;

    bool plain;
    QString testpwdfilepath;

private slots:

    void on_buttonBox_accepted();

signals:

    void setPassword(const QString& pwd);

};

#endif // TYPE_PASSWORD_DIALOG_H
