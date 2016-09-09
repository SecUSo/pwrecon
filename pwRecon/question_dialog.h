/*
 * Copyright (c) 2016 Mustafa Kargi
 * All rights reserved.
 */

#ifndef QUESTION_DIALOG_H
#define QUESTION_DIALOG_H

#include <QDialog>

namespace Ui {
class QuestionDialog;
}

class QuestionDialog : public QDialog
{
    Q_OBJECT

public:

    explicit QuestionDialog(QWidget *parent = 0);
    ~QuestionDialog();

private slots:

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:

    Ui::QuestionDialog *ui;

signals:

    void answerQuestion(bool bol);

};

#endif // QUESTION_DIALOG_H
