#pragma once

#include "LocalDateTime.h"

#define Z_DATE_TIME_COPY(zdt) ((ZonedDateTime){.dateTime = (zdt).dateTime, .zone = (zdt).zone, .offset = (zdt).offset})

typedef struct ZonedDateTime {
    DateTime dateTime;
    TimeZone zone;
    int32_t offset;
} ZonedDateTime;


/* Obtains the current date and from the hardware clock. Should be implemented on the driver side*/
extern ZonedDateTime zonedDateTimeNow(const TimeZone *zone);

/* Obtains an initialized ZonedDateTime struct from a year, month, day, hour, minute, second, milliseconds and time-zone.
 * This creates a zoned date-time matching the local date-time of the seven specified fields as closely as possible.
 * Time-zone rules, such as daylight savings, mean that not every local date-time is valid for the specified zone, thus the local date-time may be adjusted.
 * The local date-time is resolved to a single instant on the time-line. This is achieved by finding a valid offset from UTC/Greenwich for the local date-time as defined by the rules of the zone.
 * In most cases, there is only one valid offset for a local date-time.
 * This method uses the earlier offset typically corresponding to "summer".
 * In the case of a gap, when clocks jump forward, there is no valid offset. Instead, the local date-time is adjusted to be later by the length of the gap.
 * For a typical one hour daylight savings change, the local date-time will be moved one hour later into the offset typically corresponding to "summer".
 * This method exists primarily for writing test cases. Non test-code will typically use other methods to create an offset time.
 * DateTime has five additional convenience variants of the equivalent factory method taking fewer arguments. They are not provided here to reduce the footprint of the API.
 * Params:
 *  year – the year to represent, from MIN_YEAR to MAX_YEAR
 *  month – the month-of-year to represent, from 1 (January) to 12 (December)
 *  dayOfMonth – the day-of-month to represent, from 1 to 31
 *  hour – the hour-of-day to represent, from 0 to 23
 *  minute – the minute-of-hour to represent, from 0 to 59
 *  second – the second-of-minute to represent, from 0 to 59
 *  millis – the milliseconds to represent, from 0 to 999,999
 *  zone – the time-zone, not NULL
 * Returns: the zoned date-time*/
ZonedDateTime zonedDateTimeOf(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis, const TimeZone *zone);

/* Obtains an initialized ZonedDateTime struct from a local date and time.
 * This creates a zoned date-time matching the input date and time as closely as possible.
 * Time-zone rules, such as daylight savings, mean that not every local date-time is valid for the specified zone, thus the local date-time may be adjusted.
 * The local date time and first combined to form a local date-time. The local date-time is then resolved to a single instant on the time-line.
 * This is achieved by finding a valid offset from UTC/Greenwich for the local date-time as defined by the rules of the zone pointer.
 * In most cases, there is only one valid offset for a local date-time.
 * This method uses the earlier offset typically corresponding to "summer".
 * In the case of a gap, when clocks jump forward, there is no valid offset.
 * Instead, the local date-time is adjusted to be later by the length of the gap.
 * For a typical one hour daylight savings change, the local date-time will be moved one hour later into the offset typically corresponding to "summer".
 * Params:
 *  date – the local date
 *  time – the local time
 *  zone – the time-zone
 * Returns: the zoned date-time*/
ZonedDateTime zonedDateTimeFrom(Date *date, Time *time, const TimeZone *zone);

/* Obtains an initialized ZonedDateTime struct from a local date-time.
 * This creates a zoned date-time matching the input local date-time as closely as possible.
 * Time-zone rules, such as daylight savings, mean that not every local date-time is valid for the specified zone, thus the local date-time may be adjusted.
 * The local date-time is resolved to a single instant on the time-line. This is achieved by finding a valid offset from UTC/Greenwich for the local date-time as defined by the rules of the zone.
 * In most cases, there is only one valid offset for a local date-time.
 * This method uses the earlier offset typically corresponding to "summer".
 * In the case of a gap, when clocks jump forward, there is no valid offset.
 * Instead, the local date-time is adjusted to be later by the length of the gap.
 * For a typical one hour daylight savings change, the local date-time will be moved one hour later into the offset typically corresponding to "summer".
 * Params:
 * dateTime – the local date-time
 * zone – the time-zone, not NULL
 * Returns: the zoned date-time*/
ZonedDateTime zonedDateTimeOfDateTime(DateTime *dateTime, const TimeZone *zone);

