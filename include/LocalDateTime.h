#pragma once

#include "LocalDate.h"

#define DATE_TIME_COPY(dt) ((DateTime){.date = (dt).date, .time = (dt).time})

typedef struct DateTime {
    Date date;
    Time time;
} DateTime;


/* Obtains the current date and from the hardware clock. Should be implemented on the driver side*/
extern DateTime dateTimeNow();

/* Obtains an initialized DateTime struct from year, month, day, hour and minute, setting the second and milliseconds to zero.
 * The day must be valid for the year and month, otherwise an invalid date will be set. The second and millisecond fields will be set to zero.
 * Params:
 *  year – the year to represent, should be in range of YEAR_RANGE
 *  month – the month-of-year to represent
 *  dayOfMonth – the day-of-month to represent, from 1 to 31
 *  hour – the hour-of-day to represent, from 0 to 23
 *  minute – the minute-of-hour to represent, from 0 to 59
 * Returns: the local date-time*/
DateTime dateTimeOf(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes);

/* Obtains an initialized DateTime struct from year, month, day, hour, minute and second, setting the second and milliseconds to zero.
 * The day must be valid for the year and month, otherwise an invalid date will be set. The second and millisecond fields will be set to zero.
 * Params:
 *  year – the year to represent, should be in range of YEAR_RANGE
 *  month – the month-of-year to represent
 *  dayOfMonth – the day-of-month to represent, from 1 to 31
 *  hour – the hour-of-day to represent, from 0 to 23
 *  minute – the minute-of-hour to represent, from 0 to 59
 *  second – the second-of-minute to represent, from 0 to 59
 * Returns: the local date-time*/
DateTime dateTimeOfWithSeconds(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds);

/* Obtains an initialized DateTime struct from year, month, day, hour, minute, second and millisecond.
 * The day must be valid for the year and month, otherwise an invalid date will be set.
 * Params:
 *  year – the year to represent, should be in range of YEAR_RANGE
 *  month – the month-of-year to represent
 *  dayOfMonth – the day-of-month to represent, from 1 to 31
 *  hour – the hour-of-day to represent, from 0 to 23
 *  minute – the minute-of-hour to represent, from 0 to 59
 *  second – the second-of-minute to represent, from 0 to 59
 *  millis – the millis-of-second to represent, from 0 to 999
 * Returns: the local date-time*/
DateTime dateTimeOfWithMillis(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis);

/* Obtains an initialized DateTime struct from a date and time.
 * Params:
 *  date – the local date
 *  time – the local time
 * Returns: the local date-time*/
DateTime dateTimeFrom(const Date *date, const Time *time);

/* Obtains an initialized DateTime struct from an Instant and zone.
 * This creates a local date-time based on the specified instant.
 * First, the gmtOffset from UTC/Greenwich is obtained using the zone and instant, which is simple as there is only one valid gmtOffset for each instant.
 * Then, the instant and zone gmtOffset are used to calculate the local date-time.
 * Params:
 *  instant – the instant to create the date-time from
 *  zone – the time-zone
 * Returns: the local date-time*/
DateTime dateTimeOfInstant(Instant *instant, const TimeZone *zone);

/* Obtains an initialized DateTime struct using seconds from the epoch of 1970-01-01T00:00:00Z.
 * This allows the epoch-second field to be converted to a local date-time. This is primarily intended for low-level conversions rather than general application usage.
 * Params:
 *  epochSecond – the number of seconds from the epoch of 1970-01-01T00:00:00Z
 *  microsOfSecond – the microseconds within the second, from 0 to 999,999
 *  zoneSecondsOffset – the gmtOffset to use for the conversion
 * Returns: the local date-time*/
DateTime dateTimeOfEpochSeconds(int64_t epochSeconds, int64_t microsOfSecond, int32_t zoneSecondsOffset);

/* Combines provided date with the time of midnight to create a DateTime struct at the start of this date.
 * This returns a DateTime formed from provided date at the time of midnight, 00:00, at the start of this date.
 * Returns: the local date-time of midnight at the start of this date*/
DateTime dateTimeAtStartOfDay(Date *date);

/* Updates provided DateTime with the specified number of years added.
 * This method adds the specified amount to the years field in three steps:
 * Add the input years to the year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2008-02-29 (leap year) plus one year would result in the invalid date 2009-02-29 (standard year).
 * Instead of returning an invalid result, the last valid day of the month, 2009-02-28, is selected instead.
 * Params:
 *  years – the years to add, may be negative*/
