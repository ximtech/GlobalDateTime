#pragma once

#include "inttypes.h"

#include "Instant.h"
#include "ValueRange.h"

#define HOURS_PER_DAY 24

#define MINUTES_PER_HOUR   60
#define MINUTES_PER_DAY   (MINUTES_PER_HOUR * HOURS_PER_DAY)

#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR  (SECONDS_PER_MINUTE * MINUTES_PER_HOUR)
#define SECONDS_PER_DAY   (SECONDS_PER_HOUR * HOURS_PER_DAY)

#define MILLIS_PER_SECOND 1000L
#define MILLIS_PER_MINUTE (MILLIS_PER_SECOND * SECONDS_PER_MINUTE)
#define MILLIS_PER_HOUR   (MILLIS_PER_MINUTE * MINUTES_PER_HOUR)
#define MILLIS_PER_DAY    (MILLIS_PER_HOUR * HOURS_PER_DAY)

#define MICROS_PER_MINUTE (MICROS_PER_SECOND * SECONDS_PER_MINUTE)
#define MICROS_PER_HOUR   (MICROS_PER_MINUTE * MINUTES_PER_HOUR)
#define MICROS_PER_DAY    (MICROS_PER_HOUR * HOURS_PER_DAY)

#define TIME_COPY(time) ((Time){.hours = (time).hours, .minutes = (time).minutes, .seconds = (time).seconds, .millis = (time).millis})

typedef struct Time {
    int8_t hours;
    int8_t minutes;
    int8_t seconds;
    int16_t millis;
} Time;

/* The time of midnight at the start of the day, '00:00'.*/
static const Time MIDNIGHT = {.hours = 0, .minutes = 0, .seconds = 0, .millis = 0};

/* Obtains the current time from the hardware clock. Should be implemented on the driver side*/
extern Time timeNow();

/* Returns Time initialized struct from an hour, minute, second. If values invalid, returns struct with negative values -> -1
 * Params:
 *  hour – the hour-of-day to represent, from 0 to 23
 *  minute – the minute-of-hour to represent, from 0 to 59
 *  second – the second-of-minute to represent, from 0 to 59*/
Time timeOf(uint8_t hours, uint8_t minutes, uint8_t seconds);

/* Returns Time initialized struct from an instant and zone.
 * This creates a local time based on the specified instant.
 * First, the gmtOffset from UTC/Greenwich is obtained using the zone and instant, which is simple as there is only one valid gmtOffset for each instant.
 * Then, the instant and gmtOffset are used to calculate the time.
 * Params:
 *  instant – the instant to create the time from
 *  zone – the time-zone, which may be an gmtOffset*/
Time timeOfInstant(Instant *instant, const TimeZone *zone);
Time timeOfWithMillis(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis);
Time timeOfHourAndMinutes(uint8_t hours, uint8_t minutes);
Time timeOfSecondOfDay(uint32_t secondOfDay);
Time timeOfMillisOfDay(uint32_t millisOfDay);
Time timeOfMicrosOfDay(int64_t microsOfDay);

/* This adds the specified number of hours to this time. The calculation wraps around midnight.
 * This instance is immutable and unaffected by this method call.
 * Params:
 *  hoursToAdd – the hours to add, may be negative*/
Time *timePlusHours(Time *time, int32_t hoursToAdd);

/* This adds the specified number of minutes to this time. The calculation wraps around midnight.
 * Params:
 *  minutesToAdd – the minutes to add, may be negative
 * Returns: a time based on this time with the minutes added*/
Time *timePlusMinutes(Time *time, int32_t minutesToAdd);

/* This adds the specified number of seconds to this time. The calculation wraps around midnight.
 * Params:
 *  secondsToAdd – the seconds to add, may be negative
 * Returns: a time based on this time with the seconds added*/
Time *timePlusSeconds(Time *time, int32_t secondsToAdd);

/* This adds the specified number of milliseconds to this time. The calculation wraps around midnight.
 * Params:
 * millisToAdd – the millis to add, may be negative
 * Returns: a time based on this time with the milliseconds added*/
Time *timePlusMillis(Time *time, int32_t millisToAdd);

/* This subtracts the specified number of hours from this time. The calculation wraps around midnight.
 * Params:
 *  hoursToSubtract – the hours to subtract, may be negative
 * Returns: a time based on this time with the hours amount subtracted*/
Time *timeMinusHours(Time *time, int32_t hoursToSubtract);

/* This subtracts the specified number of minutes from this time. The calculation wraps around midnight.
 * Params:
 *  minutesToSubtract – the minutes to subtract, may be negative
 * Returns: a time based on this time with the minutes amount subtracted*/
Time *timeMinusMinutes(Time *time, int32_t minutesToSubtract);

/* This subtracts the specified number of seconds from this time. The calculation wraps around midnight.
 * Params:
 *  secondsToSubtract – the seconds to subtract, may be negative
 * Returns: a time based on this time with the seconds amount subtracted*/
Time *timeMinusSeconds(Time *time, int32_t secondsToSubtract);

/* This subtracts the specified number of milliseconds from this time. The calculation wraps around midnight.
 * Params:
 * millisToSubtract – the millis to subtract, may be negative
 * Returns: a time based on this time with the milliseconds amount subtracted*/
Time *timeMinusMillis(Time *time, int32_t millisToSubtract);

/* Extracts the time as seconds of day, from 0 to 24 * 60 * 60 - 1.
 * Returns: the second-of-day equivalent to this time*/
uint32_t timeToSecondsOfDay(Time *time);

/* Extracts the time as milliseconds of day, from 0 to 24 * 60 * 60 * 1000 - 1.
 * Returns: the millis of day equivalent to this time*/
uint32_t timeToMillisOfDay(Time *time);

/* Compares this time to another time.
 * The comparison is based on the time-line position of the times within a day.
 * Params:
 * first - specified time
 * second – the other time to compare to
 * Returns: value, negative if less, positive if greater*/
int8_t timeCompare(const Time *first, const Time *second);

/* Checks if this time is after the specified time.
 * The comparison is based on the time-line position of the time within a day.
 * Params:
 * time – time to check
 * other – the other time to compare to
 * Returns: true if time is after the other time*/
bool isTimeAfter(Time *time, Time *other);

/* Checks if this time is before the specified time.
 * The comparison is based on the time-line position of the time within a day.
 * Params:
 * time – time to check
 * other – the other time to compare to
 * Returns: true if time is before the specified time*/
bool isTimeBefore(Time *time, Time *other);

/* Checks if this time is between two times.
 * The comparison is based on the time-line position of the time within a day.
 * Params:
 * time – time to check
 * startExclusive – the time starts from, exclusive
 * endExclusive – the end time period, exclusive
 * Returns: true if time is between two times*/
bool isTimeBetween(Time *time, Time *startExclusive, Time *endExclusive);

/* Checks if this time is equal to another time.
 * The comparison is based on the time-line position of the time within a day.
 * Params:
 * time - the time to check
 * other – the other time to check
 * Returns: true if this is equal to the other time*/
bool isTimeEquals(const Time *time, const Time *other);

/*check inner time*/
bool isTimeValid(const Time *time);