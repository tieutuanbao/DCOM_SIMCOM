#include "MessShort.h"
#include "ui_messshort.h"
#include <QDebug>

MessShort::MessShort(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MessShort)
{
    ui->setupUi(this);
}

MessShort::~MessShort()
{
    delete ui;
}

void MessShort::setPhoneNum(QString phoneNum)
{
    this->ui->phoneNum_lbl->setText(phoneNum);
}

QString MessShort::getPhoneNum()
{
    return this->ui->phoneNum_lbl->text();
}

void MessShort::setDateTime(QString dateTime)
{
    this->ui->dateTime_lbl->setText(dateTime);
}

QString MessShort::getDateTime()
{
    return this->ui->dateTime_lbl->text();
}

void MessShort::setContent(QString content)
{
    this->ui->content_lbl->setText(content);
}

QString MessShort::getContent()
{
    return this->ui->content_lbl->text();
}

void MessShort::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit mousePress_signal(this->ui->phoneNum_lbl->text());
}