DateTime *dateTimePlusYears(DateTime *dateTime, int64_t years);

/* Updates provided DateTime with the specified number of months added.
 * This method adds the specified amount to the months field in three steps:
 * Add the input months to the month-of-year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2007-03-31 plus one month would result in the invalid date 2007-04-31.
 * Instead of returning an invalid result, the last valid day of the month, 2007-04-30, is selected instead.
 * Params:
 *  months – the months to add, may be negative*/
DateTime *dateTimePlusMonths(DateTime *dateTime, int64_t months);

/* Updates provided DateTime with the specified number of weeks added.
 * This method adds the specified amount in weeks to the days field incrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2008-12-31 plus one week would result in 2009-01-07.
 * Params:
 *  weeks – the weeks to add, may be negative
 * Returns: a DateTime based on this date-time with the weeks added*/
DateTime *dateTimePlusWeeks(DateTime *dateTime, int64_t weeks);

/* Updates provided DateTime with the specified number of days added.
 * This method adds the specified amount to the days field incrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2008-12-31 plus one day would result in 2009-01-01.
 * Params:
 *  days – the days to add, may be negative
 * Returns: a DateTime based on this date-time with the days added*/
DateTime *dateTimePlusDays(DateTime *dateTime, int64_t days);

/* Updates provided DateTime with the specified number of hours added.
 * Params:
 *  hours – the hours to add, may be negative
 * Returns: a DateTime based on this date-time with the hours added*/
DateTime *dateTimePlusHours(DateTime *dateTime, int64_t hours);

/* Updates provided DateTime with the specified number of minutes added.
 * Params:
 *  minutes – the minutes to add, may be negative
 * Returns: a DateTime based on this date-time with the minutes added*/
DateTime *dateTimePlusMinutes(DateTime *dateTime, int64_t minutes);

/* Updates provided DateTime with the specified number of seconds added.
 * Params:
 *  seconds – the seconds to add, may be negative
 * Returns: a DateTime based on this date-time with the seconds added*/
DateTime *dateTimePlusSeconds(DateTime *dateTime, int64_t seconds);

/* Updates provided DateTime with the specified number of milliseconds added.
 * Params:
 *  millis – the milliseconds to add, may be negative
 * Returns: a DateTime based on this date-time with the milliseconds added*/
DateTime *dateTimePlusMillis(DateTime *dateTime, int64_t millis);

/* Updates provided DateTime with the specified number of years subtracted.
 * This method subtracts the specified amount from the years field in three steps:
 * Subtract the input years from the year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2008-02-29 (leap year) minus one year would result in the invalid date 2007-02-29 (standard year).
 * Instead of returning an invalid result, the last valid day of the month, 2007-02-28, is selected instead.
 * Params:
 *  years – the years to subtract, may be negative
 * Returns: a DateTime based on this date-time with the years subtracted*/
DateTime *dateTimeMinusYears(DateTime *dateTime, int64_t years);

/* Updates provided DateTime with the specified number of months subtracted.
 * This method subtracts the specified amount from the months field in three steps:
 * Subtract the input months from the month-of-year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2007-03-31 minus one month would result in the invalid date 2007-02-31.
 * Instead of returning an invalid result, the last valid day of the month, 2007-02-28, is selected instead.
 * Params:
 *  months – the months to subtract, may be negative
 * Returns: a DateTime based on this date-time with the months subtracted*/
DateTime *dateTimeMinusMonths(DateTime *dateTime, int64_t months);

/* Updates provided DateTime with the specified number of weeks subtracted.
 * This method subtracts the specified amount in weeks from the days field decrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2009-01-07 minus one week would result in 2008-12-31.
 * Params:
 *  weeks – the weeks to subtract, may be negative
 * Returns: a DateTime based on this date-time with the weeks subtracted*/
DateTime *dateTimeMinusWeeks(DateTime *dateTime, int64_t weeks);

/* Updates provided DateTime with the specified number of days subtracted.
 * This method subtracts the specified amount from the days field decrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2009-01-01 minus one day would result in 2008-12-31.
 * Params:
 *  days – the days to subtract, may be negative
 * Returns: a DateTime based on this date-time with the days subtracted*/
DateTime *dateTimeMinusDays(DateTime *dateTime, int64_t days);

/* Updates provided DateTime with the specified number of hours subtracted.
 * Params:
 *  hours – the hours to subtract, may be negative
 * Returns: a DateTime based on this date-time with the hours subtracted*/
