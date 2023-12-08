#include "LocalTime.h"

#define UNINITIALIZED_TIME ((Time){.hours = -1, .minutes = -1, .seconds = -1, .millis = -1})

static Time * setTime(Time *time, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis);
static bool isProvidedTimeValid(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis);


Time timeOf(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    Time time = UNINITIALIZED_TIME;
    if (isProvidedTimeValid(hours, minutes, seconds, 0)) {
        setTime(&time, hours, minutes, seconds, 0);
    }
    return time;
}

Time timeOfInstant(Instant *instant, const TimeZone *zone) {
    if (instant != NULL && zone != NULL) {
        int64_t localSeconds = instant->seconds + zone->utcOffset;
        int64_t secondsOfDay = floorMod(localSeconds, SECONDS_PER_DAY);
        return timeOfMillisOfDay((secondsOfDay * MICROS_PER_SECOND + instant->micros) / 1000);
    }
    return UNINITIALIZED_TIME;
}

Time timeOfWithMillis(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis) {
    Time time = UNINITIALIZED_TIME;
    if (isProvidedTimeValid(hours, minutes, seconds, millis)) {
        setTime(&time, hours, minutes, seconds, millis);
    }
    return time;
}

Time timeOfHourAndMinutes(uint8_t hours, uint8_t minutes) {
    return timeOf(hours, minutes, 0);
}

Time timeOfSecondOfDay(uint32_t secondOfDay) {
    Time time = UNINITIALIZED_TIME;
    if (isValidValue(&SECOND_OF_DAY_RANGE, secondOfDay)) {
        uint8_t hours = secondOfDay / SECONDS_PER_HOUR;
        secondOfDay -= hours * SECONDS_PER_HOUR;
        uint8_t minutes = secondOfDay / SECONDS_PER_MINUTE;
        secondOfDay -= minutes * SECONDS_PER_MINUTE;
        setTime(&time, hours, minutes, secondOfDay, 0);
    }
    return time;
}

Time timeOfMillisOfDay(uint32_t millisOfDay) {
    Time time = UNINITIALIZED_TIME;
    if (isValidValue(&MILLIS_OF_DAY_RANGE, millisOfDay)) {
        uint8_t hours = millisOfDay / MILLIS_PER_HOUR;
        millisOfDay -= hours * MILLIS_PER_HOUR;
        uint8_t minutes = millisOfDay / MILLIS_PER_MINUTE;
        millisOfDay -= minutes * MILLIS_PER_MINUTE;
        uint8_t seconds = millisOfDay / MILLIS_PER_SECOND;
        millisOfDay -= seconds * MILLIS_PER_SECOND;
        setTime(&time, hours, minutes, seconds, millisOfDay);
    }
    return time;
}

Time timeOfMicrosOfDay(int64_t microsOfDay) {
    Time time = UNINITIALIZED_TIME;
    if (isValidValue(&MICROS_OF_DAY_RANGE, microsOfDay)) {
        uint8_t hours = microsOfDay / MICROS_PER_HOUR;
        microsOfDay -= hours * MICROS_PER_HOUR;
        uint8_t minutes = microsOfDay / MICROS_PER_MINUTE;
        microsOfDay -= minutes * MICROS_PER_MINUTE;
        uint8_t seconds = microsOfDay / MICROS_PER_SECOND;
        microsOfDay -= seconds * MICROS_PER_SECOND;
        uint32_t millisOfDay = microsOfDay / MICROS_PER_MILLIS;
        setTime(&time, hours, minutes, seconds, millisOfDay);
    }
    return time;
}

Time *timePlusHours(Time *time, int32_t hoursToAdd) {
    if (hoursToAdd == 0 || !isTimeValid(time)) return time;
    uint8_t newHours = ((hoursToAdd % HOURS_PER_DAY) + time->hours + HOURS_PER_DAY) % HOURS_PER_DAY;
    return setTime(time, newHours, time->minutes, time->seconds, 0);
}

Time *timePlusMinutes(Time *time, int32_t minutesToAdd) {
    if (minutesToAdd == 0 || !isTimeValid(time)) return time;
    int32_t minutesOfDay = time->hours * MINUTES_PER_HOUR + time->minutes;
    int32_t newMinutesOfDay = (minutesToAdd % MINUTES_PER_DAY + minutesOfDay + MINUTES_PER_DAY) % MINUTES_PER_DAY;
    if (minutesOfDay == newMinutesOfDay) return time;

    uint8_t newHour = newMinutesOfDay / MINUTES_PER_HOUR;
    uint8_t newMinute = newMinutesOfDay % MINUTES_PER_HOUR;
    return setTime(time, newHour, newMinute, time->seconds, 0);
}

