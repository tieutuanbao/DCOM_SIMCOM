#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QDateTime>

#include "SimCardManager.h"
#include "MessShort.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "millis.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setFormView(QWidget *formView);
    void setConnected() {
        this->isSerialPortConnected = true;
    }
    void setDisconnected() {
        this->isSerialPortConnected = false;
    }
private slots:
    void on_refreshSerial_button_clicked();
    void on_openCloseSerial_button_clicked();
public slots:
    void serialPortConnected();
    void serialPortDisconnected();
signals:
    void getAvailableSerialPort(QList<QSerialPortInfo> &serialPortAvailable);
    void openSerialPort(QString COMPort);
    void closeSerialPort();
private:
    Ui::MainWindow *ui;
    QVBoxLayout *mainContainner_layout;

    QList<QSerialPortInfo> serialPortAvail; // Lưu thông tin các Port có sẵn
    bool isSerialPortConnected = false;
};
#endif // MAINWINDOW_H
