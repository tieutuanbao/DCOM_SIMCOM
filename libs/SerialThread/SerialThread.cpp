#include "SerialThread.h"

SerialThread::SerialThread(QObject *parrent) : QThread(parrent)
{
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    this->connect(&serialPort, &QSerialPort::readyRead, this, &SerialThread::readData);
}
SerialThread::~SerialThread() {
    serialPort.close();
}
void SerialThread::openConnect(QString portName) {
    serialPort.setPortName(portName);
    if (serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Đã mở cổng Serial thành công";
        emit this->connectedHandler();
        exec();
    } else {
        qDebug() << "Không thể mở cổng Serial";
        emit this->connectedFailHandler();
    }
}
void SerialThread::closeConnect() {
    serialPort.close();
}
void SerialThread::sendData(char *pData, uint16_t dataLen) {
    this->serialPort.write(pData, dataLen);
}
bool SerialThread::isConnected() {
    return serialPort.isOpen();
}
void SerialThread::readData()
{
    // Đọc dữ liệu từ cổng Serial
    if(serialPort.isReadable()) {
        QByteArray data = serialPort.readAll();
        /* Xử lý dữ liệu */
        for (int i = 0; i < data.size(); ++i) {
            char c = data.at(i);
            if(c <= 0x7E) { // Chỉ lấy dữ liệu trong bảng ASCII
                dataBuffer.append(c);
            }
        }
        serialPort.flush();
    }
    while(1) {
        int endFrame = dataBuffer.indexOf("\r\n");
        if(endFrame == -1) {
            break;
        }
        endFrame += 2;
        currentFrame = dataBuffer.mid(0, endFrame);
        dataBuffer.remove(0, endFrame);
        /* Gửi dữ liệu nhận được đến main thread thông qua signal nếu đã hoàn thành Frame */
        emit dataReceived(currentFrame);
    }
}
