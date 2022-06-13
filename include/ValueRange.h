#pragma once

#include <stdint.h>
#include <stdbool.h>


typedef struct ValueRange {
    int64_t min;
    int64_t max;
} ValueRange;

static const ValueRange HOUR_OF_DAY_RANGE = {.min = 0, .max = 23};
static const ValueRange HOUR_OF_AM_PM = {.min = 1, .max = 12};
static const ValueRange MINUTE_OF_HOUR_RANGE = {.min = 0, .max = 59};
static const ValueRange SECOND_OF_MINUTE_RANGE = {.min = 0, .max = 59};
static const ValueRange MILLIS_OF_SECOND_RANGE = {.min = 0, .max = 999};
static const ValueRange SECOND_OF_DAY_RANGE = {.min = 0, .max = (86400L - 1)};
static const ValueRange MILLIS_OF_DAY_RANGE = {.min = 0, .max = ((86400L * 1000) - 1)};
static const ValueRange MICROS_OF_SECOND_RANGE = {.min = 0, .max = 999999};
static const ValueRange MICROS_OF_DAY_RANGE = {.min = 0, .max = ((86400LL * 1000000LL) - 1)};

static const ValueRange YEAR_RANGE = {.min = -9999999, .max = 9999999};
static const ValueRange MONTH_OF_YEAR_RANGE = {.min = 1, .max = 12};
static const ValueRange DAY_OF_MONTH_RANGE = {.min = 1, .max = 31};
static const ValueRange DAY_OF_YEAR_RANGE = {.min = 1, .max = 366};

static const ValueRange EPOCH_DAY_RANGE = {.min = -365243219162, .max = 365241780471};

static inline bool isValidValue(const ValueRange *range, int64_t value) {
    return (value >= range->min && value <= range->max);
}