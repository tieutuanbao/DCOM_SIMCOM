#include "MessSentContentForm.h"
#include "ui_MessSentContentForm.h"

MessSentContentForm::MessSentContentForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MessSentContentForm)
{
    ui->setupUi(this);
}

MessSentContentForm::~MessSentContentForm()
{
    delete ui;
}

void MessSentContentForm::setContent(QString content)
{
    this->ui->mess_label->setText(content);
}
