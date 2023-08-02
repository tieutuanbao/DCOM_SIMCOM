#include "mainwindow.h"

#include <QApplication>
#include <QList>
#include <QSerialPortInfo>
#include <QMetaObject>
#include <QThread>

#include "SerialThread.h"
#include "SimCardManager.h"

#include <MessengerForm.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "millis.h"
#include "simcom_7600.h"
#include "simcom_7600_command.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @brief ----------------------------- Class -----------------------------
 */

/**
 * @brief ----------------------------- Prototype -----------------------------
 */

/**
 * @brief ----------------------------- typedef -----------------------------
 */
typedef struct {
    SIM7600_CMGL_stat_t stat;
    QString phoneNumber;
    QString dateTime;
    QString content;
} SMS_raw_t;
/**
 * @brief ----------------------------- Variable -----------------------------
 */
MainWindow *w;
MessengerForm *messForm; // Form hiển thị khi chọn button tin nhắn

SerialThread serialThread;
bool isModuleConnected = false;
QTimer timerMillis;
QTimer gsmTimer;

SIM7600_t moduleSim;
SIM7600_CMGL_param_t paramGet;
struct {
    char content[1024];
    char phoneNum[50];
    struct {
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } time;
    struct {
        uint8_t day;
        uint8_t month;
        uint8_t year;
    } date;
} SMS_recvBuff;

QList<Contact_t> phoneBook;

QThread *testThread;

/**
 * @brief ----------------------------- Function -----------------------------
 */
void sendData(char *pData, uint16_t dataLen) {
    if(serialThread.isConnected()) {
        serialThread.sendData(pData, dataLen);
    }
}
static QDateTime APP_dateTimeConvert(SIM7600_SMS_dateTime_t *dateAndTime) {
    QDateTime dateTime;
    dateTime.setDate(QDate(dateAndTime->date.year, dateAndTime->date.month, dateAndTime->date.day));
    dateTime.setTime(QTime(dateAndTime->time.hour, dateAndTime->time.minute, dateAndTime->time.second));
    return dateTime;
}

static void APP_updateShortMess(SMS_state_t state, QString phoneNumber, QDateTime dateTime, QString content) {
    QString dateTime_string =   QString::number(dateTime.time().hour()) + ":" +
                                QString::number(dateTime.time().minute()) + ":" +
                                QString::number(dateTime.time().second()) + " " +
                                QString::number(dateTime.date().day()) + "/" +
                                QString::number(dateTime.date().month()) + "/" +
                                QString::number(dateTime.date().year());
    messForm->updateShortMess(phoneNumber, content, dateTime_string);
}

static void APP_storage_addSMS(SMS_state_t state, QString phoneNumber, QDateTime dateTime, QString content) {
    Contact_t contact_temp;
    SMS_t SMS_temp;
    SMS_temp.state = state;
    SMS_temp.content = content;
    SMS_temp.dateTime = dateTime;
    /* Thêm tin nhắn vào những số đt cũ */
    for(uint16_t indexContact = 0; indexContact < phoneBook.count(); indexContact++) {
        if(phoneBook[indexContact].phoneNum == phoneNumber) {
            phoneBook[indexContact].SMS.append(SMS_temp);
            APP_updateShortMess(state, phoneNumber, dateTime, content);
            return;
        }
    }
    /* Thêm tin nhắn và số đt mới */
    contact_temp.phoneNum = phoneNumber;
    contact_temp.SMS.append(SMS_temp);
    phoneBook.append(contact_temp);
    APP_updateShortMess(state, phoneNumber, dateTime, content);
}

static void APP_showFullMessAtContact(QString contact) {
    uint16_t indexContact = 0;
    /* Lấy vị trí contact hiện tại */
    for(indexContact = 0; indexContact < phoneBook.count(); indexContact++) {
        if(phoneBook[indexContact].phoneNum == contact) {
            break;
        }
    }
    messForm->removeMessView();
    for(uint16_t indexMess = 0; indexMess < phoneBook[indexContact].SMS.count(); indexMess++) {
        MessengerForm::MessDir dir;
        if((phoneBook[indexContact].SMS[indexMess].state == SMS_state_UNREAD) || (phoneBook[indexContact].SMS[indexMess].state == SMS_state_READ)) {
            dir = MessengerForm::MessDir::RECV;
        }
        else {
            dir = MessengerForm::MessDir::SENT;
        }
        messForm->addMessToView(dir, phoneBook[indexContact].SMS[indexMess].content);
    }
}

void SIM_fullSMS_handler() {
    qDebug() << "SMS đã đầy, không thể lưu tin nhắn mới, vui lòng xóa bớt tin nhắn.";
}

