#ifndef MESSSENTCONTENTFORM_H
#define MESSSENTCONTENTFORM_H

#include <QFrame>

namespace Ui {
    class MessSentContentForm;
}

class MessSentContentForm : public QFrame
{
    Q_OBJECT

public:
    explicit MessSentContentForm(QWidget *parent = nullptr);
    ~MessSentContentForm();

    void setContent(QString content);
private:
    Ui::MessSentContentForm *ui;
};

#endif // MESSSENTCONTENTFORM_H
