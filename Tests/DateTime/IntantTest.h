#pragma once

#include "BaseTestTemplate.h"
#include "Instant.h"

#define MIN_SECONDS INSTANT_MIN_SECONDS
#define MAX_SECONDS INSTANT_MAX_SECONDS

static const int64_t INSTANT_PLUS_SECONDS_PROVIDER[][5] = {
        {0,           0, 0,            0,          0},
        {0,           0, 1,            1,          0},
        {0,           0, -1,           -1,         0},
        {0,           0, MAX_SECONDS, MAX_SECONDS, 0},
        {0,           0, MIN_SECONDS, MIN_SECONDS, 0},
        {1,           0, 0,            1,          0},
        {1,           0, 1,            2,          0},
        {1,           0, -1,           0,          0},
        {1,           0, MAX_SECONDS - 1,          MAX_SECONDS, 0},
        {1,           0, MIN_SECONDS, MIN_SECONDS + 1,          0},
        {1,           1, 0,            1,          1},
        {1,           1, 1,            2,          1},
        {1,           1, -1,           0,          1},
        {1,           1, MAX_SECONDS - 1,          MAX_SECONDS, 1},
        {1,           1, MIN_SECONDS, MIN_SECONDS + 1,          1},
        {-1,          1, 0,            -1,         1},
        {-1,          1, 1,            0,          1},
        {-1,          1, -1,           -2,         1},
        {-1,          1, MAX_SECONDS, MAX_SECONDS - 1,          1},
        {-1,          1, MIN_SECONDS + 1,          MIN_SECONDS, 1},
        {MAX_SECONDS, 2, -MAX_SECONDS, 0,          2},
        {MIN_SECONDS, 2, -MIN_SECONDS, 0,          2},
};

static const int64_t INSTANT_PLUS_MILLIS_PROVIDER[][5] = {
        {0, 0,      0,     0,  0},
        {0, 0,      1,     0,  1000},
        {0, 0,      999,   0,  999000},
        {0, 0,      1000,  1,  0},
        {0, 0,      1001,  1,  1000},
        {0, 0,      1999,  1,  999000},
        {0, 0,      2000,  2,  0},
        {0, 0,      -1,    -1, 999000},
        {0, 0,      -999,  -1, 1000},
        {0, 0,      -1000, -1, 0},
        {0, 0,      -1001, -2, 999000},
        {0, 0,      -1999, -2, 1000},

        {0, 1,      0,     0,  1},
        {0, 1,      1,     0,  1001},
        {0, 1,      998,   0,  998001},
        {0, 1,      999,   0,  999001},
        {0, 1,      1000,  1,  1},
        {0, 1,      1998,  1,  998001},
        {0, 1,      1999,  1,  999001},
        {0, 1,      2000,  2,  1},
        {0, 1,      -1,    -1, 999001},
        {0, 1,      -2,    -1, 998001},
        {0, 1,      -1000, -1, 1},
        {0, 1,      -1001, -2, 999001},

        {0, 100000, 0,     0,  100000},
        {0, 100000, 1,     0,  101000},
        {0, 1000,   998,   0,  999000},
        {0, 1000,   999,   1,  0},
        {0, 100000, 1000,  1,  100000},
        {0, 1000,   1998,  1,  999000},
        {0, 1000,   1999,  2,  0},
        {0, 1000,   2000,  2,  1000},
        {0, 1000,   -1,    0,  0},
        {0, 1000,   -2,    -1, 999000},
        {0, 1000,   -999,  -1, 2000},
        {0, 1000,   -1000, -1, 1000},
        {0, 1000,   -1001, -1, 0},
        {0, 1000,   -1002, -2, 999000},

        {0, 999999, 0,     0,  999999},
        {0, 999999, 1,     1,  999},
        {0, 999999, 999,   1,  998999},
        {0, 999999, 1000,  1,  999999},
        {0, 999999, 1001,  2,  999},
        {0, 999999, -1,    0,  998999},
        {0, 999999, -1000, -1, 999999},
        {0, 999999, -1001, -1, 998999}
};

