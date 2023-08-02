#ifndef __SERIAL_THREAD_H__
#define __SERIAL_THREAD_H__

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>

class SerialThread : public QThread
{
    Q_OBJECT

private:
    QSerialPort serialPort;
    QString dataBuffer;
    QString currentFrame;
public:
    explicit SerialThread(QObject *parrent = nullptr);
    virtual ~SerialThread();
    void openConnect(QString portName = "COM1");
    void closeConnect();
    void sendData(char *pData, uint16_t dataLen);
    bool isConnected();
signals:
    void dataReceived(QString data);
    void connectedHandler();
    void connectedFailHandler();
private slots:
    void readData();
};

#endif
