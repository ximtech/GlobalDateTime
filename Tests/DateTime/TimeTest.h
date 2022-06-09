#pragma once

#include "BaseTestTemplate.h"
#include "Time.h"

static const int8_t INVALID_TIME_TEST_CASES[][3] = {
        {24, 0, 0},
        {-1, 0, 0},
        {0, -1, 0},
        {0, 60, 0},
        {0, 0, 80},
        {0, 0, -1},
};

static Time COMPARISON_TIME_ARRAY[] = {
        {0, 0, 0, 0},
        {0, 0, 0, 999},
        {0, 0, 59, 0},
        {0, 0, 59, 999},
        {0, 59, 0, 0},
        {0, 59, 0, 999},
        {0, 59, 59, 0},
        {0, 59, 59, 999},

        {12, 0, 0, 0},
        {12, 0, 0, 999},
        {12, 0, 59, 0},
        {12, 0, 59, 999},
        {12, 59, 0, 0},
        {12, 59, 0, 999},
        {12, 59, 59, 0},
        {12, 59, 59, 999},

        {23, 0, 0, 0},
        {23, 0, 0, 999},
        {23, 0, 59, 0},
        {23, 0, 59, 999},
        {23, 59, 0, 0},
        {23, 59, 0, 999},
        {23, 59, 59, 0},
        {23, 59, 59, 999},
};

static void checkTime(const Time *test, int8_t hours, int8_t minutes, int8_t seconds, int16_t millis) {
    assert_int8(test->hours, ==, hours);
    assert_int8(test->minutes, ==, minutes);
    assert_int8(test->seconds, ==, seconds);
    assert_int16(test->millis, ==, millis);
    Time time = timeOfWithMillis(hours, minutes, seconds, millis);
    assert_true(isTimeEquals(test, &time));
}

static void assertTimeEquals(Time test, Time expected) {
    checkTime(&test, expected.hours, expected.minutes, expected.seconds, expected.millis);
}

static MunitResult testMidnightTime(const MunitParameter params[], void *data) {
    checkTime(&MIDNIGHT, 0, 0, 0, 0);
    return MUNIT_OK;
}

static MunitResult testAllSecondsInDay(const MunitParameter params[], void *data) {
    for (int i = 0; i < (2 * 24 * 60 * 60); i++) {
        Instant instant = instantOfEpochSeconds(i);
        Time test = timeOfInstant(&instant, &UTC);
        assert_int8(test.hours, ==, (i / (60 * 60)) % 24);
        assert_int8(test.minutes, ==, (i / 60) % 60);
        assert_int8(test.seconds, ==, i % 60);
    }

    Time test = timeOfInstant(NULL, &UTC);  // null check
    assert_false(isTimeValid(&test));
    return MUNIT_OK;
}

static MunitResult testCheckTimeFactories(const MunitParameter params[], void *data) {
    Time test = timeOf(12, 30, 45);
    checkTime(&test, 12, 30, 45, 0);
    // OK
    test = timeOfWithMillis(12, 30, 10, 332);
    checkTime(&test, 12, 30, 10, 332);
    // OK
    test = timeOfHourAndMinutes(12, 30);
    checkTime(&test, 12, 30, 0, 0);
    return MUNIT_OK;
}

static MunitResult testInvalidTime(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INVALID_TIME_TEST_CASES); i++) {
        const int8_t *testCase = INVALID_TIME_TEST_CASES[i];
        Time test = timeOf(testCase[0], testCase[1], testCase[2]);
        assert_false(isTimeValid(&test));
    }
    // check millis
    Time test = timeOfWithMillis(12, 30, 33, 1045);
    assert_false(isTimeValid(&test));
    return MUNIT_OK;
}

static MunitResult testTimeOfSecondsOfDay(const MunitParameter params[], void *data) {
    Time test = timeOfSecondOfDay(2 * 60 * 60 + 17 * 60 + 23);
    checkTime(&test, 2, 17, 23, 0); // OK

    test = timeOfSecondOfDay(-1);   // too low
    assert_false(isTimeValid(&test));

    test = timeOfSecondOfDay(24 * 60 * 60);   // too high
    assert_false(isTimeValid(&test));
    return MUNIT_OK;
}

static MunitResult testTimeOfMillisOfDay(const MunitParameter params[], void *data) {
    Time test = timeOfMillisOfDay(60 * 60 * 1000L + 17);
    checkTime(&test, 1, 0, 0, 17); // OK

    test = timeOfMillisOfDay(-1);   // too low
    assert_false(isTimeValid(&test));

    test = timeOfMillisOfDay(24 * 60 * 60 * 1000L);   // too high
    assert_false(isTimeValid(&test));


    return MUNIT_OK;
}