static const int64_t INSTANT_PLUS_MICROS_PROVIDER[][5] = {
        {0,           0,      0,        0,             0},
        {0,           0,      1,        0,             1},
        {0,           0,      999999,   0,             999999},
        {0,           0,      1000000,  1,             0},
        {0,           0,      1000001,  1,             1},
        {0,           0,      1999999,  1,             999999},
        {0,           0,      2000000,  2,             0},
        {0,           0,      -1,       -1,            999999},
        {0,           0,      -999999,  -1,            1},
        {0,           0,      -1000000, -1,            0},
        {0,           0,      -1000001, -2,            999999},
        {0,           0,      -1999999, -2,            1},

        {1,           0,      0,        1,             0},
        {1,           0,      1,        1,             1},
        {1,           0,      999999,   1,             999999},
        {1,           0,      1000000,  2,             0},
        {1,           0,      1000001,  2,             1},
        {1,           0,      1999999,  2,             999999},
        {1,           0,      2000000,  3,             0},
        {1,           0,      -1,       0,             999999},
        {1,           0,      -999999,  0,             1},
        {1,           0,      -1000000, 0,             0},
        {1,           0,      -1000001, -1,            999999},
        {1,           0,      -1999999, -1,            1},

        {-1,          0,      0,        -1,            0},
        {-1,          0,      1,        -1,            1},
        {-1,          0,      999999,   -1,            999999},
        {-1,          0,      1000000,  0,             0},
        {-1,          0,      1000001,  0,             1},
        {-1,          0,      1999999,  0,             999999},
        {-1,          0,      2000000,  1,             0},
        {-1,          0,      -1,       -2,            999999},
        {-1,          0,      -999999,  -2,            1},
        {-1,          0,      -1000000, -2,            0},
        {-1,          0,      -1000001, -3,            999999},
        {-1,          0,      -1999999, -3,            1},

        {1,           1,      0,        1,             1},
        {1,           1,      1,        1,             2},
        {1,           1,      999998,   1,             999999},
        {1,           1,      999999,   2,             0},
        {1,           1,      1000000,  2,             1},
        {1,           1,      1999998,  2,             999999},
        {1,           1,      1999999,  3,             0},
        {1,           1,      2000000,  3,             1},
        {1,           1,      -1,       1,             0},
        {1,           1,      -2,       0,             999999},
        {1,           1,      -1000000, 0,             1},
        {1,           1,      -1000001, 0,             0},
        {1,           1,      -1000002, -1,            999999},
        {1,           1,      -2000000, -1,            1},

        {1,           999999, 0,        1,             999999},
        {1,           999999, 1,        2,             0},
        {1,           999999, 999999,   2,             999998},
        {1,           999999, 1000000,  2,             999999},
        {1,           999999, 1000001,  3,             0},
        {1,           999999, -1,       1,             999998},
        {1,           999999, -1000000, 0,             999999},
        {1,           999999, -1000001, 0,             999998},
        {1,           999999, -1999999, 0,             0},
        {1,           999999, -2000000, -1,            999999},

        {MAX_SECONDS, 0,      999999,     MAX_SECONDS, 999999},
        {MAX_SECONDS - 1, 0, 1999999,                  MAX_SECONDS,  999999},
        {MIN_SECONDS, 1,      -1,         MIN_SECONDS, 0},
        {MIN_SECONDS + 1, 1, -1000001,                 MIN_SECONDS,  0},

        {0,           0,     MAX_SECONDS, MAX_SECONDS / 1000000,     (int) (MAX_SECONDS % 1000000)},
        {0,           0,     MIN_SECONDS, MIN_SECONDS / 1000000 - 1, (int) (MIN_SECONDS % 1000000) + 1000000},
};

