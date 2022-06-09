#pragma once

#include "BaseTestTemplate.h"
#include "Date.h"


static Date TEST_2007_07_15 = {.year = 2007, .month = 7, .day = 15, .weekDay = SUNDAY};

static Date COMPARISON_DATE_ARRAY[] = {
        {.year = -9999999, .month = 1, . day = 1},
        {.year = -9999999, .month = 12, . day = 31},
        {.year = -1, .month = 1, . day = 1},
        {.year = -1, .month = 12, . day = 31},
        {.year = 0, .month = 1, . day = 1},
        {.year = 0, .month = 12, .day = 31},
        {.year = 1, .month = 1, .day = 1},
        {.year = 1, .month = 12, .day = 31},
        {.year = 2006, .month = 1, .day = 1},
        {.year = 2006, .month = 12, .day = 31},
        {.year = 2007, .month = 1, .day = 1},
        {.year = 2007, .month = 12, .day = 31},
        {.year = 2008, .month = 1, .day = 1},
        {.year = 2008, .month = 2, .day = 29},
        {.year = 2008, .month = 12, .day = 31},
        {.year = 9999999, .month = 1, . day = 1},
        {.year = 9999999, .month = 12, . day = 31},
};


static inline void assertDatesEquals(Date test, Date expected) {
    assert_true(isDateEquals(&test, &expected));
    assert_int(test.weekDay, ==, expected.weekDay);
}

static MunitResult testDateFactory(const MunitParameter params[], void *data) {
    // valid date
    Date test = dateOf(2007, JULY, 15);
    assert_true(isDateEquals(&TEST_2007_07_15, &test));
    assert_int(test.weekDay, ==, SUNDAY);

    // not valid dates
    test = dateOf(2007, FEBRUARY, 29);
    assert_false(isDateValid(&test));

    test = dateOf(2007, APRIL, 31);
    assert_false(isDateValid(&test));

    test = dateOf(2007, JANUARY, 0);
    assert_false(isDateValid(&test));

    test = dateOf(2007, JANUARY, 32);
    assert_false(isDateValid(&test));

    test = dateOf(2007, 0, 30);
    assert_false(isDateValid(&test));

    test = dateOf(YEAR_RANGE.max + 1, JANUARY, 1);
    assert_false(isDateValid(&test));

    test = dateOf(YEAR_RANGE.min - 1, JANUARY, 1);
    assert_false(isDateValid(&test));
    return MUNIT_OK;
}

static MunitResult testDateOfInstantFactory(const MunitParameter params[], void *data) {
    Instant instant = instantOfEpochSeconds(1653756098);
    Date test = dateOfInstant(&instant, &UTC);
    assertDatesEquals(test, dateOf(2022, MAY, 28));

    instant = EPOCH;
    test = dateOfInstant(&instant, &UTC);
    assertDatesEquals(test, dateOf(1970, JANUARY, 1));

    test = dateOfInstant(NULL, &UTC);
    assert_false(isDateValid(&test));
    return MUNIT_OK;
}

