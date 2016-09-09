/*
 * Copyright (c) 2016 Mustafa Kargi
 * All rights reserved.
 */

#ifndef SAM_DIALOG_H
#define SAM_DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SAMDialog;
}

class SAMDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SAMDialog(QString filepath, QWidget *parent = 0);
    ~SAMDialog();

private:

    Ui::SAMDialog *ui;

    QString samdumpfilepath;

private slots:

    void on_startButton_clicked();

signals:

    void sendImport();

};

#endif // SAM_DIALOG_H