static MunitResult testTimeOfMicrosOfDay(const MunitParameter params[], void *data) {
    Time test = timeOfMicrosOfDay(60 * 60 * 1000000LL + 17000);
    checkTime(&test, 1, 0, 0, 17); // OK

    test = timeOfMicrosOfDay(-1);   // too low
    assert_false(isTimeValid(&test));

    test = timeOfMicrosOfDay(24 * 60 * 60 * 1000000LL);   // too high
    assert_false(isTimeValid(&test));
    return MUNIT_OK;
}

static MunitResult testTimePlusHours(const MunitParameter params[], void *data) {
    Time test = MIDNIGHT;
    timePlusHours(&test, 0); // no change
    checkTime(&test, 0, 0, 0, 0);
    for (int i = 0; i < 50; i++) {  // add one
        timePlusHours(&test, 1);
        assert_int8(test.hours, ==, (i + 1) % 24);
    }

    for (int i = -50; i < 50; i++) {    // from zero
        test = MIDNIGHT;
        timePlusHours(&test, i);
        assert_int8(test.hours, ==, (i + 72) % 24);
    }

    for (int i = -50; i < 50; i++) {    // from one
        test = timeOfHourAndMinutes(1, 0);
        timePlusHours(&test, i);
        assert_int8(test.hours, ==, (1 + i + 72) % 24);
    }

    // should be midnight equals
    Time toMidnight = timeOfHourAndMinutes(23, 0);
    timePlusHours(&toMidnight, 1);
    assert_true(isTimeEquals(&MIDNIGHT, &toMidnight));

    // plus hours max value
    test = timeOfHourAndMinutes(2, 30);
    timePlusHours(&test, INT32_MAX);
    int8_t hours = (INT32_MAX % 24);
    Time test2 = timeOfHourAndMinutes(2, 30);
    timePlusHours(&test2, hours);
    checkTime(&test, test2.hours, test2.minutes, test2.seconds, test2.millis);
    return MUNIT_OK;
}

static MunitResult testTimePlusMinutes(const MunitParameter params[], void *data) {
    // plus minutes from one
    Time test = MIDNIGHT;
    int8_t hour = 0;
    int8_t min = 0;
    for (int i = 0; i < 70; i++) {
        timePlusMinutes(&test, 1);
        min++;
        if (min == 60) {
            hour++;
            min = 0;
        }
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
    }

    for (int8_t i = -70; i < 70; i++) { // plus minutes from zero
        Time base = MIDNIGHT;
        hour = 0;
        min = i;
        timePlusMinutes(&base, i);
        if (i < -60) {
            hour = 22;
            min = i + 120;
        } else if (i < 0) {
            hour = 23;
            min = i + 60;
        } else if (i >= 60) {
            hour = 1;
            min = i - 60;
        }
        assert_int8(base.hours, ==, hour);
        assert_int8(base.minutes, ==, min);
    }

    // no change
    Time base = timeOf(12, 30, 40);
    timePlusMinutes(&base, 0);
    checkTime(&base, 12, 30, 40, 0);

    // add one day
    timePlusMinutes(&base, 24 * 60);
    checkTime(&base, 12, 30, 40, 0);

    // to midnight
    base = timeOf(23, 59, 0);
    timePlusMinutes(&base, 1);
    checkTime(&base, 0, 0, 0, 0);

    // add max minutes
    test = timeOf(2, 30, 0);
    timePlusMinutes(&test, INT32_MAX);
    int8_t minutes = INT32_MAX % (24L * 60L);
    Time test2 = timeOf(2, 30, 0);
    timePlusMinutes(&test2, minutes);
    checkTime(&test, test2.hours, test2.minutes, test2.seconds, test2.millis);
    return MUNIT_OK;
}

