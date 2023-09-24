#include "Instant.h"


static void setInstant(Instant *instant, int64_t seconds, int32_t micros);
static void instantPlus(Instant *instant, int64_t secondsToAdd, int64_t microsToAdd);


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

void instantPlusSeconds(Instant *instant, int64_t secondsToAdd) {
    instantPlus(instant, secondsToAdd, 0);
}

void instantPlusMillis(Instant *instant, int64_t millisToAdd) {
    return instantPlus(instant, millisToAdd / 1000, (millisToAdd % 1000) * 1000);
}

void instantPlusMicros(Instant *instant, int64_t microsToAdd) {
    instantPlus(instant, 0, microsToAdd);
}

void instantMinusSeconds(Instant *instant, int64_t secondsToSubtract) {
    instantPlusSeconds(instant, -secondsToSubtract);
}

void instantMinusMillis(Instant *instant, int64_t millisToSubtract) {
   instantPlusMillis(instant, -millisToSubtract);
}

void instantMinusMicros(Instant *instant, int64_t microsToSubtract) {
    instantPlusMicros(instant, -microsToSubtract);
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

bool isInstantEquals(Instant *instant, Instant *other) {
    return instantCompare(instant, other) == 0;
}

static void setInstant(Instant *instant, int64_t seconds, int32_t micros) {
    if (seconds >= INSTANT_MIN_SECONDS && seconds <= INSTANT_MAX_SECONDS) {
        instant->seconds = seconds;
        instant->micros = micros;
    }
}

static void instantPlus(Instant *instant, int64_t secondsToAdd, int64_t microsToAdd) {
    if (instant == NULL || (secondsToAdd | microsToAdd) == 0) return;
    int64_t epochSeconds = instant->seconds + secondsToAdd;
    epochSeconds += microsToAdd / MICROS_PER_SECOND;
    microsToAdd = microsToAdd % MICROS_PER_SECOND;
    int64_t microAdjustment = instant->micros + microsToAdd;

    int64_t seconds = epochSeconds + floorDiv(microAdjustment, MICROS_PER_SECOND);
    int32_t microsOfSecond = (int32_t) floorMod(microAdjustment, MICROS_PER_SECOND);
    setInstant(instant, seconds, microsOfSecond);
}