Time *timePlusSeconds(Time *time, int32_t secondsToAdd) {
    if (secondsToAdd == 0 || !isTimeValid(time)) return time;
    int32_t secondsOfDay = time->hours * SECONDS_PER_HOUR + time->minutes * SECONDS_PER_MINUTE + time->seconds;
    int32_t newSecondsOfDay = ((secondsToAdd % SECONDS_PER_DAY) + secondsOfDay + SECONDS_PER_DAY) % SECONDS_PER_DAY;
    if (secondsOfDay == newSecondsOfDay) return time;

    int32_t newHour = newSecondsOfDay / SECONDS_PER_HOUR;
    int32_t newMinute = (newSecondsOfDay / SECONDS_PER_MINUTE) % MINUTES_PER_HOUR;
    int32_t newSecond = newSecondsOfDay % SECONDS_PER_MINUTE;
    return setTime(time, newHour, newMinute, newSecond, 0);
}

Time *timePlusMillis(Time *time, int32_t millisToAdd) {
    if (millisToAdd == 0 || !isTimeValid(time)) return time;
    uint32_t millisOfDay = timeToMillisOfDay(time);
    uint32_t newMillisOfDay = ((millisToAdd % MILLIS_PER_DAY) + millisOfDay + MILLIS_PER_DAY) % MILLIS_PER_DAY;
    if (millisOfDay == newMillisOfDay) return time;

    uint32_t newHour = newMillisOfDay / MILLIS_PER_HOUR;
    uint32_t newMinute = (newMillisOfDay / MILLIS_PER_MINUTE) % MINUTES_PER_HOUR;
    uint32_t newSecond = (newMillisOfDay / MILLIS_PER_SECOND) % SECONDS_PER_MINUTE;
    uint32_t newMillis = newMillisOfDay % MILLIS_PER_SECOND;
    return setTime(time, newHour, newMinute, newSecond, newMillis);
}

Time *timeMinusHours(Time *time, int32_t hoursToSubtract) {
   return timePlusHours(time, -(hoursToSubtract % HOURS_PER_DAY));
}

Time *timeMinusMinutes(Time *time, int32_t minutesToSubtract) {
    return timePlusMinutes(time, -(minutesToSubtract % MINUTES_PER_DAY));
}

Time *timeMinusSeconds(Time *time, int32_t secondsToSubtract) {
    return timePlusSeconds(time, -(secondsToSubtract % SECONDS_PER_DAY));
}

Time *timeMinusMillis(Time *time, int32_t millisToSubtract) {
    return timePlusMillis(time, -millisToSubtract);
}

uint32_t timeToSecondsOfDay(Time *time) {
    if (isTimeValid(time)) {
        uint32_t total = time->hours * SECONDS_PER_HOUR;
        total += time->minutes * SECONDS_PER_MINUTE;
        total += time->seconds;
        return total;
    }
    return 0;
}

uint32_t timeToMillisOfDay(Time *time) {
    if (isTimeValid(time)) {
        uint32_t total = time->hours * MILLIS_PER_HOUR;
        total += time->minutes * MILLIS_PER_MINUTE;
        total += time->seconds * MILLIS_PER_SECOND;
        total += time->millis;
        return total;
    }
    return 0;
}

int8_t timeCompare(const Time *first, const Time *second) {
    if (!isTimeValid(first) || !isTimeValid(second)) return INT8_MIN;
    int8_t compare = int32Compare(first->hours, second->hours);
    if (compare == 0) {
        compare = int32Compare(first->minutes, second->minutes);
        if (compare == 0) {
            compare = int32Compare(first->seconds, second->seconds);
            if (compare == 0) {
                compare = int32Compare(first->millis, second->millis);
            }
        }
    }
    return compare;
}

bool isTimeAfter(Time *time, Time *other) {
    return timeCompare(time, other) > 0;
}

bool isTimeBefore(Time *time, Time *other) {
    return timeCompare(time, other) < 0;
}

bool isTimeBetween(Time *time, Time *startExclusive, Time *endExclusive) {
    return isTimeAfter(time, startExclusive) && isTimeBefore(time, endExclusive);
}

bool isTimeEquals(const Time *time, const Time *other) {
    return timeCompare(time, other) == 0;
}

bool isTimeValid(const Time *time) {
    return time != NULL && isProvidedTimeValid(time->hours, time->minutes, time->seconds, time->millis);
}

static Time *setTime(Time *time, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis) {
    time->hours = (int8_t) hours;
    time->minutes = (int8_t) minutes;
    time->seconds = (int8_t) seconds;
    time->millis = (int16_t) millis;
    return time;
}

static bool isProvidedTimeValid(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis) {
    return isValidValue(&HOUR_OF_DAY_RANGE, hours) &&
           isValidValue(&MINUTE_OF_HOUR_RANGE, minutes) &&
           isValidValue(&SECOND_OF_MINUTE_RANGE, seconds) &&
           isValidValue(&MILLIS_OF_SECOND_RANGE, millis);
}
