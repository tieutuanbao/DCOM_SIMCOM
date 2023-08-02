#ifndef __MILLIS_H__
#define __MILLIS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t __tick_now;

uint32_t millis();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
