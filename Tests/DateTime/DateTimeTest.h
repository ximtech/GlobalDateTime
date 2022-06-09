#pragma once

#include "BaseTestTemplate.h"
#include "DateTime.h"

static const TimeZone ZONE_PARIS = {.id = "Europe/Paris", .utcOffset = 7200};
static const TimeZone ZONE_MINUS_TWO_HOURS = {.id = "GMT-2", .utcOffset = -7200};
static const TimeZone ZONE_PLUS_TWO_HOURS = {.id = "GMT+2", .utcOffset = 7200};

static int64_t COMPARISON_DATE_TIME_ARRAY[][7] = {
        {-9999999, 1,  1,  0,  0,  0,  0},
        {-9999999, 12, 31, 0,  0,  0,  999},
        {-1,       1,  1,  0,  0,  59, 0},
        {-1,       12, 31, 0,  0,  59, 999},
        {0,        1,  1,  0,  59, 0,  0},
        {0,        12, 31, 0,  59, 59, 999},
        {1,        1,  1,  12, 0,  0,  999},
        {1,        12, 31, 12, 0,  59, 0},
        {2008,     1,  1,  12, 0,  59, 999},
        {2008,     2,  29, 12, 59, 0,  0},
        {2008,     12, 31, 12, 59, 59, 999},
        {9999999,  1,  1,  23, 0,  0,  0},
        {9999999,  12, 31, 23, 0,  0,  999}
};

static DateTime TEST_2007_07_15_12_30_40_987;

static void *dateTimeSetup(const MunitParameter params[], void *user_data) {
    TEST_2007_07_15_12_30_40_987 = dateTimeOfWithMillis(2007, 7, 15, 12, 30, 40, 987);
    return NULL; // nothing to return, only data initialization
}


static void checkDateTime(DateTime test, int year, int month, int day, int hour, int minute, int seconds, int millis) {
    assert_int(test.date.year, ==, year);
    assert_int(test.date.month, ==, month);
    assert_int8(test.date.day, ==, day);
    assert_int8(test.time.hours, ==, hour);
    assert_int8(test.time.minutes, ==, minute);
    assert_int8(test.time.seconds, ==, seconds);
    assert_int(test.time.millis, ==, millis);
}

static void assertDateTimeEquals(DateTime test, DateTime expected) {
    checkDateTime(test,
                  expected.date.year,
                  expected.date.month,
                  expected.date.day,
                  expected.time.hours,
                  expected.time.minutes,
                  expected.time.seconds,
                  expected.time.millis);
}

static MunitResult testDateTimeFactories(const MunitParameter params[], void *data) {
    DateTime test = dateTimeOf(2022, JANUARY, 23, 12, 22);
    checkDateTime(test, 2022, 1, 23, 12, 22, 0, 0);
    assert_true(isDateTimeValid(&test));

    test = dateTimeOfWithSeconds(2022, JANUARY, 23, 12, 22, 45);
    checkDateTime(test, 2022, 1, 23, 12, 22, 45, 0);
    assert_true(isDateTimeValid(&test));

    test = dateTimeOfWithMillis(2022, JANUARY, 23, 12, 22, 45, 999);
    checkDateTime(test, 2022, 1, 23, 12, 22, 45, 999);
    assert_true(isDateTimeValid(&test));

    Date date = dateOf(2022, 1, 23);
    Time time = timeOfWithMillis(12, 22, 45, 999);
    test = dateTimeFrom(&date, &time);
    checkDateTime(test, 2022, 1, 23, 12, 22, 45, 999);
    assert_true(isDateTimeValid(&test));

    test = dateTimeFrom(NULL, &time);
    checkDateTime(test, 0, 0, 0, -1, -1, -1, -1);
    assert_false(isDateValid(&test.date));
    assert_false(isTimeValid(&test.time));
    return MUNIT_OK;
}

