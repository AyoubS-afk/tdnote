#include "instantiate.h"
#include "digital_wheel.h"

struct digital_wheel
{
    int32_t start;
    int32_t end;
    int32_t current;
};

digital_wheel digital_wheel_construct(int32_t start, int32_t end)
{
    INSTANTIATE(digital_wheel);
    self->start = start;
    self->end = end;
    digital_wheel_initialize(self);

    return self;
}

void digital_wheel_collect(digital_wheel self)
{
    free(self);
}

void digital_wheel_initialize(digital_wheel self)
{
    self->current = self->start;
}

bool digital_wheel_move_to_next_position(digital_wheel self)
{
    if (self->current == self->end)
    {
        digital_wheel_initialize(self);
        return false;
    }

    self->current++;
    return true;
}

int32_t digital_wheel_get_current_position(digital_wheel self)
{
    return self->current;
}
