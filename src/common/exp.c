#include "types.h"

float _exp(float x) {
    float p = 1.442695040f * x;
    uint32_t i = 0;
    uint32_t sign = (i >> 31);
    int w = (int) p;
    float z = p - (float) w + (float) sign;
    union {
        uint32_t i;
        float f;
    } v = {.i = (uint32_t) ((1 << 23) * (p + 121.2740838f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z))};
    return v.f;
}
