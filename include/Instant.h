#pragma once

#include "TimeZone.h"

#define MICROS_PER_MILLIS 1000L
#define MICROS_PER_SECOND 1000000LL

#define INSTANT_MIN_SECONDS (-31557014167219200L)
#define INSTANT_MAX_SECONDS (31556889864403199L)

#define INSTANT_COPY(instant) ((Instant){.seconds = (instant).seconds, .micros = (instant).micros})

typedef struct Instant {
    int64_t seconds;       // The number of seconds from the epoch of 1970-01-01T00:00:00Z.
    int32_t micros;        // The number of microseconds, later along the time-line, from the seconds field. This is always positive, and never exceeds 999999.
} Instant;

static const Instant EPOCH = {.seconds = 0, .micros = 0};


/* Obtains the current time instant from the hardware clock. Should be implemented on the driver side*/
extern Instant instantNow();


/* Obtains initialized Instant struct using seconds from the epoch of 1970-01-01T00:00:00Z.
 * The microseconds field is set to zero.
 * Params:
 *  epochSecond – the number of seconds from 1970-01-01T00:00:00Z
 * Returns: an Instant with the specified epoch seconds*/
Instant instantOfEpochSeconds(int64_t epochSecond);


/* Obtains initialized Instant struct using seconds from the epoch of 1970-01-01T00:00:00Z and microsecond fraction of second.
 * This method allows an arbitrary number of microseconds to be passed in.
 * The factory will alter the values of the second and microsecond in order to ensure that the stored microsecond is in the range 0 to 999,999.
 * For example, the following will result in exactly the same time instant:
 *      instantOfEpochSecondsAdjust(3, 1);
 *      instantOfEpochSecondsAdjust(4, -999999);
 *      instantOfEpochSecondsAdjust(2, 1000001);
 * Params:
 *  epochSecond – the number of seconds from 1970-01-01T00:00:00Z
 *  microAdjustment – the microsecond adjustment to the number of seconds, positive or negative
 * Returns: an Instant with the specified epoch seconds and micro adjustment*/
Instant instantOfEpochSecondsAdjust(int64_t epochSecond, int64_t microAdjustment);

/* Obtains initialized Instant struct using milliseconds from the epoch of 1970-01-01T00:00:00Z.
 * The seconds and microseconds are extracted from the specified milliseconds.
 * Params:
 *  epochMilli – the number of milliseconds from 1970-01-01T00:00:00Z
 * Returns: an Instant with the specified epoch millis*/
Instant instantOfEpochMillis(int64_t epochMilli);

/* This adds specified seconds to instant.
 * Params:
 *  secondsToAdd – the seconds to add, positive or negative*/
Instant *instantPlusSeconds(Instant *instant, int64_t secondsToAdd);

/* This adds specified milliseconds to Instant pointer.
 * Params:
 *  millisToAdd – the milliseconds to add, positive or negative*/
Instant *instantPlusMillis(Instant *instant, int64_t millisToAdd);

/* This adds specified microseconds to Instant pointer.
 * Params:
 *  microsToAdd – the microseconds to add, positive or negative*/
Instant *instantPlusMicros(Instant *instant, int64_t microsToAdd);

/* This subtracts specified seconds from Instant pointer.
 * Params:
 *  secondsToSubtract – the seconds to subtract, positive or negative*/
Instant *instantMinusSeconds(Instant *instant, int64_t secondsToSubtract);

/* This subtracts specified milliseconds from Instant pointer.
 * Params:
 *  millisToSubtract – the milliseconds to subtract, positive or negative*/
Instant *instantMinusMillis(Instant *instant, int64_t millisToSubtract);

/* This subtracts specified microseconds from Instant pointer.
 * Params:
 *  microsToSubtract – the microseconds to subtract, positive or negative*/
Instant *instantMinusMicros(Instant *instant, int64_t microsToSubtract);

/* Converts instant pointer to the number of milliseconds from the epoch of 1970-01-01T00:00:00Z.
 * If this instant has greater than millisecond precision,
 * then the conversion will drop any excess precision information as though the amount in microseconds was subject to integer division by one thousand.
 * Returns:
 *  the number of milliseconds since the epoch of 1970-01-01T00:00:00Z*/
int64_t instantToEpochMillis(Instant *instant);

/* Compares two time instants.
 * The comparison is based on the time-line position of the instants.
 * Params:
 *  first - the instant to check
 *  second – the other instant to compare to
 * Returns: the value, negative if less, positive if greater*/
int64_t instantCompare(Instant *first, Instant *second);

/* Checks if instant is after the other instant.
 * The comparison is based on the time-line position of the instants.
 * Params:
 *  instant – instant value to check
 *  other – the other instant to compare to
 * Returns: true if first instant is after the other instant*/
bool isInstantAfter(Instant *instant, Instant *other);

/* Checks if instant is before the other instant.
 * The comparison is based on the time-line position of the instants.
 * Params:
 *  instant – instant value to check
 *  other – the other instant to compare to
 * Returns: true if first instant is before the other instant*/
bool isInstantBefore(Instant *instant, Instant *other);

/* Checks if instant is between the other two instants.
 * The comparison is based on the time-line position of the instants.
 * Params:
 *  instant – instant value to check
 *  startExclusive – the instant from, exclusive
 *  endExclusive – the up to instant, exclusive
 * Returns: true if first instant is between the specified two instants*/
bool isInstantBetween(Instant *instant, Instant *startExclusive, Instant *endExclusive);

/* Checks if first instant is equal to the other instant.
 * The comparison is based on the time-line position of the instants.
 * Params:
 *  instant – instant value to check
 *  other – the other instant
 * Returns: true if the other instant is equal to the first*/
bool isInstantEquals(Instant *instant, Instant *other);