DateTime *dateTimeMinusHours(DateTime *dateTime, int64_t hours);

/* Updates provided DateTime with the specified number of minutes subtracted.
 * Params:
 *  minutes – the minutes to subtract, may be negative
 * Returns: a DateTime based on this date-time with the minutes subtracted*/
DateTime *dateTimeMinusMinutes(DateTime *dateTime, int64_t minutes);

/* Updates provided DateTime with the specified number of seconds subtracted.
 * Params:
 *  seconds – the seconds to subtract, may be negative
 * Returns: a DateTime based on this date-time with the seconds subtracted*/
DateTime *dateTimeMinusSeconds(DateTime *dateTime, int64_t seconds);

/* Updates provided DateTime with the specified number of milliseconds subtracted.
 * Params:
 *  millis – the millis to subtract, may be negative
 * Returns:
 * a DateTime based on this date-time with the milliseconds subtracted*/
DateTime *dateTimeMinusMillis(DateTime *dateTime, int64_t millis);

/* Converts this DateTime to the number of seconds from the epoch of 1970-01-01T00:00:00Z.
 * This combines this local date-time and the specified offset to calculate the epoch-second value, which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
 * Instants on the time-line after the epoch are positive, earlier are negative.
 * Params:
 *  dateTime – date and time value
 *  offset – the zone offset to use for the conversion
 * Returns:
 *  the number of seconds from the epoch of 1970-01-01T00:00:00Z*/
int64_t dateTimeToEpochSecond(DateTime *dateTime, int32_t offset);

/* Compares provided DateTime date-time to another date-time.
 * The comparison is primarily based on the date-time, from earliest to latest.
 * Params:
 *  first – provided date-time
 *  second – the other date-time to compare to*/
int64_t dateTimeCompare(DateTime *first, DateTime *second);

/* Checks if provided date-time is after the specified date-time.
 * This checks to see if provided date-time represents a point on the local time-line after the other date-time.
 *          DateTime a = dateTimeOf(2012, 6, 30, 12, 0);
 *          DateTime b = dateTimeOf(2012, 7, 1, 12, 0);
 *          isDateTimeAfter(&a, &b) == false
 *          isDateTimeAfter(&a, &a) == false
 *          isDateTimeAfter(&b, &a) == true
 * This method only considers the position of the two date-times on the local time-line.
 * Params:
 *  dateTime – provided date and time value
 *  other – the other date-time to compare to
 * Returns: true if provided date-time is after the specified date-time*/
bool isDateTimeAfter(DateTime *dateTime, DateTime *other);

/* Checks if provided date-time is before the specified date-time.
 * This checks to see if provided date-time represents a point on the local time-line before the other date-time.
 *          DateTime a = dateTimeOf(2012, 6, 30, 12, 0);
 *          DateTime b = dateTimeOf(2012, 7, 1, 12, 0);
 *          isDateTimeBefore(&a, &b) == true
 *          isDateTimeBefore(&a, &a) == false
 *          isDateTimeBefore(&b, &a) == false
 * This method only considers the position of the two date-times on the local time-line.
 * Params:
 *  dateTime – provided date and time value
 *  other – the other date-time to compare to
 * Returns: true if provided date-time is before the specified date-time*/
bool isDateTimeBefore(DateTime *dateTime, DateTime *other);

/* Checks if provided date-time is between the specified start and end date-time.
 * This checks to see if provided date-time represents a point on the local time-line before the other date-time.
 *          DateTime a = dateTimeOf(2012, 6, 1, 12, 12);
 *          DateTime b = dateTimeOf(2012, 6, 1, 12, 0);
 *          DateTime c = dateTimeOf(2012, 6, 1, 12, 20);
 *          isDateTimeBetween(&a, &b, &c) == true
 * This method only considers the position of the two date-times on the local time-line.
 * Params:
 *  dateTime – provided date and time value
 *  startDateInclusive – the start date, exclusive
 *  endDateExclusive – the end date, exclusive
 * Returns: true if the date between this date and the end date*/
bool isDateTimeBetween(DateTime *dateTime, DateTime *startExclusive, DateTime *endExclusive);

/* Checks if provided date-time is equal to another date-time.
 * Params:
 *  dateTime - provided date and time value
 *  other – the other date-time to check
 * Returns: true if provided is equal to the other date-time*/
bool isDateTimeEquals(DateTime *dateTime, DateTime *other);

bool isDateTimeValid(DateTime *dateTime);