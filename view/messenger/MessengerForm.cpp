#include "MessengerForm.h"
#include "ui_MessengerForm.h"
#include <MessShort.h>
#include "MessRecvContentform.h"
#include "MessSentContentForm.h"
#include <QAbstractTextDocumentLayout>

#include <QDebug>

MessengerForm::MessengerForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MessengerForm)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->connect(this->ui->textEdit->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged, this, [=](const QSizeF&){
        QSize size = this->ui->textEdit->document()->size().toSize();
        if(size.height() < messTyping_maxHeight) {
            this->ui->textEdit->setFixedHeight(size.height());
        }
    });
}

MessengerForm::~MessengerForm()
{
    delete ui;
}

void MessengerForm::updateShortMess(QString contact, QString content, QString dateTime)
{
    MessShort *messShort;
    /* 1 --- Tìm ShortMess đã có sẵn theo contact --- */
    for (uint16_t indexShortMess = 0; indexShortMess < this->ui->messShort_containner->layout()->count(); indexShortMess++) {
        QWidget *widget = this->ui->messShort_containner->layout()->itemAt(indexShortMess)->widget();
        if(widget == nullptr) {
            break;
        }
        if(widget == this->ui->messShort_spacer->widget()) {
            break;
        }
        messShort = dynamic_cast<MessShort *>(widget);
        if(messShort->getPhoneNum() == contact) {
            messShort->setContent(content);
            messShort->setDateTime(dateTime);
            return;
        }
    }
    /* 2 --- Thêm một ShortMess mới --- */
    messShort = new MessShort();
    messShort->setContent(content);
    messShort->setDateTime(dateTime);
    messShort->setPhoneNum(contact);
    /* Lấy ra spacer */
    int indexSpacer = this->ui->messShort_containner->layout()->indexOf(this->ui->messShort_spacer);
    this->ui->messShort_containner->layout()->takeAt(indexSpacer);
    /* Thêm MessShort */
    this->ui->messShort_containner->layout()->addWidget(messShort);
    this->ui->messShort_containner->layout()->addItem(this->ui->messShort_containner->layout()->takeAt(0));
    /* Thêm lại Spacer */
    this->ui->messShort_containner->layout()->addItem(this->ui->messShort_spacer);
    /* Set signal chọn ShortMess */
    this->connect(messShort, &MessShort::mousePress_signal, [=](QString contact) {
        emit this->showAllMessAtContact(contact);
    });
}

/**
 * @brief Xóa tất cả các tin nhắn tóm tắt
 */
void MessengerForm::removeAllShortMess()
{
    QLayoutItem* item;
    while ((item = this->ui->messShort_containner->layout()->takeAt(0)) != this->ui->messShort_spacer) {
        this->ui->messShort_containner->layout()->removeItem(item);
        delete item->widget();
    }
    this->ui->messShort_containner->layout()->addItem(item);
}
/**
 * @brief Xóa tất cả tin nhắn chi tiết đang hiển thị
 */
void MessengerForm::removeMessView()
{
    QLayoutItem* item;
    while ((item = this->ui->messContainer_scrollAreaContent->layout()->takeAt(0)) != this->ui->pushMess_spacer) {
        this->ui->messContainer_scrollAreaContent->layout()->removeItem(item);
        delete item->widget();
    }
    this->ui->messContainer_scrollAreaContent->layout()->addItem(item);
}
/**
 * @brief Thêm tin nhắn vào hiển thị
 * @param dir gửi/nhận
 * @param content nội dung
 */
void MessengerForm::addMessToView(MessDir dir, QString content)
{
    if(dir == MessengerForm::RECV) {
        /* Tạo messRecv trong MessContainer */
        MessRecvContentForm *messRecv = new MessRecvContentForm(this->ui->messContainer_scrollAreaContent);
        messRecv->setContent(content);
        /* Lấy Spacer ra */
        this->ui->messContainer_scrollAreaContent->layout()->removeItem(this->ui->pushMess_spacer); // Lấy ra Spacer
        /* Thêm widget vào Layout */
        this->ui->messContainer_scrollAreaContent->layout()->addWidget(messRecv);
        /* Thêm Spacer vào lại Layout */
        this->ui->messContainer_scrollAreaContent->layout()->addItem(this->ui->pushMess_spacer);
    }
    else {
        /* Tạo messSent trong MessContainer */
        MessSentContentForm *messSent = new MessSentContentForm(this->ui->messContainer_scrollAreaContent);
        messSent->setContent(content);
        /* Lấy Spacer ra */
        this->ui->messContainer_scrollAreaContent->layout()->removeItem(this->ui->pushMess_spacer); // Lấy ra Spacer
        /* Thêm widget vào Layout */
        this->ui->messContainer_scrollAreaContent->layout()->addWidget(messSent);
        /* Thêm Spacer vào lại Layout */
        this->ui->messContainer_scrollAreaContent->layout()->addItem(this->ui->pushMess_spacer);
    }
}

void MessengerForm::on_textEdit_textChanged()
{

}