static MunitResult testTimePlusSeconds(const MunitParameter params[], void *data) {
    // plus seconds from one
    int8_t hour = 0;
    int8_t min = 0;
    int8_t sec = 0;
    Time test = MIDNIGHT;
    for (int i = 0; i < 3700; i++) {
        timePlusSeconds(&test, 1);
        sec++;
        if (sec == 60) {
            min++;
            sec = 0;
        }
        if (min == 60) {
            hour++;
            min = 0;
        }
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
    }

    // plus seconds from zero
    hour = 22;
    min = 59;
    sec = 0;
    int8_t delta = 30;
    for (int i = -3660; i <= 3660;) {
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
        Time test = MIDNIGHT;
        timePlusSeconds(&test, i);
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
    }

    // no change
    Time base = timeOf(12, 30, 40);
    timePlusSeconds(&base, 0);
    checkTime(&base, 12, 30, 40, 0);

    // add one day
    timePlusSeconds(&base, 24 * 60 * 60);
    checkTime(&base, 12, 30, 40, 0);

    // to midnight
    base = timeOf(23, 59, 59);
    timePlusSeconds(&base, 1);
    checkTime(&base, 0, 0, 0, 0);
    return MUNIT_OK;
}

static MunitResult testTimePlusMillis(const MunitParameter params[], void *data) {
    Time test = MIDNIGHT;
    int8_t hour = 0;
    int8_t min = 0;
    int8_t sec = 0;
    int16_t millis = 0;

    for (int i = 0; i < (3700 * 1000L); i+= 500) {
        timePlusMillis(&test, 500);
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
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
        assert_int16(test.millis, ==, millis);
    }

    // from zero
    int delta = 750;
    int i = -3660 * 1000;
    hour = 22;
    min = 59;
    sec = 0;
    millis = 0;
    while (i <= 3660 * 1000) {
        i += delta;
        millis += delta;

        if (millis >= 1000) {
            sec += millis / 1000;
            millis %= 1000;

            if (sec >= 60) {
                min++;
                sec %= 60;

                if (min == 60) {
                    hour++;
                    min = 0;

                    if (hour == 24) {
                        hour = 0;
                    }
                }
            }
        }
        test = MIDNIGHT;
        timePlusMillis(&test, i);
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
        assert_int8(test.millis, ==, millis);
    }

    // no change
    Time base = timeOf(12, 30, 40);
    timePlusMillis(&base, 0);
    checkTime(&base, 12, 30, 40, 0);

    // add one day
    timePlusMillis(&base, 24 * 60 * 60 * 1000);
    checkTime(&base, 12, 30, 40, 0);

    // to midnight
    base = timeOfWithMillis(23, 59, 59, 999);
    timePlusMillis(&base, 1);
    checkTime(&base, 0, 0, 0, 0);
    return MUNIT_OK;
}

static MunitResult testTimeMinusHours(const MunitParameter params[], void *data) {
    // minus one
    Time test = MIDNIGHT;
    for (int i = 0; i < 50; i++) {
        timeMinusHours(&test, 1);
        assert_int8(test.hours, ==, (((-i + 23) % 24) + 24) % 24);
    }

    for (int i = -50; i < 50; i++) {    // from zero
        Time base = timeOfHourAndMinutes(1, 0);
        timeMinusHours(&base, i);
        assert_int8(base.hours, ==, (1 + (-i % 24) + 24) % 24);
    }
    return MUNIT_OK;
}

static MunitResult testTimeMinusMinutes(const MunitParameter params[], void *data) {
    // minus one
    Time test = MIDNIGHT;
    int8_t hour = 0;
    int8_t min = 0;
    for (int i = 70; i > -70; i--) {
        timeMinusMinutes(&test, 1);
        min--;
        if (min == -1) {
            hour--;
            min = 59;

            if (hour == -1) {
                hour = 23;
            }
        }
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
    }

    hour = 22;
    min = 49;
    for (int i = 70; i > -70; i--) {    // from zero
        Time base = MIDNIGHT;
        timeMinusMinutes(&base, i);
        min++;

        if (min == 60) {
            hour++;
            min = 0;

            if (hour == 24) {
                hour = 0;
            }
        }
        assert_int8(base.hours, ==, hour);
        assert_int8(base.minutes, ==, min);
    }
    return MUNIT_OK;
}

static MunitResult testTimeMinusSeconds(const MunitParameter params[], void *data) {
    // minus one
    Time test = MIDNIGHT;
    int8_t hour = 0;
    int8_t min = 0;
    int8_t sec = 0;
    for (int i = 0; i < 3700; i++) {
        timeMinusSeconds(&test, 1);
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
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
    }

    // from zero
    hour = 22;
    min = 59;
    sec = 0;
    int8_t delta = 30;
    for (int i = 3660; i >= -3660;) {
        i -= delta;
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
        test = MIDNIGHT;
        timeMinusSeconds(&test, i);
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
    }
    return MUNIT_OK;
}