static const int64_t INSTANT_MINUS_SECONDS_PROVIDER[][5] = {
        {0,           0, 0,           0,                        0},
        {0,           0, 1,           -1,                       0},
        {0,           0, -1,          1,                        0},
        {0,           0, -MIN_SECONDS,             MIN_SECONDS, 0},
        {1,           0, 0,           1,                        0},
        {1,           0, 1,           0,                        0},
        {1,           0, -1,          2,                        0},
        {1,           0, -MIN_SECONDS + 1,         MIN_SECONDS, 0},
        {1,           1, 0,           1,                        1},
        {1,           1, 1,           0,                        1},
        {1,           1, -1,          2,                        1},
        {1,           1, -MIN_SECONDS,             MIN_SECONDS + 1, 1},
        {1,           1, -MIN_SECONDS + 1,         MIN_SECONDS, 1},
        {-1,          1, 0,           -1,                       1},
        {-1,          1, 1,           -2,                       1},
        {-1,          1, -1,          0,                        1},
        {-1,          1, -MAX_SECONDS,             MAX_SECONDS - 1, 1},
        {-1,          1, -(MAX_SECONDS + 1),       MAX_SECONDS, 1},

        {MIN_SECONDS, 2, MIN_SECONDS, 0,                        2},
        {MIN_SECONDS + 1, 2,          MIN_SECONDS, 1,               2},
        {MAX_SECONDS - 1, 2,          MAX_SECONDS, -1,              2},
        {MAX_SECONDS, 2, MAX_SECONDS, 0,                        2},
};

static const int64_t INSTANT_MINUS_MILLIS_PROVIDER[][5] = {
        {0, 0,      0,     0,  0},
        {0, 0,      1,     -1, 999000},
        {0, 0,      999,   -1, 1000},
        {0, 0,      1000,  -1, 0},
        {0, 0,      1001,  -2, 999000},
        {0, 0,      1999,  -2, 1000},
        {0, 0,      2000,  -2, 0},
        {0, 0,      -1,    0,  1000},
        {0, 0,      -999,  0,  999000},
        {0, 0,      -1000, 1,  0},
        {0, 0,      -1001, 1,  1000},
        {0, 0,      -1999, 1,  999000},

        {0, 1,      0,     0,  1},
        {0, 1,      1,     -1, 999001},
        {0, 1,      998,   -1, 2001},
        {0, 1,      999,   -1, 1001},
        {0, 1,      1000,  -1, 1},
        {0, 1,      1998,  -2, 2001},
        {0, 1,      1999,  -2, 1001},
        {0, 1,      2000,  -2, 1},
        {0, 1,      -1,    0,  1001},
        {0, 1,      -2,    0,  2001},
        {0, 1,      -1000, 1,  1},
        {0, 1,      -1001, 1,  1001},

        {0, 1000,   0,     0,  1000},
        {0, 1000,   1,     0,  0},
        {0, 1000,   998,   -1, 3000},
        {0, 1000,   999,   -1, 2000},
        {0, 1000,   1000,  -1, 1000},
        {0, 1000,   1998,  -2, 3000},
        {0, 1000,   1999,  -2, 2000},
        {0, 1000,   2000,  -2, 1000},
        {0, 1000,   -1,    0,  2000},
        {0, 1000,   -2,    0,  3000},
        {0, 1000,   -999,  1,  0},
        {0, 1000,   -1000, 1,  1000},
        {0, 1000,   -1001, 1,  2000},
        {0, 1000,   -1002, 1,  3000},

        {0, 999999, 0,     0,  999999},
        {0, 999999, 1,     0,  998999},
        {0, 999999, 999,   0,  999},
        {0, 999999, 1000,  -1, 999999},
        {0, 999999, 1001,  -1, 998999},
        {0, 999999, -1,    1,  999},
        {0, 999999, -1000, 1,  999999},
        {0, 999999, -1001, 2,  999},
};

