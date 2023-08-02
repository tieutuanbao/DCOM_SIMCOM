#include "millis.h"

uint32_t __tick_now = 0;

uint32_t millis() {
    return __tick_now;
}
