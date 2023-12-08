#include "LocalDate.h"

#define UNINITIALIZED_DATE ((Date){.year = -1, .month = 0, .day = -1})

static Date * setDate(Date *date, int64_t year, Month month, uint8_t dayOfMonth);
static bool isProvidedDateValid(int64_t year, Month month, uint8_t dayOfMonth);
static Date * resolvePreviousValidDate(Date *date, int64_t year, Month month, uint8_t day);
static Month monthPlus(Month month, uint8_t months);


Date dateOf(int64_t year, Month month, uint8_t dayOfMonth) {
    Date date = UNINITIALIZED_DATE;
    return isProvidedDateValid(year, month, dayOfMonth) ? *setDate(&date, year, month, dayOfMonth) : date;
}

Date dateOfEpochDay(int64_t epochDay) {
    if (isValidValue(&EPOCH_DAY_RANGE, epochDay)) {
        int64_t zeroDay = epochDay + DAYS_0000_TO_1970;
        // find the march-based year
        zeroDay -= 60;  // adjust to 0000-03-01 so leap day is at end of four-year cycle
        int64_t adjust = 0;
        if (zeroDay < 0) {
            // adjust negative years to positive for calculation
            int64_t adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
            adjust = adjustCycles * 400;
            zeroDay += -adjustCycles * DAYS_PER_CYCLE;
        }
        int64_t yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
        int64_t doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
        if (doyEst < 0) {
            yearEst--;// fix estimate
            doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
        }
        yearEst += adjust;  // reset any negative year
        uint32_t marchDoy0 = (uint32_t) doyEst;

        // convert march-based values back to january-based
        uint32_t marchMonth0 = (marchDoy0 * 5 + 2) / 153;
        uint32_t month = (marchMonth0 + 2) % 12 + 1;
        uint32_t dayOfMonth = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
        yearEst += marchMonth0 / 10;

        if (isValidValue(&YEAR_RANGE, yearEst)) {
            return dateOf(yearEst, month, dayOfMonth);
        }
    }
    return UNINITIALIZED_DATE;
}

Date dateOfYearDay(uint32_t year, uint32_t dayOfYear) {
    if (isValidValue(&YEAR_RANGE, year) && isValidValue(&DAY_OF_YEAR_RANGE, dayOfYear)) {
        bool leap = isLeapYear(year);
        if (dayOfYear == DAY_OF_YEAR_RANGE.max && !leap) return UNINITIALIZED_DATE; // Invalid date 366 as this year is not a leap year

        Month month = (dayOfYear - 1) / 31 + 1;
        if (!isValidValue(&MONTH_OF_YEAR_RANGE, month)) return UNINITIALIZED_DATE; // Invalid value for Month
        uint32_t monthEnd = firstDayOfYearByMonth(month, leap) + lengthOfMonth(month, leap) - 1;
        if (dayOfYear > monthEnd) {
            month = monthPlus(month, 1);
        }
        uint32_t dayOfMonth = dayOfYear - firstDayOfYearByMonth(month, leap) + 1;
        return dateOf(year, month, dayOfMonth);
    }
    return UNINITIALIZED_DATE;
}

Date dateOfInstant(Instant *instant, const TimeZone *zone) {
    if (instant != NULL && zone != NULL) {
        int64_t localSeconds = instant->seconds + zone->utcOffset;
        int64_t localEpochDay = floorDiv(localSeconds, SECONDS_PER_DAY);
        return dateOfEpochDay(localEpochDay);
    }
    return UNINITIALIZED_DATE;
}

int64_t dateToEpochDay(Date *date) {
    if (!isDateValid(date)) return INT64_MIN;
    int64_t year = date->year;
    int64_t month = date->month;
    int64_t total = 0;
    total += 365 * year;
    if (year >= 0) {
        total += (year + 3) / 4 - (year + 99) / 100 + (year + 399) / 400;
    } else {
        total -= year / -4 - year / -100 + year / -400;
    }
    total += ((367 * month - 362) / 12);
    total += date->day - 1;
    if (month > 2) {
        total--;
        if (!isLeapYear(date->year)) {
            total--;
        }
    }
    return total - DAYS_0000_TO_1970;
}