/* Obtains an initialized ZonedDateTime struct from an Instant.
 * This creates a zoned date-time with the same instant as that specified.
 * Params:
 *  instant – the instant to create the date-time from
 *  zone – the time-zone, not NULL
 *  Returns: the zoned date-time*/
ZonedDateTime zonedDateTimeOfInstant(Instant *instant, const TimeZone *zone);

/* Returns a copy of provided zoned date-time with a different time-zone.
 * This method changes the time-zone. This normally results in a change to the local date-time.
 * This method is based on retaining the same instant.
 * Params:
 *  zone – the time-zone to change to, not NULL
 * Returns: a ZonedDateTime struct based on this date-time with the requested zone*/
ZonedDateTime zonedDateTimeWithSameInstant(ZonedDateTime *zonedDateTime, const TimeZone *zone);

/* Updates provided ZonedDateTime with the specified number of years added.
 * This operates on the local time-line, adding years to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * When converting back to ZonedDateTime, if the local date-time is in an overlap, then the offset will be retained if possible, otherwise the earlier offset will be used.
 * If in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  years – the years to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the years added*/
ZonedDateTime *zonedDateTimePlusYears(ZonedDateTime *zonedDateTime, int64_t years);

/* Updates provided ZonedDateTime with the specified number of months added.
 * This operates on the local time-line, adding months to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * If in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  months – the months to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the months added*/
ZonedDateTime *zonedDateTimePlusMonths(ZonedDateTime *zonedDateTime, int64_t months);

/* Updates provided ZonedDateTime with the specified number of weeks added.
 * This operates on the local time-line, adding weeks to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * If in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  weeks – the weeks to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the weeks added*/
ZonedDateTime *zonedDateTimePlusWeeks(ZonedDateTime *zonedDateTime, int64_t weeks);

/* Updates provided ZonedDateTime with the specified number of days added.
 * This operates on the local time-line, adding days to the local date-time.
 * When converting back to ZonedDateTime, if the local date-time is in an overlap, then the offset will be retained if possible, otherwise the earlier offset will be used.
 * If in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  days – the days to add, may be negative
 * Returns: Returns: a ZonedDateTime struct based on this date-time with the days added*/
ZonedDateTime *zonedDateTimePlusDays(ZonedDateTime *zonedDateTime, int64_t days);

/* Updates provided ZonedDateTime with the specified number of hours added.
 * This operates on the instant time-line, such that adding one hour will always be a duration of one hour later.
 * This may cause the local date-time to change by an amount other than one hour.
 *
 * For example, consider a time-zone, such as 'Europe/Paris', where the Autumn DST cutover means that the local times 02:00 to 02:59 occur twice changing from offset +02:00 in summer to +01:00 in winter.
 * Adding one hour to 01:30+02:00 will result in 02:30+02:00 (both in summer time)
 * Adding one hour to 02:30+02:00 will result in 02:30+01:00 (moving from summer to winter time)
 * Adding one hour to 02:30+01:00 will result in 03:30+01:00 (both in winter time)
 * Adding three hours to 01:30+02:00 will result in 03:30+01:00 (moving from summer to winter time)
 *
 * Params:
 *  hours – the hours to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the hours added*/
ZonedDateTime *zonedDateTimePlusHours(ZonedDateTime *zonedDateTime, int64_t hours);

/* Updates provided ZonedDateTime with the specified number of minutes added.
 * This operates on the instant time-line, such that adding one minute will always be a duration of one minute later.
 * This may cause the local date-time to change by an amount other than one minute.
 * Params:
 *  minutes – the minutes to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the minutes added*/
ZonedDateTime *zonedDateTimePlusMinutes(ZonedDateTime *zonedDateTime, int64_t minutes);

/* Updates provided ZonedDateTime with the specified number of seconds added.
 * This operates on the instant time-line, such that adding one second will always be a duration of one second later.
 * This may cause the local date-time to change by an amount other than one second.
 * Params:
 *  seconds – the seconds to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the seconds added*/
ZonedDateTime *zonedDateTimePlusSeconds(ZonedDateTime *zonedDateTime, int64_t seconds);

/* Updates provided ZonedDateTime with the specified number of milliseconds added.
 * This operates on the instant time-line, such that adding one milliseconds will always be a duration of one millisecond later.
 * This may cause the local date-time to change by an amount other than one millisecond.
 * Params:
 *  millis - the milliseconds to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the milliseconds added*/
ZonedDateTime *zonedDateTimePlusMillis(ZonedDateTime *zonedDateTime, int64_t millis);

