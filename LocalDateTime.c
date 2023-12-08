#include "LocalDateTime.h"

static const DateTime UNINITIALIZED_DATE_TIME = {.date = {0},
        .time = {.hours = -1, .minutes = -1, .seconds = -1, .millis = -1}};


static void plusWithOverflow(Date *date, Time *time, int64_t hours, int64_t minutes, int64_t seconds, int64_t millis, int8_t sign);


DateTime dateTimeOf(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes) {
    Date date = dateOf(year, month, dayOfMonth);
    Time time = timeOf(hours, minutes, 0);
    return (DateTime){.date = date, .time = time};
}

DateTime dateTimeOfWithSeconds(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds) {
    Date date = dateOf(year, month, dayOfMonth);
    Time time = timeOf(hours, minutes, seconds);
    return (DateTime){.date = date, .time = time};
}

DateTime dateTimeOfWithMillis(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis) {
    Date date = dateOf(year, month, dayOfMonth);
    Time time = timeOfWithMillis(hours, minutes, seconds, millis);
    return (DateTime){.date = date, .time = time};
}

DateTime dateTimeFrom(const Date *date, const Time *time) {
    DateTime dateTime = UNINITIALIZED_DATE_TIME;
    if (isDateValid(date) && isTimeValid(time)) {
        return dateTimeOfWithMillis(date->year, date->month, date->day, time->hours, time->minutes, time->seconds, time->millis);
    }
    return dateTime;
}

DateTime dateTimeOfInstant(Instant *instant, const TimeZone *zone) {
    return dateTimeOfEpochSeconds(instant->seconds, instant->micros, zone->utcOffset);
}

DateTime dateTimeOfEpochSeconds(int64_t epochSeconds, int64_t microsOfSecond, int32_t zoneSecondsOffset) {
    DateTime dateTime = UNINITIALIZED_DATE_TIME;
    if (isValidValue(&MICROS_OF_SECOND_RANGE, microsOfSecond)) {
        int64_t localSeconds = epochSeconds + zoneSecondsOffset;
        int64_t localEpochDay = floorDiv(localSeconds, SECONDS_PER_DAY);
        int64_t secondsOfDay = floorMod(localSeconds, SECONDS_PER_DAY);
        Date date = dateOfEpochDay(localEpochDay);
        Time time = timeOfMicrosOfDay(secondsOfDay * MICROS_PER_SECOND + microsOfSecond);
        dateTime.date = date;
        dateTime.time = time;
    }
    return dateTime;
}

DateTime dateTimeAtStartOfDay(Date *date) {
    return dateTimeFrom(date, &MIDNIGHT);
}

DateTime *dateTimePlusYears(DateTime *dateTime, int64_t years) {
    datePlusYears(&dateTime->date, years);
    return dateTime;
}

DateTime *dateTimePlusMonths(DateTime *dateTime, int64_t months) {
    datePlusMonths(&dateTime->date, months);
    return dateTime;
}

DateTime *dateTimePlusWeeks(DateTime *dateTime, int64_t weeks) {
    datePlusWeeks(&dateTime->date, weeks);
    return dateTime;
}

DateTime *dateTimePlusDays(DateTime *dateTime, int64_t days) {
    datePlusDays(&dateTime->date, days);
    return dateTime;
}

DateTime *dateTimePlusHours(DateTime *dateTime, int64_t hours) {
    plusWithOverflow(&dateTime->date, &dateTime->time, hours, 0, 0, 0, 1);
    return dateTime;
}

DateTime * dateTimePlusMinutes(DateTime *dateTime, int64_t minutes) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, minutes, 0, 0, 1);
    return dateTime;
}

DateTime *dateTimePlusSeconds(DateTime *dateTime, int64_t seconds) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, 0, seconds, 0, 1);
    return dateTime;
}

DateTime *dateTimePlusMillis(DateTime *dateTime, int64_t millis) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, 0, 0, millis, 1);
    return dateTime;
}

DateTime *dateTimeMinusYears(DateTime *dateTime, int64_t years) {
    return dateTimePlusYears(dateTime, -years);
}

DateTime *dateTimeMinusMonths(DateTime *dateTime, int64_t months) {
    return dateTimePlusMonths(dateTime, -months);
}

