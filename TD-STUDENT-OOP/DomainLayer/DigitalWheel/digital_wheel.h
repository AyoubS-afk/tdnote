#ifndef DIGITAL_WHEEL_H
#define DIGITAL_WHEEL_H
#include <stdint.h>
#include <stdbool.h>
#include "class.h"

CLASS(digital_wheel);

extern digital_wheel digital_wheel_construct(int32_t start, int32_t end);
extern void digital_wheel_collect(digital_wheel self);

extern void digital_wheel_initialize(digital_wheel self);
extern bool digital_wheel_move_to_next_position(digital_wheel self);
extern int32_t digital_wheel_get_current_position(digital_wheel self);

#endif