static MunitResult testTimeMinusMillis(const MunitParameter params[], void *data) {
    Time test = MIDNIGHT;
    int8_t hour = 0;
    int8_t min = 0;
    int8_t sec = 0;
    int16_t millis = 0;
    for (int i = 0; i < 3700 * 1000L; i+= 500) {
        timeMinusMillis(&test, 500);
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
        assert_int8(test.hours, ==, hour);
        assert_int8(test.minutes, ==, min);
        assert_int8(test.seconds, ==, sec);
        assert_int16(test.millis, ==, millis);
    }
    return MUNIT_OK;
}

static MunitResult testTimeToSecondsOfDay(const MunitParameter params[], void *data) {
    Time test = MIDNIGHT;
    assert_uint32(timeToSecondsOfDay(NULL), ==, 0); // null check
    for (int i = 0; i < 24 * 60 * 60; i++) {
        assert_uint32(timeToSecondsOfDay(&test), ==, i);
        timePlusSeconds(&test, 1);
    }
    return MUNIT_OK;
}

static MunitResult testTimeToMillisOfDay(const MunitParameter params[], void *data) {
    Time test = MIDNIGHT;
    assert_uint32(timeToMillisOfDay(NULL), ==, 0); // null check
    for (int i = 0; i < 1000000; i++) {
        assert_uint32(timeToMillisOfDay(&test), ==, i);
        timePlusMillis(&test, 1);
    }
    return MUNIT_OK;
}

static MunitResult testTimeCompare(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(COMPARISON_TIME_ARRAY); i++) {
        Time *a = &COMPARISON_TIME_ARRAY[i];
        for (int j = 0; j < ARRAY_SIZE(COMPARISON_TIME_ARRAY); j++) {
            Time *b = &COMPARISON_TIME_ARRAY[j];
            if (i < j) {
                assert_true(timeCompare(a, b) < 0);
                assert_true(isTimeBefore(a, b));
                assert_false(isTimeAfter(a, b));
                assert_false(isTimeEquals(a, b));

            }  else if (i > j) {
                assert_true(timeCompare(a, b) > 0);
                assert_false(isTimeBefore(a, b));
                assert_true(isTimeAfter(a, b));
                assert_false(isTimeEquals(a, b));

            } else {
                assert_true(timeCompare(a, b) == 0);
                assert_false(isTimeBefore(a, b));
                assert_false(isTimeAfter(a, b));
                assert_true(isTimeEquals(a, b));
            }
        }
    }
    return MUNIT_OK;
}

static MunitTest timeTests[] = {
        {.name =  "Test midnight - midnight value should be correctly set", .test = testMidnightTime},
        {.name =  "Test all seconds in day", .test = testAllSecondsInDay},
        {.name =  "Test time factory methods", .test = testCheckTimeFactories},
        {.name =  "Test isTimeValid() - invalid time values should fail", .test = testInvalidTime},
        {.name =  "Test timeOfSecondOfDay() - check that time has been correctly created", .test = testTimeOfSecondsOfDay},
        {.name =  "Test timeOfMillisOfDay() - check that time has been correctly created", .test = testTimeOfMillisOfDay},
        {.name =  "Test testTimeOfMicrosOfDay() - check that time has been correctly created", .test = testTimeOfMicrosOfDay},

        {.name =  "Test timePlusHours() - should correctly add hours", .test = testTimePlusHours},
        {.name =  "Test timePlusMinutes() - should correctly add minutes", .test = testTimePlusMinutes},
        {.name =  "Test timePlusSeconds() - should correctly add seconds", .test = testTimePlusSeconds},
        {.name =  "Test timePlusMillis() - should correctly add milliseconds", .test = testTimePlusMillis},

        {.name =  "Test timeMinusHours() - should correctly subtract hours", .test = testTimeMinusHours},
        {.name =  "Test timeMinusMinutes() - should correctly subtract minutes", .test = testTimeMinusMinutes},
        {.name =  "Test timeMinusSeconds() - should correctly subtract seconds", .test = testTimeMinusSeconds},
        {.name =  "Test timeMinusMillis() - should correctly subtract milliseconds", .test = testTimeMinusMillis},

        {.name =  "Test timeToSecondsOfDay() - should correctly convert time to seconds of day", .test = testTimeToSecondsOfDay},
        {.name =  "Test timeToMillisOfDay() - should correctly convert time to milliseconds of day", .test = testTimeToMillisOfDay},
        {.name =  "Test timeCompare() - should correctly compare two time instances", .test = testTimeCompare},
        END_OF_TESTS
};

static const MunitSuite timeTestSuite = {
        .prefix = "Time: ",
        .tests = timeTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};