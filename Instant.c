#include "Instant.h"


static Instant * setInstant(Instant *instant, int64_t seconds, int32_t micros);
static Instant * instantPlus(Instant *instant, int64_t secondsToAdd, int64_t microsToAdd);


Instant instantOfEpochSeconds(int64_t epochSecond) {
    Instant instant = EPOCH;
    setInstant(&instant, epochSecond, 0);
    return instant;
}

Instant instantOfEpochSecondsAdjust(int64_t epochSecond, int64_t microAdjustment) {
    Instant instant = EPOCH;
    int64_t seconds = epochSecond + floorDiv(microAdjustment, MICROS_PER_SECOND);
    int32_t microsOfSecond = (int32_t) floorMod(microAdjustment, MICROS_PER_SECOND);
    setInstant(&instant, seconds, microsOfSecond);
    return instant;
}

Instant instantOfEpochMillis(int64_t epochMilli) {
    Instant instant = EPOCH;
    int64_t seconds = floorDiv(epochMilli, 1000);
    int32_t millisOfSecond = (int32_t) floorMod(epochMilli, 1000);
    setInstant(&instant, seconds, millisOfSecond * 1000);
    return instant;
}

Instant *instantPlusSeconds(Instant *instant, int64_t secondsToAdd) {
    return instantPlus(instant, secondsToAdd, 0);
}

Instant *instantPlusMillis(Instant *instant, int64_t millisToAdd) {
    return instantPlus(instant, millisToAdd / 1000, (millisToAdd % 1000) * 1000);
}

Instant *instantPlusMicros(Instant *instant, int64_t microsToAdd) {
    return instantPlus(instant, 0, microsToAdd);
}

Instant *instantMinusSeconds(Instant *instant, int64_t secondsToSubtract) {
    return instantPlusSeconds(instant, -secondsToSubtract);
}

Instant *instantMinusMillis(Instant *instant, int64_t millisToSubtract) {
   return instantPlusMillis(instant, -millisToSubtract);
}

Instant *instantMinusMicros(Instant *instant, int64_t microsToSubtract) {
    return instantPlusMicros(instant, -microsToSubtract);
}

int64_t instantToEpochMillis(Instant *instant) {
    if (instant == NULL) return 0;
    if (instant->seconds < 0 && instant->micros > 0) {
        int64_t millis = multiplyExact(instant->seconds + 1, 1000);
        int64_t adjustment = (instant->micros / 1000) - 1000;
        return addExact(millis, adjustment);
    }
    int64_t millis = multiplyExact(instant->seconds, 1000);
    return addExact(millis, instant->micros / 1000);
}

int64_t instantCompare(Instant *first, Instant *second) {
    int8_t compare = int64Compare(first->seconds, second->seconds);
    if (compare != 0) {
        return compare;
    }
    return first->micros - second->micros;
}

bool isInstantAfter(Instant *instant, Instant *other) {
    return instantCompare(instant, other) > 0;
}

bool isInstantBefore(Instant *instant, Instant *other) {
    return instantCompare(instant, other) < 0;
}

bool isInstantBetween(Instant *instant, Instant *startExclusive, Instant *endExclusive) {
    return isInstantAfter(instant, startExclusive) && isInstantBefore(instant, endExclusive);
}

bool isInstantEquals(Instant *instant, Instant *other) {
    return instantCompare(instant, other) == 0;
}

static Instant *setInstant(Instant *instant, int64_t seconds, int32_t micros) {
    if (seconds >= INSTANT_MIN_SECONDS && seconds <= INSTANT_MAX_SECONDS) {
        instant->seconds = seconds;
        instant->micros = micros;
    }
    return instant;
}

static Instant *instantPlus(Instant *instant, int64_t secondsToAdd, int64_t microsToAdd) {
    if (instant == NULL || (secondsToAdd | microsToAdd) == 0) return instant;
    int64_t epochSeconds = instant->seconds + secondsToAdd;
    epochSeconds += microsToAdd / MICROS_PER_SECOND;
    microsToAdd = microsToAdd % MICROS_PER_SECOND;
    int64_t microAdjustment = instant->micros + microsToAdd;

    int64_t seconds = epochSeconds + floorDiv(microAdjustment, MICROS_PER_SECOND);
    int32_t microsOfSecond = (int32_t) floorMod(microAdjustment, MICROS_PER_SECOND);
    return setInstant(instant, seconds, microsOfSecond);
}