static MunitResult testDateTimeOfInstant(const MunitParameter params[], void *data) {
    Instant instant = instantOfEpochSecondsAdjust(86400 + 3600 + 120 + 4, 500000);
    DateTime test = dateTimeOfInstant(&instant, &ZONE_PARIS);
    assertDateTimeEquals(test, dateTimeOfWithMillis(1970, 1, 2, 3, 2, 4, 500));

    test = dateTimeOfInstant(&instant, &ZONE_MINUS_TWO_HOURS);
    assertDateTimeEquals(test, dateTimeOfWithMillis(1970, 1, 1, 23, 2, 4, 500));

    instant = instantOfEpochSecondsAdjust(-86400 + 4, 500000);
    test = dateTimeOfInstant(&instant, &ZONE_PLUS_TWO_HOURS);
    assertDateTimeEquals(test, dateTimeOfWithMillis(1969, 12, 31, 2, 0, 4, 500));
    return MUNIT_OK;
}

static MunitResult testDateTimeOfEpochSeconds(const MunitParameter params[], void *data) {
    // after epoch
    DateTime base = dateTimeOfWithMillis(1970, 1, 1, 2, 0, 0, 500);
    for (int i = 0; i < 100000; i++) {
        DateTime test = dateTimeOfEpochSeconds(i, 500000, ZONE_PLUS_TWO_HOURS.utcOffset);
        assertDateTimeEquals(test, base);
        dateTimePlusSeconds(&base, 1);
    }

    // before epoch
    base = dateTimeOfWithMillis(1970, 1, 1, 2, 0, 0, 500);
    for (int i = 0; i < 100000; i++) {
        DateTime test = dateTimeOfEpochSeconds(-i, 500000, ZONE_PLUS_TWO_HOURS.utcOffset);
        assertDateTimeEquals(test, base);
        dateTimeMinusSeconds(&base, 1);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeAtStartOfDay(const MunitParameter params[], void *data) {
    DateTime test = dateTimeOfWithMillis(2008, 1, 23, 12, 32, 50, 500);
    test = dateTimeAtStartOfDay(&test.date);
    checkDateTime(test, 2008, 1, 23, 0, 0, 0, 0);
    return MUNIT_OK;
}

static MunitResult testDateTimePlusYears(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusYears(&test, 1);    // normal
    checkDateTime(test, 2008, 7, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusYears(&test, -1);    // negative
    checkDateTime(test, 2006, 7, 15, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimePlusMonths(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusMonths(&test, 1);    // normal
    checkDateTime(test, 2007, 8, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusMonths(&test, 25);    // over years
    checkDateTime(test, 2009, 8, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusMonths(&test, -7);    // negative across year
    checkDateTime(test, 2006, 12, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusMonths(&test, -31);    // negative over years
    checkDateTime(test, 2004, 12, 15, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimePlusWeeks(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusWeeks(&test, 1);    // normal
    checkDateTime(test, 2007, 7, 22, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusWeeks(&test, 9);    // over months
    checkDateTime(test, 2007, 9, 16, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusWeeks(&test, 52);    // over year
    checkDateTime(test, 2008, 7, 13, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;    // over leap year
    dateTimePlusYears(&test, -1);
    dateTimePlusWeeks(&test, 104);
    checkDateTime(test, 2008, 7, 12, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusWeeks(&test, -1);    // negative
    checkDateTime(test, 2007, 7, 8, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusWeeks(&test, -28);    // negative across year
    checkDateTime(test, 2006, 12, 31, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimePlusDays(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusDays(&test, 1);    // normal
    checkDateTime(test, 2007, 7, 16, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusDays(&test, 62);    // over months
    checkDateTime(test, 2007, 9, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusDays(&test, 366);    // over year
    checkDateTime(test, 2008, 7, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;    // over leap year
    dateTimePlusYears(&test, -1);
    dateTimePlusDays(&test, 365 + 366);
    checkDateTime(test, 2008, 7, 15, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusDays(&test, -1);    // negative
    checkDateTime(test, 2007, 7, 14, 12, 30, 40, 987);

    test = TEST_2007_07_15_12_30_40_987;
    dateTimePlusDays(&test, -196);    // negative across year
    checkDateTime(test, 2006, 12, 31, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimePlusHours(const MunitParameter params[], void *data) {
    // plus one test
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;
    for (int i = 0; i < 50; i++) {
        dateTimePlusHours(&test, 1);

        if ((i + 1) % 24 == 0) {
            datePlusDays(&date, 1);
        }
        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, (i + 1) % 24);
    }

    // plus from zero
    test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    date = test.date;
    dateMinusDays(&date, 3);
    Time time = timeOfHourAndMinutes(21, 0);

    for (int i = -50; i < 50; i++) {
        DateTime dt = test;
        dateTimePlusHours(&dt, i);
        timePlusHours(&time, 1);

        if (time.hours == 0) {
            datePlusDays(&date, 1);
        }
        assertDatesEquals(dt.date, date);
        assertTimeEquals(dt.time, time);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimePlusMinutes(const MunitParameter params[], void *data) {
    // plus one test
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;

    int hour = 0;
    int min = 0;

    for (int i = 0; i < 70; i++) {
        dateTimePlusMinutes(&test, 1);
        min++;
        if (min == 60) {
            hour++;
            min = 0;
        }
        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
    }

    // plus from zero
    test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    date = test.date;
    dateMinusDays(&date, 1);
    Time time = timeOfHourAndMinutes(22, 49);

    for (int i = -70; i < 70; i++) {
        DateTime dt = test;
        dateTimePlusMinutes(&dt, i);
        timePlusMinutes(&time, 1);

        if (isTimeEquals(&time, &MIDNIGHT)) {
            datePlusDays(&date, 1);
        }

        assertDatesEquals(dt.date, date);
        assertTimeEquals(dt.time, time);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimePlusSeconds(const MunitParameter params[], void *data) {
    // plus one test
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;

    int hour = 0;
    int min = 0;
    int sec = 0;

    for (int i = 0; i < 3700; i++) {
        dateTimePlusSeconds(&test, 1);
        sec++;
        if (sec == 60) {
            min++;
            sec = 0;
        }
        if (min == 60) {
            hour++;
            min = 0;
        }

        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
        assert_int8(test.time.seconds, ==, sec);
    }

    // plus from zero
    date = TEST_2007_07_15_12_30_40_987.date;
    dateMinusDays(&date, 1);
    int delta = 30;
    hour = 22;
    min = 59;
    sec = 0;

    for (int i = -3660; i < 3660;) {
        i += delta;
        sec += delta;

        if (sec >= 60) {
            min++;
            sec -= 60;

            if (min == 60) {
                hour++;
                min = 0;

                if (hour == 24) {
                    hour = 0;
                }
            }
        }

        if (i == 0) {
            datePlusDays(&date, 1);
        }

        DateTime base = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
        dateTimePlusSeconds(&base, i);
        assertDatesEquals(base.date, date);
        assert_int8(base.time.hours, ==, hour);
        assert_int8(base.time.minutes, ==, min);
        assert_int8(base.time.seconds, ==, sec);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimePlusMillis(const MunitParameter params[], void *data) {
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;

    int hour = 0;
    int min = 0;
    int sec = 0;
    int millis = 0;

    for (int i = 0; i < 3700 * 1000L; i += 500) {
        dateTimePlusMillis(&test, 500);
        millis += 500;

        if (millis == 1000) {
            sec++;
            millis = 0;
        }
        if (sec == 60) {
            min++;
            sec = 0;
        }
        if (min == 60) {
            hour++;
            min = 0;
        }

        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
        assert_int8(test.time.seconds, ==, sec);
        assert_int8(test.time.millis, ==, millis);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusYears(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimeMinusYears(&test, 1);
    checkDateTime(test, 2006, 7, 15, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusMonths(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimeMinusMonths(&test, 25);
    checkDateTime(test, 2005, 6, 15, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusWeeks(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimeMinusWeeks(&test, 9);
    checkDateTime(test, 2007, 5, 13, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusDays(const MunitParameter params[], void *data) {
    DateTime test = TEST_2007_07_15_12_30_40_987;
    dateTimeMinusDays(&test, 62);
    checkDateTime(test, 2007, 5, 14, 12, 30, 40, 987);
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusHours(const MunitParameter params[], void *data) {
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;

    for (int i = 0; i < 50; i++) {
        dateTimeMinusHours(&test, 1);

        if (i % 24 == 0) {
            dateMinusDays(&date, 1);
        }
        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, (((-i + 23) % 24) + 24) % 24);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusMinutes(const MunitParameter params[], void *data) {
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;
    dateMinusDays(&date, 1);

    int hour = 0;
    int min = 0;

    for (int i = 0; i < 70; i++) {
        dateTimeMinusMinutes(&test, 1);
        min--;
        if (min == -1) {
            hour--;
            min = 59;

            if (hour == -1) {
                hour = 23;
            }
        }
        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusSeconds(const MunitParameter params[], void *data) {
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;
    dateMinusDays(&date, 1);

    int hour = 0;
    int min = 0;
    int sec = 0;

    for (int i = 0; i < 3700; i++) {
        dateTimeMinusSeconds(&test, 1);
        sec--;
        if (sec == -1) {
            min--;
            sec = 59;

            if (min == -1) {
                hour--;
                min = 59;

                if (hour == -1) {
                    hour = 23;
                }
            }
        }

        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
        assert_int8(test.time.seconds, ==, sec);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeMinusMillis(const MunitParameter params[], void *data) {
    DateTime test = dateTimeAtStartOfDay(&TEST_2007_07_15_12_30_40_987.date);
    Date date = test.date;
    dateMinusDays(&date, 1);

    int hour = 0;
    int min = 0;
    int sec = 0;
    int millis = 0;

    for (long i = 0; i < 3700 * 1000L; i += 500) {
        dateTimeMinusMillis(&test, 500);
        millis -= 500;

        if (millis < 0) {
            sec--;
            millis += 1000;

            if (sec == -1) {
                min--;
                sec += 60;

                if (min == -1) {
                    hour--;
                    min += 60;

                    if (hour == -1) {
                        hour += 24;
                    }
                }
            }
        }

        assertDatesEquals(test.date, date);
        assert_int8(test.time.hours, ==, hour);
        assert_int8(test.time.minutes, ==, min);
        assert_int8(test.time.seconds, ==, sec);
        assert_int8(test.time.millis, ==, millis);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeToEpochSecond(const MunitParameter params[], void *data) {
    for (int i = 0; i < 100000; i++) {
        DateTime test = dateTimeOf(1970, 1, 1, 0, 0);
        dateTimeMinusSeconds(&test, i);
        assert_int64(dateTimeToEpochSecond(&test, UTC.utcOffset), ==, -i);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeCompare(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(COMPARISON_DATE_TIME_ARRAY); i++) {
        DateTime a = dateTimeOfWithMillis(
                COMPARISON_DATE_TIME_ARRAY[i][0],
                COMPARISON_DATE_TIME_ARRAY[i][1],
                COMPARISON_DATE_TIME_ARRAY[i][2],
                COMPARISON_DATE_TIME_ARRAY[i][3],
                COMPARISON_DATE_TIME_ARRAY[i][4],
                COMPARISON_DATE_TIME_ARRAY[i][5],
                COMPARISON_DATE_TIME_ARRAY[i][6]);
        for (int j = 0; j < ARRAY_SIZE(COMPARISON_DATE_TIME_ARRAY); j++) {
            DateTime b = dateTimeOfWithMillis(
                    COMPARISON_DATE_TIME_ARRAY[j][0],
                    COMPARISON_DATE_TIME_ARRAY[j][1],
                    COMPARISON_DATE_TIME_ARRAY[j][2],
                    COMPARISON_DATE_TIME_ARRAY[j][3],
                    COMPARISON_DATE_TIME_ARRAY[j][4],
                    COMPARISON_DATE_TIME_ARRAY[j][5],
                    COMPARISON_DATE_TIME_ARRAY[j][6]);

            if (i < j) {
                assert_true(dateTimeCompare(&a, &b) < 0);
                assert_true(isDateTimeBefore(&a, &b));
                assert_false(isDateTimeAfter(&a, &b));
                assert_false(isDateTimeEquals(&a, &b));

            } else if (i > j) {
                assert_true(dateTimeCompare(&a, &b) > 0);
                assert_false(isDateTimeBefore(&a, &b));
                assert_true(isDateTimeAfter(&a, &b));
                assert_false(isDateTimeEquals(&a, &b));

            } else {
                assert_true(dateTimeCompare(&a, &b) == 0);
                assert_false(isDateTimeBefore(&a, &b));
                assert_false(isDateTimeAfter(&a, &b));
                assert_true(isDateTimeEquals(&a, &b));
            }
        }
    }
    return MUNIT_OK;
}


static MunitTest dateTimeTests[] = {
        {.name =  "Test dateTimeOf() - should validate and create date-time", .test = testDateTimeFactories},
        {.name =  "Test dateTimeOfInstant() - should create date-time from instant", .test = testDateTimeOfInstant},
        {.name =  "Test dateTimeOfEpochSeconds() - should create date-time from epoch seconds", .test = testDateTimeOfEpochSeconds},
        {.name =  "Test dateTimeAtStartOfDay() - should set time to the midnight", .test = testDateTimeAtStartOfDay},

        {.name =  "Test dateTimePlusYears() - should correctly add years to date-time", .test = testDateTimePlusYears, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusMonths() - should correctly add months to date-time", .test = testDateTimePlusMonths, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusWeeks() - should correctly add weeks to date-time", .test = testDateTimePlusWeeks, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusDays() - should correctly add days to date-time", .test = testDateTimePlusDays, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusHours() - should correctly add hours to date-time", .test = testDateTimePlusHours, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusMinutes() - should correctly add minutes to date-time", .test = testDateTimePlusMinutes, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusSeconds() - should correctly add seconds to date-time", .test = testDateTimePlusSeconds, .setup = dateTimeSetup},
        {.name =  "Test dateTimePlusMillis() - should correctly add millis to date-time", .test = testDateTimePlusMillis, .setup = dateTimeSetup},

        {.name =  "Test dateTimeMinusYears() - should correctly subtract years from date-time", .test = testDateTimeMinusYears, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusMonths() - should correctly subtract months from date-time", .test = testDateTimeMinusMonths, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusWeeks() - should correctly subtract weeks from date-time", .test = testDateTimeMinusWeeks, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusDays() - should correctly subtract days from date-time", .test = testDateTimeMinusDays, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusHours() - should correctly subtract hours from date-time", .test = testDateTimeMinusHours, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusMinutes() - should correctly subtract minutes from date-time", .test = testDateTimeMinusMinutes, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusSeconds() - should correctly subtract seconds from date-time", .test = testDateTimeMinusSeconds, .setup = dateTimeSetup},
        {.name =  "Test dateTimeMinusMillis() - should correctly subtract millis from date-time", .test = testDateTimeMinusMillis, .setup = dateTimeSetup},

        {.name =  "Test dateTimeToEpochSecond() - should correctly convert date-time to epoch seconds", .test = testDateTimeToEpochSecond},
        {.name =  "Test dateTimeCompare() - should correctly compare two date-time instances", .test = testDateTimeCompare},
        END_OF_TESTS
};

static const MunitSuite dateTimeTestSuite = {
        .prefix = "DateTime: ",
        .tests = dateTimeTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};