static MunitResult testDateOfEpochDay(const MunitParameter params[], void *data) {
    long date_0000_01_01 = -678941 - 40587;
    Date minDate = dateOf(YEAR_RANGE.min, 1, 1);
    Date maxDate = dateOf(YEAR_RANGE.max, 12, 31);
    int64_t minValidEpochDays = dateToEpochDay(&minDate);
    int64_t maxValidEpochDays = dateToEpochDay(&maxDate);

    assertDatesEquals(dateOfEpochDay(0), dateOf(1970, 1, 1));
    assertDatesEquals(dateOfEpochDay(date_0000_01_01), dateOf(0, 1, 1));
    assertDatesEquals(dateOfEpochDay(maxValidEpochDays), maxDate);
    assertDatesEquals(dateOfEpochDay(minValidEpochDays), minDate);

    // Since plusDays/minusDays actually depends on MJDays, it cannot be used for testing
    Date test = dateOf(0, 1, 1);
    for (long i = date_0000_01_01; i < 700000; i++) {
        Date epochDate = dateOfEpochDay(i);
        assertDatesEquals(epochDate, test);

        int8_t newDayOfMonth = (int8_t) (test.day + 1);
        uint8_t monthLength = lengthOfMonth(test.month, isLeapYear(test.year));
        if (newDayOfMonth <= monthLength) {
            test = dateOf(test.year, test.month, newDayOfMonth);
            continue;
        }
        test.day = 1;
        datePlusMonths(&test, 1);
    }

    // test decrement from max year
    long long date_9999999_1_1 = 3651705107;
    long long till_date_9999916_11_12 = 3651675107;
    test = dateOf(9999999, 1, 1);
    for (long long i = date_9999999_1_1; i > till_date_9999916_11_12; i--) {
        Date epochDate = dateOfEpochDay(i);
        assertDatesEquals(epochDate, test);

        int8_t newDayOfMonth = (int8_t) (test.day - 1);
        if (newDayOfMonth > 0) {
            test = dateOf(test.year, test.month, newDayOfMonth);
            continue;
        }
        dateMinusMonths(&test, 1);
        int8_t monthLength = lengthOfMonth(test.month, isLeapYear(test.year));
        test = dateOf(test.year, test.month, monthLength);
    }
    return MUNIT_OK;
}

static MunitResult testDateOfYearDay(const MunitParameter params[], void *data) {
    // non leap year
    Date test = dateOf(2007, 1, 1);
    for (int i = 1; i < 365; i++) {
        Date dateFromYearDay = dateOfYearDay(2007, i);
        assertDatesEquals(dateFromYearDay, test);
        datePlusDays(&test, 1);
    }

    // leap year
    test = dateOf(2008, 1, 1);
    for (int i = 1; i < 366; i++) {
        Date dateFromYearDay = dateOfYearDay(2008, i);
        assertDatesEquals(dateFromYearDay, test);
        datePlusDays(&test, 1);
    }

    Date invalidDate = dateOfYearDay(2008, 455);
    assert_false(isDateValid(&invalidDate));
    invalidDate = dateOfYearDay(YEAR_RANGE.max + 1, 23);
    assert_false(isDateValid(&invalidDate));
    return MUNIT_OK;
}

static MunitResult testDateToEpochDay(const MunitParameter params[], void *data) {
    Date test = dateOf(1858, 11, 17);
    assert_int64(dateToEpochDay(&test), ==, -40587);
    test = dateOf(1, 1, 1);
    assert_int64(dateToEpochDay(&test), ==, -678575 - 40587);
    test = dateOf(1995, 9, 27);
    assert_int64(dateToEpochDay(&test), ==, 49987 - 40587);
    test = dateOf(1970, 1, 1);
    assert_int64(dateToEpochDay(&test), ==, 0);
    return MUNIT_OK;
}

static MunitResult testDateToEpochSeconds(const MunitParameter params[], void *data) {
    Date testDate = dateOf(1858, 11, 17);
    Time testTime = timeOf(12, 23, 45);
    assert_int64(dateToEpochSeconds(&testDate, &testTime, &UTC), ==, -3506672175);

    testDate = dateOf(1970, 1, 1);
    testTime = timeOf(0, 0, 0);
    assert_int64(dateToEpochSeconds(&testDate, &testTime, &UTC), ==, 0);

    testDate = dateOf(1995, 9, 27);
    testTime = timeOf(17, 23, 13);
    assert_int64(dateToEpochSeconds(&testDate, &testTime, &UTC), ==, 812222593);

    assert_int64(dateToEpochSeconds(NULL, NULL, NULL), ==, 0);
    return MUNIT_OK;
}

