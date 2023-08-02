#include "MessRecvContentform.h"
#include "ui_MessRecvContentform.h"

MessRecvContentForm::MessRecvContentForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MessRecvContentForm)
{
    ui->setupUi(this);
}

MessRecvContentForm::~MessRecvContentForm()
{
    delete ui;
}

void MessRecvContentForm::setContent(QString content)
{
    this->ui->mess_label->setText(content);
}