/* Updates provided ZonedDateTime with the specified number of years subtracted.
 * This operates on the local time-line, subtracting years to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * When converting back to ZonedDateTime, if the local date-time is in an overlap, then the offset will be retained if possible, otherwise the earlier offset will be used.
 * If in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  years – the years to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the years subtracted*/
ZonedDateTime *zonedDateTimeMinusYears(ZonedDateTime *zonedDateTime, int64_t years);

/* Updates provided ZonedDateTime with the specified number of months subtracted.
 * This operates on the local time-line, subtracting months to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * When converting back to ZonedDateTime, if in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  months – the months to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the months subtracted*/
ZonedDateTime *zonedDateTimeMinusMonths(ZonedDateTime *zonedDateTime, int64_t months);

/* Updates provided ZonedDateTime with the specified number of weeks subtracted.
 * This operates on the local time-line, subtracting weeks to the local date-time.
 * This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * When converting back to ZonedDateTime, if in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  weeks – the weeks to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the weeks subtracted*/
ZonedDateTime *zonedDateTimeMinusWeeks(ZonedDateTime *zonedDateTime, int64_t weeks);

/* Updates provided ZonedDateTime with the specified number of days subtracted.
 * This operates on the local time-line, subtracting days to the local date-time. This is then converted back to a ZonedDateTime, using the zone to obtain the offset.
 * When converting back to ZonedDateTime, if in a gap, the local date-time will be adjusted forward by the length of the gap.
 * Params:
 *  days – the days to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the days subtracted*/
ZonedDateTime *zonedDateTimeMinusDays(ZonedDateTime *zonedDateTime, int64_t days);

/* Updates provided ZonedDateTime with the specified number of hours added.
 * This operates on the instant time-line, such that adding one hour will always be a duration of one hour later.
 * This may cause the local date-time to change by an amount other than one hour. Note that this is a different approach to that used by days, months and years, thus adding one day is not the same as adding 24 hours.
 * For example, consider a time-zone, such as 'Europe/Paris', where the Autumn DST cutover means that the local times 02:00 to 02:59 occur twice changing from offset +02:00 in summer to +01:00 in winter.
 * Adding one hour to 01:30+02:00 will result in 02:30+02:00 (both in summer time)
 * Adding one hour to 02:30+02:00 will result in 02:30+01:00 (moving from summer to winter time)
 * Adding one hour to 02:30+01:00 will result in 03:30+01:00 (both in winter time)
 * Adding three hours to 01:30+02:00 will result in 03:30+01:00 (moving from summer to winter time)
 * Params:
 *  hours – the hours to add, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the hours added*/
ZonedDateTime *zonedDateTimeMinusHours(ZonedDateTime *zonedDateTime, int64_t hours);

/* Updates provided ZonedDateTime with the specified number of minutes subtracted.
 * This operates on the instant time-line, such that subtracting one minute will always be a duration of one minute earlier.
 * This may cause the local date-time to change by an amount other than one minute.
 * Params:
 *  minutes – the minutes to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the minutes subtracted*/
ZonedDateTime *zonedDateTimeMinusMinutes(ZonedDateTime *zonedDateTime, int64_t minutes);

/* Updates provided ZonedDateTime with the specified number of seconds subtracted.
 * This operates on the instant time-line, such that subtracting one second will always be a duration of one second earlier.
 * This may cause the local date-time to change by an amount other than one second.
 * Params:
 *  seconds – the seconds to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the seconds subtracted*/
ZonedDateTime *zonedDateTimeMinusSeconds(ZonedDateTime *zonedDateTime, int64_t seconds);

/* Updates provided ZonedDateTime with the specified number of milliseconds subtracted.
 * This operates on the instant time-line, such that subtracting one millisecond will always be a duration of one millisecond earlier.
 * This may cause the local date-time to change by an amount other than one millisecond.
 * Params:
 *  millis - the milliseconds to subtract, may be negative
 * Returns: a ZonedDateTime struct based on this date-time with the milliseconds subtracted*/
ZonedDateTime *zonedDateTimeMinusMillis(ZonedDateTime *zonedDateTime, int64_t millis);

/* Checks if provided date-time is equal to another date-time.
 * The comparison is based on the offset date-time and the zone.
 * Params:
 *  other – the ZonedDateTime pointer to check, NULL returns false
 * Returns: true if zonedDateTime is equal to the other date-time*/
bool isZonedDateTimeEquals(ZonedDateTime *zonedDateTime, ZonedDateTime *other);