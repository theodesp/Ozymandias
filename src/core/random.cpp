#include "core/random.h"

#include <string.h>
#include "io/io_buffer.h"
//#include <cmath>

static random_data_t data;

const random_data_t * random_data_struct() {
    return &data;
}

void random_init(void) {
    memset(&data, 0, sizeof(data));
    data.iv1 = 0x54657687;
    data.iv2 = 0x72641663;
}

static void random_bits_fill() {
    data.random1_3bit = data.iv1 & 0x7;
    data.random1_7bit = data.iv1 & 0x7f;
    data.random1_15bit = data.iv1 & 0x7fff;
    data.random2_3bit = data.iv2 & 0x7;
    data.random2_7bit = data.iv2 & 0x7f;
    data.random2_15bit = data.iv2 & 0x7fff;
}
void random_generate_next(void) {
    data.pool[data.pool_index++] = data.random1_7bit;
    if (data.pool_index >= MAX_RANDOM)
        data.pool_index = 0;

    for (int i = 0; i < 31; i++) {
        unsigned int r1 = (((data.iv1 & 0x10) >> 4) ^ data.iv1) & 1;
        unsigned int r2 = (((data.iv2 & 0x10) >> 4) ^ data.iv2) & 1;
        data.iv1 = data.iv1 >> 1;
        data.iv2 = data.iv2 >> 1;
        if (r1)
            data.iv1 |= 0x40000000;
        if (r2)
            data.iv2 |= 0x40000000;
    }
    random_bits_fill();
}

void random_generate_pool(void) {
    data.pool_index = 0;
    for (int i = 0; i < MAX_RANDOM; i++) {
        random_generate_next();
    }
}

void random_TEMP_SET_DEBUG(uint32_t iv1, uint32_t iv2) {
    data.iv1 = iv1;
    data.iv2 = iv2;
    random_bits_fill();
}

int8_t random_byte(void) {
    return data.random1_7bit;
}
int8_t random_byte_alt(void) {
    return data.random2_7bit;
}
int16_t random_short(void) {
    return data.random1_15bit;
}
int32_t random_from_pool(int index) {
    return data.pool[(data.pool_index + index) % MAX_RANDOM];
}

void randomize_event_fields(int16_t field[4], int32_t *seed) {

    // original values
    auto f_fixed = field[1];
    auto f_min = field[2];
    auto f_max = field[3];

    // first operation
    if (f_fixed == -1) {
        if ((f_min > -1) && (f_max > -1) && (f_max == f_min)) {
            field[0] = f_fixed;
            return;
        }
    } else if (f_max == -1) {
        field[0] = f_fixed;
        return;
    }

    // second operation
    random_generate_next();
    unsigned long long random_broche = data.random1_15bit; //_DAT_00d3a360
    if (f_fixed < 0) {
        int field_range = f_max - f_min;
        *seed = (uint32_t)(random_broche / field_range);
        field[0] = (random_broche % field_range) + f_min;
        return;
    }

    // final composition
    int determinant;
    if (f_min < 0)
        determinant = 1;
    else
        determinant = (-1 < f_max) + 2;
    *seed = (int32_t)(random_broche / (unsigned long long)determinant);
    determinant = (int32_t)(random_broche % (unsigned long long)determinant);
    if (determinant != 0) {
        if (determinant == 1) {
            field[0] = f_min;
            return;
        }
        *seed = (int32_t)(uint16_t)f_max;
        field[0] = f_max;
        return;
    }

    // default value
    field[0] = f_fixed;
}

void random_around_point(int x_home, int y_home, int x, int y, int *dest_x, int *dest_y, int step, int bias, int max_dist) {
    random_generate_next();
    int det = 64/step;
    int rand_x = random_byte() / det - step;
    int rand_y = random_byte_alt() / det - step;
    *dest_x = x + rand_x;
    *dest_y = y + rand_y;
    int dist_x = (x_home - *dest_x);
    int dist_y = (y_home - *dest_y);
    if (bias <= 1)
        bias = 1;
    *dest_x += dist_x / bias;
    *dest_y += dist_y / bias;
    if (max_dist > 0) {
        dist_x = (x_home - *dest_x);
        dist_y = (y_home - *dest_y);
        if (dist_x > max_dist)
            *dest_x = x_home + max_dist;
        if (dist_x < -max_dist)
            *dest_x = x_home - max_dist;
        if (dist_y > max_dist)
            *dest_y = y_home + max_dist;
        if (dist_y < -max_dist)
            *dest_y = y_home - max_dist;
    }
}

// please email me if you have a better name for this
bool random_bool_lerp_scalar_int(int minimum, int maximum, int v) {

    // let's not make our life harder.
    v -= minimum;
    maximum -= minimum;
    minimum = 0;

    if (v <= minimum)
        return false;
    if (v >= maximum)
        return true;

    int length = maximum - minimum;
    int max_length = uint16_t(-1);
    float det = (float)max_length / (float)length;

    random_generate_next();
    uint16_t rand_short = (uint16_t)random_short();
    rand_short = rand_short / det;

    if (v < rand_short)
        return false;
    else
        return true;
}

io_buffer *iob_random_iv = new io_buffer([](io_buffer *iob) {
    iob->bind(BIND_SIGNATURE_UINT32, &data.iv1);
    iob->bind(BIND_SIGNATURE_UINT32, &data.iv2);
    random_bits_fill();
});

/////////

static uint32_t anti_scum_seed = 0;
// used in OG Pharaoh to get non-deterministic random values
uint16_t anti_scum_random_15bit(bool update) {
    if (update) {
        anti_scum_seed = anti_scum_seed * 214013 + 2531011;
        return anti_scum_seed >> 0x10 & 0x7fff;
    } else
        return anti_scum_seed;
}
bool anti_scum_random_bool() {
    int randm = anti_scum_random_15bit();
    randm = randm & 0x80000001;
    bool rnd_bool_test = randm == 0;
    if ((int)randm < 0) {
        rnd_bool_test = (randm - 1 | 0xfffffffe) == 0xffffffff;
    }
    return rnd_bool_test;
}