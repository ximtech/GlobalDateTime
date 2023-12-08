#pragma once

#include "LocalTime.h"

/*The number of days in a 400 year cycle.*/
#define DAYS_PER_CYCLE 146097L
/*The number of days from year zero to year 1970. There are five 400 year cycles from year zero to 2000. There are 7 leap years from 1970 to 2000.*/
#define DAYS_0000_TO_1970 ((DAYS_PER_CYCLE * 5L) - (30L * 365L + 7L))

#define DATE_COPY(date) ((Date){.day = (date).day, .month = (date).month, .year = (date).year, .weekDay = (date).weekDay})

static const char *const WEEK_DAY_NAME_SHORT[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
static const char *const WEEK_DAY_NAME_LONG[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
static const char *const MONTH_NAME_SHORT[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static const char *const MONTH_NAME_LONG[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static const char *const ISO_ERA_NAME_SHORT[] = {"CE", "BCE"};
static const char *const ISO_ERA_NAME_LONG[] = {"Current era", "Before current era"};

typedef enum DayOfWeek {
    MONDAY    = 1,
    TUESDAY	  = 2,
    WEDNESDAY = 3,
    THURSDAY  = 4,
    FRIDAY    = 5,
    SATURDAY  = 6,
    SUNDAY    = 7,
} DayOfWeek;

typedef enum Month {
    JANUARY   = 1,
    FEBRUARY  = 2,
    MARCH	  = 3,
    APRIL	  = 4,
    MAY		  = 5,
    JUNE	  = 6,
    JULY	  = 7,
    AUGUST	  = 8,
    SEPTEMBER = 9,
    OCTOBER   = 10,
    NOVEMBER  = 11,
    DECEMBER  = 12
} Month;

typedef struct Date {
    int8_t day;
    Month month;
    int64_t year;
    DayOfWeek weekDay;
} Date;

/* Obtains the current date from the hardware clock. Should be implemented on the driver side*/
extern Date dateNow();

/* The minimum supported Date, '-999999999-01-01'. This could be used by an application as a "far past" date.
 * And the maximum supported Date, '+999999999-12-31'. This could be used by an application as a "far future" date. */
Date dateOf(int64_t year, Month month, uint8_t dayOfMonth);

/* Return initialized Date structure from the epoch day count.
 * The result is initialized Date with the specified epoch-day.
 * The EPOCH_DAY is a simple incrementing count of days where day 0 is 1970-01-01. Negative numbers represent earlier days.
 * Params:
 *  epochDay – the Epoch Day to convert, based on the epoch 1970-01-01*/
Date dateOfEpochDay(int64_t epochDay);

/* Return initialized Date structure from a year and day-of-year.
 * The result is initialized Date with the specified year and day-of-year. The day-of-year must be valid for the year.
 * Params:
 *  year – the year to represent, from MIN_YEAR to MAX_YEAR
 *  dayOfYear – the day-of-year to represent, from 1 to 366 */
Date dateOfYearDay(uint32_t year, uint32_t dayOfYear);

/* Return initialized Date structure from an Time instant and zone.
 * This creates a date based on the specified instant.
 * First, the gmtOffset from UTC/Greenwich is obtained using the zone and instant, which is simple as there is only one valid gmtOffset for each instant.
 * Then, the instant and gmtOffset are used to calculate the local date.
 * Params:
 *  instant – the instant to create the date from
 *  zone – the time-zone, which may be an gmtOffset*/
Date dateOfInstant(Instant *instant, const TimeZone *zone);

/* Converts Date to the Epoch Day.
 * The Epoch Day count is a simple incrementing count of days where day 0 is 1970-01-01 (ISO). This definition is the same for all chronologies, enabling conversion.
 * Returns: the Epoch Day equivalent to this date*/
int64_t dateToEpochDay(Date *date);

/* Converts Date to the number of seconds since the epoch of 1970-01-01T00:00:00Z.
 * This combines this date with the specified time and gmtOffset to calculate the epoch-second value, which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
 * Instants on the time-line after the epoch are positive, earlier are negative.
 * Params:
 *  time – the local time
 *  zone – the zone pointer
 * Returns: the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative*/
int64_t dateToEpochSeconds(Date *date, Time *time, const TimeZone *zone);

/* This method adds the specified amount to the years field in three steps:
 * Add the input years to the year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2008-02-29 (leap year) plus one year would result in the invalid date 2009-02-29 (standard year).
 * Instead of returning an invalid result, the last valid day of the month, 2009-02-28, is selected instead.
 * Params:
    * yearsToAdd – the years to add, may be negative*/
Date * datePlusYears(Date *date, int64_t yearsToAdd);

/* This method adds the specified amount to the months field in three steps:
 * Add the input months to the month-of-year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2007-03-31 plus one month would result in the invalid date 2007-04-31.
 * Instead of returning an invalid result, the last valid day of the month, 2007-04-30, is selected instead.
 * Params:
    * monthsToAdd – the months to add, may be negative*/
Date * datePlusMonths(Date *date, int64_t monthsToAdd);

/* This method adds the specified amount in weeks to the days field incrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2008-12-31 plus one week would result in 2009-01-07.
 * Params:
    * weeksToAdd – the weeks to add, may be negative*/
Date * datePlusWeeks(Date *date, int64_t weeksToAdd);

/* This method adds the specified amount to the days field incrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2008-12-31 plus one day would result in 2009-01-01.
 * Params:
    * daysToAdd – the days to add, may be negative*/
Date * datePlusDays(Date *date, int64_t daysToAdd);

/* This method subtracts the specified amount from the years field in three steps:
 * Subtract the input years from the year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2008-02-29 (leap year) minus one year would result in the invalid date 2007-02-29 (standard year).
 * Instead of returning an invalid result, the last valid day of the month, 2007-02-28, is selected instead.
 * Params:
    * yearsToSubtract – the years to subtract, may be negative*/
Date * dateMinusYears(Date *date, int64_t yearsToSubtract);

/* This method subtracts the specified amount from the months field in three steps:
 * Subtract the input months from the month-of-year field
 * Check if the resulting date would be invalid
 * Adjust the day-of-month to the last valid day if necessary
 * For example, 2007-03-31 minus one month would result in the invalid date 2007-02-31.
 * Instead of returning an invalid result, the last valid day of the month, 2007-02-28, is selected instead.
 * Params:
    * monthsToSubtract – the months to subtract, may be negative*/
Date * dateMinusMonths(Date *date, int64_t monthsToSubtract);

/* This method subtracts the specified amount in weeks from the days field decrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2009-01-07 minus one week would result in 2008-12-31.
 * Params:
    * weeksToSubtract – the weeks to subtract, may be negative*/
Date * dateMinusWeeks(Date *date, int64_t weeksToSubtract);

/* This method subtracts the specified amount from the days field decrementing the month and year fields as necessary to ensure the result remains valid.
 * The result is only invalid if the maximum/minimum year is exceeded.
 * For example, 2009-01-01 minus one day would result in 2008-12-31.
 * Params:
    * daysToSubtract – the days to subtract, may be negative*/
Date * dateMinusDays(Date *date, int64_t daysToSubtract);

/* Compares this date to another date.
 * The comparison is primarily based on the date, from earliest to latest.
 * Params:
    * first – input date to compare to
    * second – the other date to compare to
 * Returns:
    * value, negative if less, positive if greater*/
int64_t dateCompare(const Date *first, const Date *second);

/* Checks if this date is after the specified date.
This checks to see if this date represents a point on the time-line after the other date.
 *         Date a = dateOf(2012, 6, 30);
 *         Date b = dateOf(2012, 7, 1);
 *         isDateAfter(&a, &b) == false
 *         isDateAfter(&a, &a) == false
 *         isDateAfter(&b, &a) == true
 * This method only considers the position of the two dates on the local time-line.
 * It does not take into account the chronology, or calendar system.
 * Params:
 *  date - specified date
 *  other – the other date to compare to
 * Returns: true if this date is after the specified date*/
bool isDateAfter(Date *date, Date *other);

/* Checks if this date is before the specified date.
 * This checks to see if this date represents a point on the time-line before the other date.
 *         Date a = dateOf(2012, 6, 30);
 *         Date b = dateOf(2012, 7, 1);
 *         isDateBefore(&a, &b) == true
 *         isDateBefore(&a, &a) == false
 *         isDateBefore(&b, &a) == false
 * This method only considers the position of the two dates on the time-line.
 * Params:
 *  date - specified date
 *  other – the other date to compare to
 * Returns: true if this date is before the specified date*/
bool isDateBefore(Date *date, Date *other);

/* Checks if this date is between two days.
 * This checks to see if this date represents a point on the time-line between the other two dates.
 *         Date a = dateOf(2012, 6, 30);
 *         Date b = dateOf(2012, 6, 28);
 *         Date c = dateOf(2012, 7, 1);
 *         isDateBetween(&a, &b, &c) == true
 * This method only considers the position of the two dates on the time-line.
 * Params:
 *  date - specified date
 *  startExclusive – from date, exclusive
 *  endExclusive – up to date, exclusive
 * Returns: true if this date is between two specified dates*/
bool isDateBetween(Date *date, Date *startExclusive, Date *endExclusive);

/* Checks if this date is equal to the specified date.
 * This checks to see if this date represents the same point on the local time-line as the other date.
 *         Date a = dateOf(2012, 6, 30);
 *         Date b = dateOf(2012, 7, 1);
 *         isDatesEqual(&a, &b) == false
 *         isDatesEqual(&a, &a) == true
 *         isDateEquals(&b, &a) == false
 * This method only considers the position of the two dates on the local time-line.
 * Params:
 *  date - specified date
 *  other – the other date to compare to
 * Returns: true if this date is equal to the specified date*/
bool isDateEquals(const Date *date, const Date *other);

/* Gets the day-of-year field.
 * This method returns int value for the day-of-year.
 * Returns: the day-of-year, from 1 to 365, or 366 in a leap year*/
uint16_t getDayOfYear(Date *date);

/* Gets the day-of-week field, which is an enum DayOfWeek.
 * This method returns the enum DayOfWeek for the day-of-week.
 * This avoids confusion as to what int values mean.
 * Returns: the day-of-week*/
DayOfWeek getDayOfWeek(Date *date);

uint8_t getWeekOfYear(Date *date);
uint8_t getWeekOfMonth(Date *date);

/* Returns the day-of-week that is the specified number of days after this one.
 * The calculation rolls around the end of the week from Sunday to Monday. The specified period may be negative.
 * Params:
 *  dayOfWeek – the day of week to add
 *  days – the days to add, positive or negative
 * Returns: the resulting day-of-week*/
DayOfWeek dayOfWeekPlus(DayOfWeek dayOfWeek, int32_t days);

/* Returns the length of the year represented by this date.
 * This returns the length of the year in days, either 365 or 366.
 * Returns: 366 if the year is leap, 365 otherwise*/
uint16_t getLengthOfYear(int64_t year);

/* Checks if the year is a leap year, according to the ISO proleptic calendar system rules.
 * This method applies the current rules for leap years across the whole time-line.
 * In general, a year is a leap year if it is divisible by four without remainder.
 * However, years divisible by 100, are not leap years, with the exception of years divisible by 400 which are.
 * For example, 1904 is a leap year it is divisible by 4. 1900 was not a leap year as it is divisible by 100, however 2000 was a leap year as it is divisible by 400.
 * The calculation is proleptic - applying the same rules into the far future and far past. This is historically inaccurate, but is correct for the ISO-8601 standard.
 * Returns:
 * true if the year is leap, false otherwise*/
bool isLeapYear(int64_t year);

uint32_t firstDayOfYearByMonth(Month month, bool leapYear);
int8_t lengthOfMonth(Month month, bool leapYear);
bool isDateValid(const Date *date);


static inline const char *getWeekDayNameShort(DayOfWeek weekDay) {
    return WEEK_DAY_NAME_SHORT[weekDay - 1];
}

static inline const char *getWeekDayNameLong(DayOfWeek weekDay) {
    return WEEK_DAY_NAME_LONG[weekDay - 1];
}

static inline const char *getMonthNameShort(Month month) {
    return MONTH_NAME_SHORT[month - 1];
}

static inline const char *getMonthNameLong(Month month) {
    return MONTH_NAME_LONG[month - 1];
}

static inline const char *getIsoEraShort(int64_t year) {
    return year >= 1 ? ISO_ERA_NAME_SHORT[0] : ISO_ERA_NAME_SHORT[1];
}

static inline const char *getIsoEraLong(int64_t year) {
    return year >= 1 ? ISO_ERA_NAME_LONG[0] : ISO_ERA_NAME_LONG[1];
}

