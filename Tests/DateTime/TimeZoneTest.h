#pragma once

#include "BaseTestTemplate.h"
#include "TimeZone.h"


static MunitResult testZoneOf(const MunitParameter params[], void *data) {
    const TimeZone *test = findTimeZone("Indian/Reunion");
    assert_not_null(test);
    assert_string_equal(test->id, "Indian/Reunion");
    assert_int(test->utcOffset, ==, 14400);

    test = findTimeZone("Australia/North");
    assert_not_null(test);
    assert_string_equal(test->id, "Australia/North");
    assert_int(test->utcOffset, ==, 34200);

    test = findTimeZone("error");
    assert_null(test);
    return MUNIT_OK;
}

static MunitResult testResolveTimeZoneOffset(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOf(2008, 4, 12, 7, 23);
    int64_t epoch = dateTimeToEpochSecond(&dt, 0);

    const TimeZone *zone = findTimeZone("Europe/Paris");  // rules check
    int32_t result = resolveTimeZoneOffset(epoch, zone);
    assert_int32(result, ==, 7200);

    dt = dateTimeOf(2022, 3, 27, 1, 30);    // day light saving gap to +1 hour
    epoch = dateTimeToEpochSecond(&dt, 0);
    result = resolveTimeZoneOffset(epoch, zone);
    assert_int32(result, ==, 3600);
    return MUNIT_OK;
}

static MunitResult testResolveTimeZoneName(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOf(2022, 3, 27, 1, 30);    // summer time
    int64_t epoch = dateTimeToEpochSecond(&dt, 0);
    const TimeZone *zone = findTimeZone("Europe/Paris");
    const char *shortName = resolveTimeZoneName(epoch, zone, ZONE_NAME_SHORT);
    assert_string_equal(shortName, "CEST");
    const char *longName = resolveTimeZoneName(epoch, zone, ZONE_NAME_LONG);
    assert_string_equal(longName, "Central European Summer Time");

    dt = dateTimeOf(2022, 2, 27, 1, 30);    // winter
    epoch = dateTimeToEpochSecond(&dt, 0);
    shortName = resolveTimeZoneName(epoch, zone, ZONE_NAME_SHORT);
    assert_string_equal(shortName, "CET");
    longName = resolveTimeZoneName(epoch, zone, ZONE_NAME_LONG);
    assert_string_equal(longName, "Central European Time");

    zone = findTimeZone("Antarctica/Troll");     // GMT+2
    dt = dateTimeOf(2022, 2, 27, 1, 30);
    epoch = dateTimeToEpochSecond(&dt, 0);
    shortName = resolveTimeZoneName(epoch, zone, ZONE_NAME_SHORT);
    assert_string_equal(shortName, "UTC");
    longName = resolveTimeZoneName(epoch, zone, ZONE_NAME_LONG);
    assert_string_equal(longName, "Coordinated Universal Time");
    return MUNIT_OK;
}

static MunitTest timeZoneTests[] = {
        {.name =  "Test findTimeZone() - should validate and find time zone struct", .test = testZoneOf},
        {.name =  "Test resolveTimeZoneOffset() - should correctly resolve zone offset", .test = testResolveTimeZoneOffset},
        {.name =  "Test resolveTimeZoneName() - should correctly resolve zone name with style", .test = testResolveTimeZoneName},
        END_OF_TESTS
};

static const MunitSuite timeZoneTestSuite = {
        .prefix = "TimeZone: ",
        .tests = timeZoneTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};