int64_t dateToEpochSeconds(Date *date, Time *time, const TimeZone *zone) {
    if (date != NULL && time != NULL && zone != NULL) {
        int64_t seconds = dateToEpochDay(date) * SECONDS_PER_DAY + timeToSecondsOfDay(time);
        seconds -= zone->utcOffset;
        return seconds;
    }
    return 0;
}

Date *datePlusYears(Date *date, int64_t yearsToAdd) {
    if (yearsToAdd != 0 && isDateValid(date) && isValidValue(&YEAR_RANGE, date->year + yearsToAdd)) {
        return resolvePreviousValidDate(date, date->year + yearsToAdd, date->month, date->day);
    }
    return date;
}

Date *datePlusMonths(Date *date, int64_t monthsToAdd) {
    if (monthsToAdd == 0 || !isDateValid(date)) {
        return date;
    }
    int64_t monthCount = date->year * 12 + (date->month - 1);
    int64_t calcMonths = monthCount + monthsToAdd;
    int64_t newYear = floorDiv(calcMonths, 12);
    if (isValidValue(&YEAR_RANGE, newYear)) {
        int64_t newMonth = floorMod(calcMonths, 12) + 1;
        return resolvePreviousValidDate(date, newYear, newMonth, date->day);
    }
    return date;
}

Date *datePlusWeeks(Date *date, int64_t weeksToAdd) {
    return datePlusDays(date, multiplyExact(weeksToAdd, 7));
}

Date *datePlusDays(Date *date, int64_t daysToAdd) {
    if (!isDateValid(date)) return date;
    int64_t totalDays = date->day + daysToAdd;
    if (totalDays > 0) {
        if (totalDays <= 28) {
            return setDate(date, date->year, date->month, totalDays);

        } else if (totalDays <= 59) { // 59th Jan is 28th Feb, 59th Feb is 31st Mar
            uint8_t monthLength = lengthOfMonth(date->month, isLeapYear(date->year));
            if (totalDays <= monthLength) {
                return setDate(date, date->year, date->month, totalDays);

            } else if (date->month < 12) {
                return setDate(date, date->year, date->month + 1, totalDays - monthLength);

            } else {
                if (isValidValue(&YEAR_RANGE, date->year + 1)) {
                    return setDate(date, date->year + 1, 1, (totalDays - monthLength));
                }
            }
        }
    }

    int64_t mjDay = dateToEpochDay(date) + daysToAdd;
    Date tmpDate = dateOfEpochDay(mjDay);
    if (isDateValid(&tmpDate)) {
        date->year = tmpDate.year;
        date->month = tmpDate.month;
        date->day = tmpDate.day;
        date->weekDay = tmpDate.weekDay;
    }
    return date;
}

Date *dateMinusYears(Date *date, int64_t yearsToSubtract) {
    return datePlusYears(date, -yearsToSubtract);
}

Date *dateMinusMonths(Date *date, int64_t monthsToSubtract) {
    return datePlusMonths(date, -monthsToSubtract);
}

Date *dateMinusWeeks(Date *date, int64_t weeksToSubtract) {
    return datePlusWeeks(date, -weeksToSubtract);
}

Date *dateMinusDays(Date *date, int64_t daysToSubtract) {
    return datePlusDays(date, -daysToSubtract);
}

int64_t dateCompare(const Date *first, const Date *second) {
    if (!isDateValid(first) || !isDateValid(second)) return INT8_MIN;
    int64_t compare = (first->year - second->year);
    if (compare == 0) {
        compare = ((int8_t) first->month - (int8_t) second->month);
        if (compare == 0) {
            compare = (first->day - second->day);
        }
    }
    return compare;
}

bool isDateAfter(Date *date, Date *other) {
    return dateCompare(date, other) > 0;
}

bool isDateBefore(Date *date, Date *other) {
    return dateCompare(date, other) < 0;
}

bool isDateBetween(Date *date, Date *startExclusive, Date *endExclusive) {
    return isDateAfter(date, startExclusive) && isDateBefore(date, endExclusive);
}