static MunitResult testDatePlusYears(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    datePlusYears(&test, 1); // normal
    assertDatesEquals(test, dateOf(2008, 7, 15));

    test = TEST_2007_07_15;
    datePlusYears(&test, -1);  // negative
    assertDatesEquals(test, dateOf(2006, 7, 15));

    test = dateOf(2008, 2, 29);     // adjust day
    datePlusYears(&test, 1);
    assertDatesEquals(test, dateOf(2009, 2, 28));

    int64_t years = 20 + YEAR_RANGE.max;
    test = dateOf(-40, 6, 1);
    datePlusYears(&test, years); // big value
    assertDatesEquals(test, dateOf((-40L + years), 6, 1));

    test = dateOf(YEAR_RANGE.max, 6, 1);
    assert_true(isDateValid(&test));
    datePlusYears(&test, YEAR_RANGE.max); // too large value
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 6, 1));

    test = dateOf(YEAR_RANGE.min, 6, 1);
    assert_true(isDateValid(&test));
    datePlusYears(&test, -1); // too small valid year
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 6, 1));
    return MUNIT_OK;
}

static MunitResult testDatePlusMonths(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    datePlusMonths(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 8, 15));

    test = TEST_2007_07_15;
    datePlusMonths(&test, 25); // over years
    assertDatesEquals(test, dateOf(2009, 8, 15));

    test = TEST_2007_07_15;
    datePlusMonths(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 6, 15));

    test = TEST_2007_07_15;
    datePlusMonths(&test, -7); // negative across year
    assertDatesEquals(test, dateOf(2006, 12, 15));

    test = TEST_2007_07_15;
    datePlusMonths(&test, -31); // negative over years
    assertDatesEquals(test, dateOf(2004, 12, 15));

    test = dateOf(2008, 2, 29);
    datePlusMonths(&test, 12); // adjust day from leap year
    assertDatesEquals(test, dateOf(2009, 2, 28));

    test = dateOf(2007, 3, 31);
    datePlusMonths(&test, 1); // adjust day from month length
    assertDatesEquals(test, dateOf(2007, 4, 30));

    test = dateOf(YEAR_RANGE.max, 12, 1);
    assert_true(isDateValid(&test));
    datePlusMonths(&test, 1); // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 1));
    return MUNIT_OK;
}

static MunitResult testDatePlusWeeks(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    datePlusWeeks(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 7, 22));

    test = TEST_2007_07_15;
    datePlusWeeks(&test, 9); // over months
    assertDatesEquals(test, dateOf(2007, 9, 16));

    test = dateOf(2006, 7, 16);
    datePlusWeeks(&test, 52);           // over years
    assertDatesEquals(test, TEST_2007_07_15);

    test = TEST_2007_07_15;
    datePlusYears(&test, -1);
    datePlusWeeks(&test, 104);           // over leap years
    assertDatesEquals(test, dateOf(2008, 7, 12));

    test = TEST_2007_07_15;
    datePlusWeeks(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 7, 8));

    test = TEST_2007_07_15;
    datePlusWeeks(&test, -28); // negative across year
    assertDatesEquals(test, dateOf(2006, 12, 31));

    test = TEST_2007_07_15;
    datePlusWeeks(&test, -104); // negative over years
    assertDatesEquals(test, dateOf(2005, 7, 17));

    test = dateOf(YEAR_RANGE.max, 12, 24);
    assert_true(isDateValid(&test));
    datePlusWeeks(&test, 1);         // maximum
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 8);
    assert_true(isDateValid(&test));
    datePlusWeeks(&test, -1);         // minimum
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));

    test = dateOf(YEAR_RANGE.max, 12, 25);
    assert_true(isDateValid(&test));
    datePlusWeeks(&test, 1);         // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 25));

    test = dateOf(YEAR_RANGE.min, 1, 7);
    assert_true(isDateValid(&test));
    datePlusWeeks(&test, -1);         // too small
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 7));
    return MUNIT_OK;
}

