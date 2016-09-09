/*
 * Copyright (c) 2016 Mustafa Kargi
 * All rights reserved.
 */

#include "question_dialog.h"
#include "ui_question_dialog.h"

QuestionDialog::QuestionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
}

QuestionDialog::~QuestionDialog()
{
    delete ui;
}

void QuestionDialog::on_buttonBox_accepted()
{
    emit answerQuestion(true);
}

void QuestionDialog::on_buttonBox_rejected()
{
    emit answerQuestion(false);
}
