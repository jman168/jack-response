#pragma once

#include <jack/jack.h>
#include "math.h"

int process (jack_nframes_t nframes, void *arg);

double float_to_db(double value) {
    return 20*log10(abs(value));
}

double db_to_float(double db) {
    return pow(db/20.0, 10.0);
}