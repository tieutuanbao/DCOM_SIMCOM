#ifndef MESSENGERFORM_H
#define MESSENGERFORM_H

#include <QFrame>
#include <QVBoxLayout>

namespace Ui {
    class MessengerForm;
}

class MessengerForm : public QFrame
{
    Q_OBJECT

public:
    enum MessDir {
        RECV,
        SENT
    };

    explicit MessengerForm(QWidget *parent = nullptr);
    ~MessengerForm();

    void updateShortMess(QString contact, QString content, QString dateTime);
    void setViewMess();
    void removeAllShortMess();
    void removeMessView();
    void addMessToView(MessDir dir, QString content);
signals:
    void showAllMessAtContact(QString contact);
private slots:
    void on_textEdit_textChanged();

private:
    Ui::MessengerForm *ui;
    const int messTyping_maxHeight = 300;
};

#endif // MESSENGERFORM_H