DateTime *dateTimeMinusWeeks(DateTime *dateTime, int64_t weeks) {
    return dateTimePlusWeeks(dateTime, -weeks);
}

DateTime *dateTimeMinusDays(DateTime *dateTime, int64_t days) {
    return dateTimePlusDays(dateTime, -days);
}

DateTime *dateTimeMinusHours(DateTime *dateTime, int64_t hours) {
    plusWithOverflow(&dateTime->date, &dateTime->time, hours, 0, 0, 0, -1);
    return dateTime;
}

DateTime *dateTimeMinusMinutes(DateTime *dateTime, int64_t minutes) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, minutes, 0, 0, -1);
    return dateTime;
}

DateTime *dateTimeMinusSeconds(DateTime *dateTime, int64_t seconds) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, 0, seconds, 0, -1);
    return dateTime;
}

DateTime *dateTimeMinusMillis(DateTime *dateTime, int64_t millis) {
    plusWithOverflow(&dateTime->date, &dateTime->time, 0, 0, 0, millis, -1);
    return dateTime;
}

int64_t dateTimeToEpochSecond(DateTime *dateTime, int32_t offset) {
    if (!isDateValid(&dateTime->date) || !isTimeValid(&dateTime->time)) return INT64_MIN;
    int64_t epochDay = dateToEpochDay(&dateTime->date);
    int64_t totalSeconds = epochDay * 86400 + timeToSecondsOfDay(&dateTime->time);
    return (totalSeconds - offset);
}

int64_t dateTimeCompare(DateTime *first, DateTime *second) {
    int64_t compare = dateCompare(&first->date, &second->date);
    if (compare == 0) {
        return timeCompare(&first->time, &second->time);
    }
    return compare;
}

bool isDateTimeAfter(DateTime *dateTime, DateTime *other) {
    return dateTimeCompare(dateTime, other) > 0;
}

bool isDateTimeBefore(DateTime *dateTime, DateTime *other) {
    return dateTimeCompare(dateTime, other) < 0;
}

bool isDateTimeBetween(DateTime *dateTime, DateTime *startExclusive, DateTime *endExclusive) {
    return isDateTimeAfter(dateTime, startExclusive) && isDateTimeBefore(dateTime, endExclusive);
}

bool isDateTimeEquals(DateTime *dateTime, DateTime *other) {
    return dateTimeCompare(dateTime, other) == 0;
}

bool isDateTimeValid(DateTime *dateTime) {
    return dateTime != NULL && isDateValid(&dateTime->date) && isTimeValid(&dateTime->time);
}

/* Combines result for this DateTime with the specified period added.
 * Params:
 *  date – the date to base the calculation on
 *  hours – the hours to add, may be negative
 *  minutes – the minutes to add, may be negative
 *  seconds – the seconds to add, may be negative
 *  millis – the milliseconds to add, may be negative
 *  sign – the sign to determine add or subtract*/
static void plusWithOverflow(Date *date, Time *time, int64_t hours, int64_t minutes, int64_t seconds, int64_t millis, int8_t sign) {
    if ((hours | minutes | seconds | millis) == 0) return;
    int64_t toTotalDays = (millis / MILLIS_PER_DAY) +
            (seconds / SECONDS_PER_DAY) +
            (minutes / MINUTES_PER_DAY) +
            (hours / HOURS_PER_DAY);
    toTotalDays *= sign;

    int64_t totalTimeMillis = (millis % MILLIS_PER_DAY) +
            (seconds % SECONDS_PER_DAY) * MILLIS_PER_SECOND +
            (minutes % MINUTES_PER_DAY) * MILLIS_PER_MINUTE +
            (hours % HOURS_PER_DAY) * MILLIS_PER_HOUR;

    int64_t currentMillisOfDay = timeToMillisOfDay(time);
    totalTimeMillis = totalTimeMillis * sign + currentMillisOfDay;
    toTotalDays += floorDiv(totalTimeMillis, MILLIS_PER_DAY);
    int64_t newMillisOfDay = floorMod(totalTimeMillis, MILLIS_PER_DAY);
    Time newTime = (newMillisOfDay == currentMillisOfDay) ? *time : timeOfMillisOfDay(newMillisOfDay);
    datePlusDays(date, toTotalDays);
    *time = newTime;
}