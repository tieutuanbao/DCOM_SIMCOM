#ifndef MESSRECVCONTENTFORM_H
#define MESSRECVCONTENTFORM_H

#include <QFrame>

namespace Ui {
    class MessRecvContentForm;
}

class MessRecvContentForm : public QFrame
{
    Q_OBJECT

public:
    explicit MessRecvContentForm(QWidget *parent = nullptr);
    ~MessRecvContentForm();
    void setContent(QString content);
private:
    Ui::MessRecvContentForm *ui;
};

#endif // MESSRECVCONTENTFORM_H
