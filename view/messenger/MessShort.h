#ifndef MESSSHORT_H
#define MESSSHORT_H

#include <QFrame>

namespace Ui {
    class MessShort;
}

class MessShort : public QFrame
{
    Q_OBJECT

public:
    explicit MessShort(QWidget *parent = nullptr);
    ~MessShort();

    void setPhoneNum(QString phoneNum);
    QString getPhoneNum();
    void setDateTime(QString dateTime);
    QString getDateTime();
    void setContent(QString content);
    QString getContent();
protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void mousePress_signal(QString phoneNum);
private:
    Ui::MessShort *ui;
};

#endif // MESSSHORT_H