void SIM_haveSMS_handler(SIM7600_SMS_stat_t stat, char *phoneNumber, SIM7600_SMS_dateTime_t *dateAndTime, char *content) {
    SMS_state_t state;
    switch(stat) {
        case SIM7600_SMS_stat_REC_UNREAD: {
            state = SMS_state_UNREAD;
            break;
        }
        case SIM7600_SMS_stat_REC_READ: {
            state = SMS_state_READ;
            break;
        }
        case SIM7600_SMS_stat_STO_UNSENT: {
            state = SMS_state_UNSENT;
            break;
        }
        case SIM7600_SMS_stat_STO_SENT: {
            state = SMS_state_SENT;
            break;
        }
    }
    APP_storage_addSMS(state, QString(phoneNumber), APP_dateTimeConvert(dateAndTime), QString(content));
}

void SIM_addSMSAt_handler(uint16_t index, SIM7600_SMS_stat_t stat, char *phoneNumber, SIM7600_SMS_dateTime_t *dateAndTime, char *content) {
    Q_UNUSED(index);
    SMS_state_t state;
    switch(stat) {
        case SIM7600_SMS_stat_REC_UNREAD: {
            state = SMS_state_UNREAD;
            break;
        }
        case SIM7600_SMS_stat_REC_READ: {
            state = SMS_state_READ;
            break;
        }
        case SIM7600_SMS_stat_STO_UNSENT: {
            state = SMS_state_UNSENT;
            break;
        }
        case SIM7600_SMS_stat_STO_SENT: {
            state = SMS_state_SENT;
            break;
        }
    }
    APP_storage_addSMS(state, QString(phoneNumber), APP_dateTimeConvert(dateAndTime), QString(content));
}


bool moduleGSM = false;

void testThread_func() {
    /* Lấy thông tin CMGF */
    SIM7600_AT_CMGF_get(&moduleSim);
    testThread->sleep(3);
    /* Lấy tất cả tin nhắn */
    paramGet.content = SMS_recvBuff.content;
    paramGet.phoneNumber = SMS_recvBuff.phoneNum;
    paramGet.callback = SIM_addSMSAt_handler;

    memset(paramGet.content, 0, 1024);
    memset(paramGet.phoneNumber, 0, 50);
    SIM7600_AT_CMGL_get(&moduleSim, SIM7600_CMGL_stat_ALL, (SIM7600_responseHandler)SIM7600_AT_CMGL_TEXTMode_handler, &paramGet);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow;

    messForm = new MessengerForm();
    w->setFormView(messForm);

    /* Chạy timer cho millis */
    timerMillis.setInterval(1);
    QObject::connect(&timerMillis, &QTimer::timeout, []() {
        __tick_now++;
    });
    timerMillis.start();
    /* Chajy timer cho GSM */
    gsmTimer.setInterval(100);
    QObject::connect(&gsmTimer, &QTimer::timeout, []() {
        SIM7600_exec(&moduleSim);
    });
    /* Tạo Thread test module sim */
    testThread = new QThread;
    QObject::connect(testThread, &QThread::started, [](){
        QMetaObject::invokeMethod(testThread, [&](){
            testThread_func();
        });
    });
    /* Sig&Slot mở cổng */
    QObject::connect(w, &MainWindow::openSerialPort, [](QString COMPort){
        if(serialThread.isConnected()) {
            serialThread.closeConnect();
        }
        serialThread.openConnect(COMPort);
    });
    /* Sig&Slot đóng cổng */
    QObject::connect(w, &MainWindow::closeSerialPort, [&](){
        if(serialThread.isConnected()) {
            serialThread.closeConnect();
            w->serialPortDisconnected();
        }
        testThread->exit();
        delete testThread;
        gsmTimer.stop();
    });
    /* Sig&Slot lấy các cổng đang có */
    QObject::connect(w, &MainWindow::getAvailableSerialPort, [](QList<QSerialPortInfo> &serialPortAvailable){
        serialPortAvailable = QSerialPortInfo::availablePorts();
    });
    /* Sig&Slot event kết nối thành công */
    QObject::connect(&serialThread, &SerialThread::connectedHandler, [&](){
        ::w->serialPortConnected();
        /* Khởi tạo module SIM */
        SIM7600_init(&moduleSim, 0, 0, 0, sendData, false);
        moduleSim.fullSMS_cb = SIM_fullSMS_handler;
        gsmTimer.start();
        testThread->start();
    });
    /* Sig&Slot event kết nối lỗi */
    QObject::connect(&serialThread, &SerialThread::connectedFailHandler, w, &MainWindow::serialPortDisconnected);
    /* Sig&Slot event nhận dữ liệu */
    QObject::connect(&serialThread, &SerialThread::dataReceived, [](QString data) {
        qDebug() << "Dữ liệu nhận được:" << data;
        char *dataChar = data.toUtf8().data();
        SIM7600_data_handler(&moduleSim, dataChar, data.toUtf8().length());
    });
    /* Sig&Slot hiển thị Full tin nhắn */
    QObject::connect(messForm, &MessengerForm::showAllMessAtContact, [=](QString contact) {
        APP_showFullMessAtContact(contact);
    });
    /* Hiển thị GUI */
    w->show();
    return a.exec();
}