static MunitResult testDatePlusDays(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    datePlusDays(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 7, 16));

    test = dateOf(2007, 12, 31);
    datePlusDays(&test, 2);     // over year
    assertDatesEquals(test, dateOf(2008, 1, 2));

    test = TEST_2007_07_15;
    datePlusDays(&test, 62); // over months
    assertDatesEquals(test, dateOf(2007, 9, 15));

    test = dateOf(2006, 7, 14);
    datePlusDays(&test, 366);    // over years
    assertDatesEquals(test, TEST_2007_07_15);

    test = TEST_2007_07_15;
    datePlusYears(&test, -1);
    datePlusDays(&test, 365 + 366);  // over leap years
    assertDatesEquals(test, dateOf(2008, 7, 15));

    test = TEST_2007_07_15;
    datePlusDays(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 7, 14));

    test = TEST_2007_07_15;
    datePlusDays(&test, -196); // negative across year
    assertDatesEquals(test, dateOf(2006, 12, 31));

    test = TEST_2007_07_15;
    datePlusDays(&test, -730); // negative over years
    assertDatesEquals(test, dateOf(2005, 7, 15));

    test = dateOf(YEAR_RANGE.max, 12, 30);
    assert_true(isDateValid(&test));
    datePlusDays(&test, 1);         // maximum
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 2);
    assert_true(isDateValid(&test));
    datePlusDays(&test, -1);         // minimum
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));

    test = dateOf(YEAR_RANGE.max, 12, 31);
    assert_true(isDateValid(&test));
    datePlusDays(&test, 1);         // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 1);
    assert_true(isDateValid(&test));
    datePlusDays(&test, -1);         // too small
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));
    return MUNIT_OK;
}

static MunitResult testDateMinusYears(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    dateMinusYears(&test, 1); // normal
    assertDatesEquals(test, dateOf(2006, 7, 15));

    test = TEST_2007_07_15;
    dateMinusYears(&test, -1);  // negative
    assertDatesEquals(test, dateOf(2008, 7, 15));

    test = dateOf(2008, 2, 29);     // adjust day
    dateMinusYears(&test, 1);
    assertDatesEquals(test, dateOf(2007, 2, 28));

    int64_t years = 20 + YEAR_RANGE.max;
    test = dateOf(40, 6, 1);
    dateMinusYears(&test, years); // big value
    assertDatesEquals(test, dateOf((40L - years), 6, 1));

    test = dateOf(YEAR_RANGE.max, 6, 1);
    assert_true(isDateValid(&test));
    dateMinusYears(&test, -1); // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 6, 1));

    test = dateOf(YEAR_RANGE.min, 1, 1);
    assert_true(isDateValid(&test));
    dateMinusYears(&test, 1); // too small valid year
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));
    return MUNIT_OK;
}

static MunitResult testDateMinusMonths(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    dateMinusMonths(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 6, 15));

    test = TEST_2007_07_15;
    dateMinusMonths(&test, 25); // over years
    assertDatesEquals(test, dateOf(2005, 6, 15));

    test = TEST_2007_07_15;
    dateMinusMonths(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 8, 15));

    test = TEST_2007_07_15;
    dateMinusMonths(&test, -7); // negative across year
    assertDatesEquals(test, dateOf(2008, 2, 15));

    test = TEST_2007_07_15;
    dateMinusMonths(&test, -31); // negative over years
    assertDatesEquals(test, dateOf(2010, 2, 15));

    test = dateOf(2008, 2, 29);
    dateMinusMonths(&test, 12); // adjust day from leap year
    assertDatesEquals(test, dateOf(2007, 2, 28));

    test = dateOf(2007, 3, 31);
    dateMinusMonths(&test, 1); // adjust day from month length
    assertDatesEquals(test, dateOf(2007, 2, 28));

    test = dateOf(YEAR_RANGE.max, 12, 1);
    assert_true(isDateValid(&test));
    dateMinusMonths(&test, -1); // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 1));
    return MUNIT_OK;
}