static const int64_t INSTANT_MINUS_MICROS_PROVIDER[][5] = {
        {0,           0,      0,        0,          0},
        {0,           0,      1,        -1,         999999},
        {0,           0,      999999,   -1,         1},
        {0,           0,      1000000,  -1,         0},
        {0,           0,      1000001,  -2,         999999},
        {0,           0,      1999999,  -2,         1},
        {0,           0,      2000000,  -2,         0},
        {0,           0,      -1,       0,          1},
        {0,           0,      -999999,  0,          999999},
        {0,           0,      -1000000, 1,          0},
        {0,           0,      -1000001, 1,          1},
        {0,           0,      -1999999, 1,          999999},

        {1,           0,      0,        1,          0},
        {1,           0,      1,        0,          999999},
        {1,           0,      999999,   0,          1},
        {1,           0,      1000000,  0,          0},
        {1,           0,      1000001,  -1,         999999},
        {1,           0,      1999999,  -1,         1},
        {1,           0,      2000000,  -1,         0},
        {1,           0,      -1,       1,          1},
        {1,           0,      -999999,  1,          999999},
        {1,           0,      -1000000, 2,          0},
        {1,           0,      -1000001, 2,          1},
        {1,           0,      -1999999, 2,          999999},

        {-1,          0,      0,        -1,         0},
        {-1,          0,      1,        -2,         999999},
        {-1,          0,      999999,   -2,         1},
        {-1,          0,      1000000,  -2,         0},
        {-1,          0,      1000001,  -3,         999999},
        {-1,          0,      1999999,  -3,         1},
        {-1,          0,      2000000,  -3,         0},
        {-1,          0,      -1,       -1,         1},
        {-1,          0,      -999999,  -1,         999999},
        {-1,          0,      -1000000, 0,          0},
        {-1,          0,      -1000001, 0,          1},
        {-1,          0,      -1999999, 0,          999999},

        {1,           1,      0,        1,          1},
        {1,           1,      1,        1,          0},
        {1,           1,      999998,   0,          3},
        {1,           1,      999999,   0,          2},
        {1,           1,      1000000,  0,          1},
        {1,           1,      1999998,  -1,         3},
        {1,           1,      1999999,  -1,         2},
        {1,           1,      2000000,  -1,         1},
        {1,           1,      -1,       1,          2},
        {1,           1,      -2,       1,          3},
        {1,           1,      -1000000, 2,          1},
        {1,           1,      -1000001, 2,          2},
        {1,           1,      -1000002, 2,          3},
        {1,           1,      -2000000, 3,          1},

        {1,           999999, 0,        1,          999999},
        {1,           999999, 1,        1,          999998},
        {1,           999999, 999999,   1,          0},
        {1,           999999, 1000000,  0,          999999},
        {1,           999999, 1000001,  0,          999998},
        {1,           999999, -1,       2,          0},
        {1,           999999, -1000000, 2,          999999},
        {1,           999999, -1000001, 3,          0},
        {1,           999999, -1999999, 3,          999998},
        {1,           999999, -2000000, 3,          999999},

        {MAX_SECONDS, 0,      -999999, MAX_SECONDS, 999999},
        {MAX_SECONDS - 1, 0, -1999999,              MAX_SECONDS, 999999},
        {MIN_SECONDS, 1,      1,       MIN_SECONDS, 0},
        {MIN_SECONDS + 1, 1, 1000001,               MIN_SECONDS, 0},
};

const int COMPARISON_INSTANT_ARRAY[][2] = {
        {-2L, 0},
        {-2L, 999998},
        {-2L, 999999},
        {-1L, 0},
        {-1L, 1},
        {-1L, 999998},
        {-1L, 999999},
        {0L,  0},
        {0L,  1},
        {0L,  2},
        {0L,  999999},
        {1L,  0},
        {2L,  0}
};

typedef struct TestDataStruct {
    int64_t seconds;
    int64_t micro;
    int64_t amount;
    int64_t expectedSeconds;
    int64_t expectedMicroOfSecond;
} TestDataStruct;

struct TestDataStruct getInstantTestData(const int64_t dataProvider[][5], int index) {
    int64_t seconds = dataProvider[index][0];
    int64_t micro = dataProvider[index][1];
    int64_t amount = dataProvider[index][2];
    int64_t expectedSeconds = dataProvider[index][3];
    int64_t expectedMicroOfSecond = dataProvider[index][4];
    return (struct TestDataStruct) {
            .seconds = seconds,
            .micro = micro,
            .amount = amount,
            .expectedSeconds = expectedSeconds,
            .expectedMicroOfSecond = expectedMicroOfSecond};
}


