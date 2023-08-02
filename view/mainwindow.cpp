#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainContainner_layout = new QVBoxLayout(ui->mainContainer);
    mainContainner_layout->setContentsMargins(0, 0, 0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFormView(QWidget *formView)
{
    this->mainContainner_layout->takeAt(0); // Xóa form hiện tại
    this->mainContainner_layout->addWidget(formView); // Set form hiển thị
}

void MainWindow::on_refreshSerial_button_clicked()
{
    emit getAvailableSerialPort(this->serialPortAvail);
    this->ui->comboBox->clear();
    /* Đưa thông tin các cổng COM vào mảng và ComboBox */
    foreach (const QSerialPortInfo &sPortInfo, this->serialPortAvail) {
        this->ui->comboBox->addItem(sPortInfo.portName() + " - " + sPortInfo.description() + " " + (sPortInfo.isBusy() ? "(Used)" : "(Unused)"));
    }
}

void MainWindow::on_openCloseSerial_button_clicked()
{
    if(this->isSerialPortConnected == false) {
        emit this->openSerialPort(this->serialPortAvail.at(ui->comboBox->currentIndex()).portName());
    }
    else {
        emit this->closeSerialPort();
    }
}

void MainWindow::serialPortConnected()
{
    this->ui->comboBox->setDisabled(true);
    this->ui->openCloseSerial_button->setText("Ngắt kết nối");
    this->isSerialPortConnected = true;
}

void MainWindow::serialPortDisconnected()
{
    this->ui->comboBox->setEnabled(true);
    this->ui->openCloseSerial_button->setText("Kết nối");
    this->isSerialPortConnected = false;
}