static MunitResult testDateMinusWeeks(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    dateMinusWeeks(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 7, 8));

    test = TEST_2007_07_15;
    dateMinusWeeks(&test, 9); // over months
    assertDatesEquals(test, dateOf(2007, 5, 13));

    test = dateOf(2008, 7, 13);
    dateMinusWeeks(&test, 52);           // over years
    assertDatesEquals(test, TEST_2007_07_15);

    test = TEST_2007_07_15;
    dateMinusYears(&test, -1);
    dateMinusWeeks(&test, 104);           // over leap years
    assertDatesEquals(test, dateOf(2006, 7, 18));

    test = TEST_2007_07_15;
    dateMinusWeeks(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 7, 22));

    test = TEST_2007_07_15;
    dateMinusWeeks(&test, -28); // negative across year
    assertDatesEquals(test, dateOf(2008, 1, 27));

    test = TEST_2007_07_15;
    dateMinusWeeks(&test, -104); // negative over years
    assertDatesEquals(test, dateOf(2009, 7, 12));

    test = dateOf(YEAR_RANGE.max, 12, 24);
    assert_true(isDateValid(&test));
    dateMinusWeeks(&test, -1);         // maximum
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 8);
    assert_true(isDateValid(&test));
    dateMinusWeeks(&test, 1);         // minimum
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));

    test = dateOf(YEAR_RANGE.max, 12, 25);
    assert_true(isDateValid(&test));
    dateMinusWeeks(&test, -1);         // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 25));

    test = dateOf(YEAR_RANGE.min, 1, 7);
    assert_true(isDateValid(&test));
    dateMinusWeeks(&test, 1);         // too small
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 7));
    return MUNIT_OK;
}

static MunitResult testDateMinusDays(const MunitParameter params[], void *data) {
    Date test = TEST_2007_07_15;
    dateMinusDays(&test, 1); // normal
    assertDatesEquals(test, dateOf(2007, 7, 14));

    test = TEST_2007_07_15;
    dateMinusDays(&test, 62); // over months
    assertDatesEquals(test, dateOf(2007, 5, 14));

    test = dateOf(2008, 7, 16);
    dateMinusDays(&test, 367);    // over years
    assertDatesEquals(test, TEST_2007_07_15);

    test = TEST_2007_07_15;
    datePlusYears(&test, 2);
    dateMinusDays(&test, 365 + 366);  // over leap years
    assertDatesEquals(test, TEST_2007_07_15);

    test = TEST_2007_07_15;
    dateMinusDays(&test, -1); // negative
    assertDatesEquals(test, dateOf(2007, 7, 16));

    test = TEST_2007_07_15;
    dateMinusDays(&test, -169); // negative across year
    assertDatesEquals(test, dateOf(2007, 12, 31));

    test = TEST_2007_07_15;
    dateMinusDays(&test, -731); // negative over years
    assertDatesEquals(test, dateOf(2009, 7, 15));

    test = dateOf(YEAR_RANGE.max, 12, 30);
    assert_true(isDateValid(&test));
    dateMinusDays(&test, -1);         // maximum
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 2);
    assert_true(isDateValid(&test));
    dateMinusDays(&test, 1);         // minimum
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));

    test = dateOf(YEAR_RANGE.max, 12, 31);
    assert_true(isDateValid(&test));
    dateMinusDays(&test, -1);         // too large
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.max, 12, 31));

    test = dateOf(YEAR_RANGE.min, 1, 1);
    assert_true(isDateValid(&test));
    dateMinusDays(&test, 1);         // too small
    assert_true(isDateValid(&test));
    assertDatesEquals(test, dateOf(YEAR_RANGE.min, 1, 1));
    return MUNIT_OK;
}

static MunitResult testDateCompare(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(COMPARISON_DATE_ARRAY); i++) {
        Date *a = &COMPARISON_DATE_ARRAY[i];
        for (int j = 0; j < ARRAY_SIZE(COMPARISON_DATE_ARRAY); j++) {
            Date *b = &COMPARISON_DATE_ARRAY[j];
            if (i < j) {
                assert_true(dateCompare(a, b) < 0);
                assert_true(isDateBefore(a, b));
                assert_false(isDateAfter(a, b));
                assert_false(isDateEquals(a, b));

            } else if (i > j) {
                assert_true(dateCompare(a, b) > 0);
                assert_false(isDateBefore(a, b));
                assert_true(isDateAfter(a, b));
                assert_false(isDateEquals(a, b));

            } else {
                assert_true(dateCompare(a, b) == 0);
                assert_false(isDateBefore(a, b));
                assert_false(isDateAfter(a, b));
                assert_true(isDateEquals(a, b));
            }
        }
    }
    return MUNIT_OK;
}