static MunitResult testInstantOfEpochSeconds(const MunitParameter params[], void *data) {
    Instant test = instantOfEpochSeconds(1653582675);
    assert_int64(test.seconds, ==, 1653582675);
    assert_int64(test.micros, ==, 0);
    return MUNIT_OK;
}

static MunitResult testInstantOfEpochSecondsAdjust(const MunitParameter params[], void *data) {
    for (long i = -2; i <= 2; i++) {
        for (int j = 0; j < 10; j++) {
            Instant t = instantOfEpochSecondsAdjust(i, j);
            assert_int64(t.seconds, ==, i);
            assert_int64(t.micros, ==, j);
        }

        for (int j = -10; j < 0; j++) {
            Instant t = instantOfEpochSecondsAdjust(i, j);
            assert_int64(t.seconds, ==, i - 1);
            assert_int64(t.micros, ==, j + 1000000);
        }

        for (int j = 999990; j < 1000000; j++) {
            Instant t = instantOfEpochSecondsAdjust(i, j);
            assert_int64(t.seconds, ==, i);
            assert_int64(t.micros, ==, j);
        }
    }

    Instant test = instantOfEpochSecondsAdjust(2L, -1); // micros negative adjusted
    assert_int64(test.seconds, ==, 1);
    assert_int64(test.micros, ==, 999999);
    return MUNIT_OK;
}

static MunitResult testInstantOfEpochMillis(const MunitParameter params[], void *data) {
    int millisProvider[][3] = {
            {0,     0,  0},
            {1,     0,  1000},
            {2,     0,  2000},
            {999,   0,  999000},
            {1000,  1,  0},
            {1001,  1,  1000},
            {-1,    -1, 999000},
            {-2,    -1, 998000},
            {-999,  -1, 1000},
            {-1000, -1, 0},
            {-1001, -2, 999000},
    };

    for (int i = 0; i < ARRAY_SIZE(millisProvider); i++) {
        int millis = millisProvider[i][0];
        int expectedSeconds = millisProvider[i][1];
        int expectedMicroSeconds = millisProvider[i][2];
        Instant test = instantOfEpochMillis(millis);
        assert_int64(test.seconds, ==, expectedSeconds);
        assert_int64(test.micros, ==, expectedMicroSeconds);
    }
    return MUNIT_OK;
}

