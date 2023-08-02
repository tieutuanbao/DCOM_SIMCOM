#ifndef __SIMCARDMANAGER_H__
#define __SIMCARDMANAGER_H__

#include <QDateTime>
#include <QList>

typedef enum {
    SMS_state_UNREAD,
    SMS_state_READ,
    SMS_state_UNSENT,
    SMS_state_SENT
} SMS_state_t;

typedef struct {
    SMS_state_t state;
    QDateTime dateTime;
    QString content;
} SMS_t;

typedef struct {
    QList<SMS_t> SMS;
    QString phoneNum;
} Contact_t;

#endif // __SIMCARDMANAGER_H__