static MunitResult testGetDayOfYear(const MunitParameter params[], void *data) {
    Date sampleDates[] = {
            {.year = 2008, .month = 7, .day = 5},
            {.year = 2007, .month = 7, .day = 5},
            {.year = 2006, .month = 7, .day = 5},
            {.year = 2005, .month = 7, .day = 5},
            {.year = 2004, .month = 1, .day = 1},
            {.year = -1, .month = 1, .day = 2},
    };

    for (int i = 0; i < ARRAY_SIZE(sampleDates); i++) {
        int total = 1;
        Date date = sampleDates[i];
        for (int j = 0; j < date.month; j++) {
            total += lengthOfMonth(j, isLeapYear(date.year));
        }
        int doy = total + date.day;
        int testDoY = (int) getDayOfYear(&date);
        assert_int(testDoY, ==, doy);
    }

    return MUNIT_OK;
}

static MunitResult testGetDayOfWeek(const MunitParameter params[], void *data) {
    DayOfWeek dow = MONDAY;
    for (Month month = JANUARY; month <= DECEMBER; month++) {
        int8_t length = lengthOfMonth(month, false);
        for (int i = 1; i <= length; i++) {
            Date d = dateOf(2007, month, i);
            DayOfWeek test = getDayOfWeek(&d);
            assert_int(test, ==, dow);
            dow = dayOfWeekPlus(dow, 1);
        }
    }
    return MUNIT_OK;
}

static MunitResult testWeekOfYear(const MunitParameter params[], void *data) {
    int expectedValues[] = {5, 9, 13, 18, 22, 26, 31, 35, 39, 44, 48, 53};
    for (int i = 1; i <= 12; i++) {
        Date test = dateOf(2022, i, lengthOfMonth(i, false));
        uint8_t weekCount = getWeekOfYear(&test);
        assert_int8(weekCount, ==, expectedValues[i - 1]);
    }
    return MUNIT_OK;
}

static MunitResult testWeekOfMonth(const MunitParameter params[], void *data) {
    int expectedValues[] = {1, 2, 3, 4};
    for (int i = 1; i <= 12; i++) {
        Date test = dateOf(2022, i, 1);
        for (int j = 0; j < 4; j++) {
            uint8_t weekOfMonth = getWeekOfMonth(&test);
            assert_int8(weekOfMonth, ==, expectedValues[j]);
            datePlusWeeks(&test, 1);
        }
    }
    return MUNIT_OK;
}

static MunitResult testLengthOfYear(const MunitParameter params[], void *data) {
    assert_int(getLengthOfYear(2007), ==, 365);
    assert_int(getLengthOfYear(2008), ==, 366);
    return MUNIT_OK;
}