static MunitResult testInstantPlusSeconds(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_PLUS_SECONDS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_PLUS_SECONDS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantPlusSeconds(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantPlusMillis(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_PLUS_MILLIS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_PLUS_MILLIS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantPlusMillis(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantPlusMicros(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_PLUS_MICROS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_PLUS_MICROS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantPlusMicros(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantMinusSeconds(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_MINUS_SECONDS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_MINUS_SECONDS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantMinusSeconds(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantMinusMillis(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_MINUS_MILLIS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_MINUS_MILLIS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantMinusMillis(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantMinusMicros(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(INSTANT_MINUS_MICROS_PROVIDER); i++) {
        TestDataStruct dataStruct = getInstantTestData(INSTANT_MINUS_MICROS_PROVIDER, i);
        Instant test = instantOfEpochSecondsAdjust(dataStruct.seconds, dataStruct.micro);
        instantMinusMicros(&test, dataStruct.amount);
        assert_int64(test.seconds, ==, dataStruct.expectedSeconds);
        assert_int64(test.micros, ==, dataStruct.expectedMicroOfSecond);
    }
    return MUNIT_OK;
}

static MunitResult testInstantToEpochMillis(const MunitParameter params[], void *data) {
    Instant test = instantOfEpochSecondsAdjust(1, 1000);
    assert_int64(instantToEpochMillis(&test), ==, 1001);

    test = instantOfEpochSecondsAdjust(1, 2000);
    assert_int64(instantToEpochMillis(&test), ==, 1002);

    test = instantOfEpochSecondsAdjust(1, 567);
    assert_int64(instantToEpochMillis(&test), ==, 1000);

    test = instantOfEpochSecondsAdjust(0L, -1000);
    assert_int64(instantToEpochMillis(&test), ==, -1);

    test = instantOfEpochSecondsAdjust(0L, 1000);
    assert_int64(instantToEpochMillis(&test), ==, 1);

    test = instantOfEpochSecondsAdjust(0L, 999);
    assert_int64(instantToEpochMillis(&test), ==, 0);

    test = instantOfEpochSecondsAdjust(0L, 1);
    assert_int64(instantToEpochMillis(&test), ==, 0);

    test = instantOfEpochSecondsAdjust(0L, 0);
    assert_int64(instantToEpochMillis(&test), ==, 0);

    test = instantOfEpochSecondsAdjust(0L, -1);
    assert_int64(instantToEpochMillis(&test), ==, -1);

    test = instantOfEpochSecondsAdjust(0L, -999);
    assert_int64(instantToEpochMillis(&test), ==, -1);

    test = instantOfEpochSecondsAdjust(0L, -1000);
    assert_int64(instantToEpochMillis(&test), ==, -1);

    test = instantOfEpochSecondsAdjust(0L, -1001);
    assert_int64(instantToEpochMillis(&test), ==, -2);
    return MUNIT_OK;
}

static MunitResult testInstantCompare(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(COMPARISON_INSTANT_ARRAY); i++) {
        Instant a = instantOfEpochSecondsAdjust(COMPARISON_INSTANT_ARRAY[i][0], COMPARISON_INSTANT_ARRAY[i][1]);
        for (int j = 0; j < ARRAY_SIZE(COMPARISON_INSTANT_ARRAY); j++) {
            Instant b = instantOfEpochSecondsAdjust(COMPARISON_INSTANT_ARRAY[j][0], COMPARISON_INSTANT_ARRAY[j][1]);
            if (i < j) {
                assert_true(instantCompare(&a, &b) < 0);
                assert_true(isInstantBefore(&a, &b));
                assert_false(isInstantAfter(&a, &b));
                assert_false(isInstantEquals(&a, &b));

            } else if (i > j) {
                assert_true(instantCompare(&a, &b) > 0);
                assert_false(isInstantBefore(&a, &b));
                assert_true(isInstantAfter(&a, &b));
                assert_false(isInstantEquals(&a, &b));

            } else {
                assert_true(instantCompare(&a, &b) == 0);
                assert_false(isInstantBefore(&a, &b));
                assert_false(isInstantAfter(&a, &b));
                assert_true(isInstantEquals(&a, &b));
            }
        }
    }
    return MUNIT_OK;
}

static MunitTest instantTests[] = {
        {.name =  "Test instantOfEpochSeconds() - should correctly create instant struct", .test = testInstantOfEpochSeconds},
        {.name =  "Test instantOfEpochSecondsAdjust() - should correctly create instant struct with adjust", .test = testInstantOfEpochSecondsAdjust},
        {.name =  "Test instantOfEpochMillis() - should correctly convert millis to instant", .test = testInstantOfEpochMillis},

        {.name =  "Test instantPlusSeconds() - should correctly add seconds to instant", .test = testInstantPlusSeconds},
        {.name =  "Test instantPlusMillis() - should correctly add millis to instant", .test = testInstantPlusMillis},
        {.name =  "Test instantPlusMicros() - should correctly add micros to instant", .test = testInstantPlusMicros},

        {.name =  "Test instantMinusSeconds() - should correctly subtract seconds from instant", .test = testInstantMinusSeconds},
        {.name =  "Test instantMinusMillis() - should correctly subtract millis from instant", .test = testInstantMinusMillis},
        {.name =  "Test instantMinusMicros() - should correctly subtract micros from instant", .test = testInstantMinusMicros},

        {.name =  "Test instantToEpochMillis() - should correctly convert instant to epoch millis", .test = testInstantToEpochMillis},
        {.name =  "Test instantCompare() - should correctly compare two instants", .test = testInstantCompare},
        END_OF_TESTS
};

static const MunitSuite instantTestSuite = {
        .prefix = "Instant: ",
        .tests = instantTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};