bool isDateEquals(const Date *date, const Date *other) {
    return dateCompare(date, other) == 0;
}

uint16_t getDayOfYear(Date *date) {
    if (!isDateValid(date)) return 0;
    return firstDayOfYearByMonth(date->month, isLeapYear(date->year)) + date->day - 1;
}

DayOfWeek getDayOfWeek(Date *date) {            // find day of week from a given date
    if (!isDateValid(date)) return 0;
    int64_t dayOfWeek = floorMod(dateToEpochDay(date) + 3, 7);
    return dayOfWeek + 1;
}

uint8_t getWeekOfYear(Date *date) {
    return ((getDayOfYear(date) - 1) / 7) + 1;
}

uint8_t getWeekOfMonth(Date *date) {
    if (!isDateValid(date)) return 0;
    return ((date->day - 1) / 7) + 1;
}

DayOfWeek dayOfWeekPlus(DayOfWeek dayOfWeek, int32_t days) {
    int32_t amount = days % 7;
    return (((dayOfWeek - 1) + (amount + 7)) % 7) + 1;
}

uint16_t getLengthOfYear(int64_t year) {
    return isLeapYear(year) ? 366 : 365;
}

bool isLeapYear(int64_t year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

uint32_t firstDayOfYearByMonth(Month month, bool leapYear) {
    switch (month) {
        case JANUARY:
            return 1;
        case FEBRUARY:
            return 32;
        case MARCH:
            return 60 + leapYear;
        case APRIL:
            return 91 + leapYear;
        case MAY:
            return 121 + leapYear;
        case JUNE:
            return 152 + leapYear;
        case JULY:
            return 182 + leapYear;
        case AUGUST:
            return 213 + leapYear;
        case SEPTEMBER:
            return 244 + leapYear;
        case OCTOBER:
            return 274 + leapYear;
        case NOVEMBER:
            return 305 + leapYear;
        case DECEMBER:
        default:
            return 335 + leapYear;
    }
}

int8_t lengthOfMonth(Month month, bool leapYear) {
    switch (month) {
        case FEBRUARY:
            return leapYear ? 29 : 28;
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            return 30;
        case JANUARY:
        case MARCH:
        case MAY:
        case JULY:
        case AUGUST:
        case OCTOBER:
        case DECEMBER:
            return 31;
        default:
            return -1;
    }
}

bool isDateValid(const Date *date) {
    return date != NULL && isProvidedDateValid(date->year, date->month, date->day);
}

static Date *setDate(Date *date, int64_t year, Month month, uint8_t dayOfMonth) {
    date->year = year;
    date->month = month;
    date->day = (int8_t) dayOfMonth;
    date->weekDay = getDayOfWeek(date);
    return date;
}

static bool isProvidedDateValid(int64_t year, Month month, uint8_t dayOfMonth) {
    if (isValidValue(&YEAR_RANGE, year) && isValidValue(&MONTH_OF_YEAR_RANGE, month) && isValidValue(&DAY_OF_MONTH_RANGE, dayOfMonth)) {
        if (dayOfMonth > 28) {
            uint8_t dom = 31;
            switch (month) {
                case FEBRUARY:
                    dom = isLeapYear(year) ? 29 : 28;
                    break;
                case APRIL:
                case JUNE:
                case SEPTEMBER:
                case NOVEMBER:
                    dom = 30;
                    break;
                default:
                    break;
            }
            if (dayOfMonth > dom) return false;
            return true;
        }
        return true;
    }
    return false;
}


static Date *resolvePreviousValidDate(Date *date, int64_t year, Month month, uint8_t day) {
    switch (month) {
        case FEBRUARY:
            day = minInt64(day, isLeapYear(year) ? 29 : 28);
            break;
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            day = minInt64(day, 30);
            break;
        default:
            break;
    }
    return setDate(date, year, month, day);
}

static Month monthPlus(Month month, uint8_t months) {
    uint8_t amount = months % 12;
    return (((month - 1) + (amount + 12)) % 12) + 1;
}