static MunitResult testLengthOfMonth(const MunitParameter params[], void *data) {
    // not leap year
    assert_int8(lengthOfMonth(1, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(2, isLeapYear(2007)), ==, 28);
    assert_int8(lengthOfMonth(3, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(4, isLeapYear(2007)), ==, 30);
    assert_int8(lengthOfMonth(5, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(6, isLeapYear(2007)), ==, 30);
    assert_int8(lengthOfMonth(7, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(8, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(9, isLeapYear(2007)), ==, 30);
    assert_int8(lengthOfMonth(10, isLeapYear(2007)), ==, 31);
    assert_int8(lengthOfMonth(11, isLeapYear(2007)), ==, 30);
    assert_int8(lengthOfMonth(12, isLeapYear(2007)), ==, 31);

    // leap year
    assert_int8(lengthOfMonth(1, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(2, isLeapYear(2008)), ==, 29);
    assert_int8(lengthOfMonth(3, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(4, isLeapYear(2008)), ==, 30);
    assert_int8(lengthOfMonth(5, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(6, isLeapYear(2008)), ==, 30);
    assert_int8(lengthOfMonth(7, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(8, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(9, isLeapYear(2008)), ==, 30);
    assert_int8(lengthOfMonth(10, isLeapYear(2008)), ==, 31);
    assert_int8(lengthOfMonth(11, isLeapYear(2008)), ==, 30);
    assert_int8(lengthOfMonth(12, isLeapYear(2008)), ==, 31);
    return MUNIT_OK;
}

static MunitResult testDateIsLeapYear(const MunitParameter params[], void *data) {
    assert_false(isLeapYear(1999));
    assert_true(isLeapYear(2000));
    assert_false(isLeapYear(2001));
    assert_false(isLeapYear(2002));
    assert_false(isLeapYear(2003));
    assert_true(isLeapYear(2004));
    assert_false(isLeapYear(2005));

    assert_false(isLeapYear(1500));
    assert_true(isLeapYear(1600));
    assert_false(isLeapYear(1700));
    assert_false(isLeapYear(1800));
    assert_false(isLeapYear(1900));
    return MUNIT_OK;
}

static MunitResult testGetWeekDayNameShort(const MunitParameter params[], void *data) {
    assert_string_equal(getWeekDayNameShort(MONDAY), "Mon");
    assert_string_equal(getWeekDayNameShort(TUESDAY), "Tue");
    assert_string_equal(getWeekDayNameShort(WEDNESDAY), "Wed");
    assert_string_equal(getWeekDayNameShort(THURSDAY), "Thu");
    assert_string_equal(getWeekDayNameShort(FRIDAY), "Fri");
    assert_string_equal(getWeekDayNameShort(SATURDAY), "Sat");
    assert_string_equal(getWeekDayNameShort(SUNDAY), "Sun");
    return MUNIT_OK;
}

static MunitResult testGetWeekDayNameLong(const MunitParameter params[], void *data) {
    assert_string_equal(getWeekDayNameLong(MONDAY), "Monday");
    assert_string_equal(getWeekDayNameLong(TUESDAY), "Tuesday");
    assert_string_equal(getWeekDayNameLong(WEDNESDAY), "Wednesday");
    assert_string_equal(getWeekDayNameLong(THURSDAY), "Thursday");
    assert_string_equal(getWeekDayNameLong(FRIDAY), "Friday");
    assert_string_equal(getWeekDayNameLong(SATURDAY), "Saturday");
    assert_string_equal(getWeekDayNameLong(SUNDAY), "Sunday");
    return MUNIT_OK;
}

static MunitResult testGetMonthNameShort(const MunitParameter params[], void *data) {
    assert_string_equal(getMonthNameShort(JANUARY), "Jan");
    assert_string_equal(getMonthNameShort(FEBRUARY), "Feb");
    assert_string_equal(getMonthNameShort(MARCH), "Mar");
    assert_string_equal(getMonthNameShort(APRIL), "Apr");
    assert_string_equal(getMonthNameShort(MAY), "May");
    assert_string_equal(getMonthNameShort(JUNE), "Jun");
    assert_string_equal(getMonthNameShort(JULY), "Jul");
    assert_string_equal(getMonthNameShort(AUGUST), "Aug");
    assert_string_equal(getMonthNameShort(SEPTEMBER), "Sep");
    assert_string_equal(getMonthNameShort(OCTOBER), "Oct");
    assert_string_equal(getMonthNameShort(NOVEMBER), "Nov");
    assert_string_equal(getMonthNameShort(DECEMBER), "Dec");
    return MUNIT_OK;
}

static MunitResult testGetMonthNameLong(const MunitParameter params[], void *data) {
    assert_string_equal(getMonthNameLong(JANUARY), "January");
    assert_string_equal(getMonthNameLong(FEBRUARY), "February");
    assert_string_equal(getMonthNameLong(MARCH), "March");
    assert_string_equal(getMonthNameLong(APRIL), "April");
    assert_string_equal(getMonthNameLong(MAY), "May");
    assert_string_equal(getMonthNameLong(JUNE), "June");
    assert_string_equal(getMonthNameLong(JULY), "July");
    assert_string_equal(getMonthNameLong(AUGUST), "August");
    assert_string_equal(getMonthNameLong(SEPTEMBER), "September");
    assert_string_equal(getMonthNameLong(OCTOBER), "October");
    assert_string_equal(getMonthNameLong(NOVEMBER), "November");
    assert_string_equal(getMonthNameLong(DECEMBER), "December");
    return MUNIT_OK;
}

static MunitResult testGetIsoEraShort(const MunitParameter params[], void *data) {
    assert_string_equal(getIsoEraShort(-1), "BCE");
    assert_string_equal(getIsoEraShort(1), "CE");
    return MUNIT_OK;
}

static MunitResult testGetIsoEraLong(const MunitParameter params[], void *data) {
    assert_string_equal(getIsoEraLong(-1), "Before current era");
    assert_string_equal(getIsoEraLong(1), "Current era");
    return MUNIT_OK;
}

static MunitTest dateTests[] = {
        {.name =  "Test dateOf() - should validate and create date", .test = testDateFactory},
        {.name =  "Test dateOfInstant() - should correctly create date from instant", .test = testDateOfInstantFactory},
        {.name =  "Test dateOfEpochDay() - should correctly create date from epoch", .test = testDateOfEpochDay},
        {.name =  "Test dateOfYearDay() - should correctly create date day in year", .test = testDateOfYearDay},
        {.name =  "Test dateToEpochDay() - should correctly convert date to epoch days", .test = testDateToEpochDay},
        {.name =  "Test dateToEpochSeconds() - should correctly convert date to epoch seconds", .test = testDateToEpochSeconds},

        {.name =  "Test datePlusYears() - should correctly add years to date", .test = testDatePlusYears},
        {.name =  "Test datePlusMonths() - should correctly add months to date", .test = testDatePlusMonths},
        {.name =  "Test datePlusWeeks() - should correctly add weeks to date", .test = testDatePlusWeeks},
        {.name =  "Test datePlusDays() - should correctly add days to date", .test = testDatePlusDays},

        {.name =  "Test dateMinusYears() - should correctly subtract years from date", .test = testDateMinusYears},
        {.name =  "Test dateMinusMonths() - should correctly subtract months from date", .test = testDateMinusMonths},
        {.name =  "Test dateMinusWeeks() - should correctly subtract weeks from date", .test = testDateMinusWeeks},
        {.name =  "Test dateMinusDays() - should correctly subtract days from date", .test = testDateMinusDays},
        {.name =  "Test lengthOfMonth() - should correctly return day count in month", .test = testLengthOfMonth},
        {.name =  "Test isLeapYear() - should correctly check for leap year", .test = testDateIsLeapYear},

        {.name =  "Test dateCompare() - should correctly compare two date instances", .test = testDateCompare},
        {.name =  "Test getDayOfYear() - should correctly calculate day of year", .test = testGetDayOfYear},
        {.name =  "Test getDayOfWeek() - should correctly calculate day of week", .test = testGetDayOfWeek},
        {.name =  "Test getWeekOfYear() - should correctly calculate week of year", .test = testWeekOfYear},
        {.name =  "Test getWeekOfMonth() - should correctly calculate week of month", .test = testWeekOfMonth},
        {.name =  "Test getLengthOfYear() - should correctly return day count in year", .test = testLengthOfYear},
        {.name =  "Test getWeekDayNameShort() - should return short week name", .test = testGetWeekDayNameShort},
        {.name =  "Test getWeekDayNameLong() - should return long week name", .test = testGetWeekDayNameLong},
        {.name =  "Test getMonthNameShort() - should return short month name", .test = testGetMonthNameShort},
        {.name =  "Test getMonthNameLong() - should return long month name", .test = testGetMonthNameLong},
        {.name =  "Test getIsoEraShort() - should return short ISO era name", .test = testGetIsoEraShort},
        {.name =  "Test getIsoEraLong() - should return long ISO era name", .test = testGetIsoEraLong},
        END_OF_TESTS
};

static const MunitSuite dateTestSuite = {
        .prefix = "Date: ",
        .tests = dateTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};