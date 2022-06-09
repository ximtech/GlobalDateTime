#pragma once

#include "BaseTestTemplate.h"
#include "ZonedDateTime.h"

static TimeZone ZONE_RIO_BRANCO = {.id = "America/Rio_Branco", .utcOffset = -18000};


static void checkZonedDateTime(ZonedDateTime test,
                               int year,
                               int month,
                               int day,
                               int hour,
                               int minute,
                               int seconds,
                               int millis,
                               TimeZone zone) {
    assert_int(test.dateTime.date.year, ==, year);
    assert_int(test.dateTime.date.month, ==, month);
    assert_int8(test.dateTime.date.day, ==, day);
    assert_int8(test.dateTime.time.hours, ==, hour);
    assert_int8(test.dateTime.time.minutes, ==, minute);
    assert_int8(test.dateTime.time.seconds, ==, seconds);
    assert_int(test.dateTime.time.millis, ==, millis);
    assert_int(test.zone.utcOffset, ==, zone.utcOffset);
    assert_string_equal(test.zone.id, zone.id);
}

static void assertZonedDateTimeEquals(ZonedDateTime test, ZonedDateTime expected) {
    assert_true(isZonedDateTimeEquals(&test, &expected));
}

static MunitResult testZonedDateTimeFactories(const MunitParameter params[], void *data) {
    ZonedDateTime test = zonedDateTimeOf(2008, 6, 30, 11, 30, 10, 500, &ZONE_PARIS);
    checkZonedDateTime(test, 2008, 6, 30, 11, 30, 10, 500, ZONE_PARIS);

    Date date = dateOf(2008, 6, 30);
    Time time = timeOf(11, 30, 10);
    test = zonedDateTimeFrom(&date, &time, &ZONE_PARIS);
    checkZonedDateTime(test, 2008, 6, 30, 11, 30, 10, 0, ZONE_PARIS);
    test = zonedDateTimeFrom(&date, NULL, &ZONE_PARIS);
    assert_false(isDateTimeValid(&test.dateTime));

    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 11, 30, 10, 500);
    test = zonedDateTimeOfDateTime(&dt, &ZONE_PARIS);
    checkZonedDateTime(test, 2008, 6, 30, 11, 30, 10, 500, ZONE_PARIS);
    test = zonedDateTimeOfDateTime(NULL, &ZONE_PARIS);
    assert_false(isDateTimeValid(&test.dateTime));

    dt = dateTimeOfWithMillis(2008, 6, 30, 11, 30, 10, 35);
    Instant instant = instantOfEpochSecondsAdjust(dateTimeToEpochSecond(&dt, 0), dt.time.millis * MICROS_PER_MILLIS);
    test = zonedDateTimeOfInstant(&instant, &ZONE_PARIS);
    checkZonedDateTime(test, 2008, 6, 30, 11, 30, 10, 35, ZONE_PARIS);
    test = zonedDateTimeOfInstant(NULL, &ZONE_PARIS);
    assert_false(isDateTimeValid(&test.dateTime));

    return MUNIT_OK;
}

static MunitResult testZonedDateTimeWithSameInstant(const MunitParameter params[], void *data) {
    /* March -> 03
    Paris: 01:30, 27 Mar. 2022  -> no gap
    Rio_Branco: 19:30, 26 Mar. 2022 */
    DateTime dateTime = dateTimeOf(2022, 3, 27, 1, 30);
    ZonedDateTime parisZDT = zonedDateTimeOfDateTime(&dateTime, findTimeZone("Europe/Paris"));
    ZonedDateTime rioBrancoZDT = zonedDateTimeWithSameInstant(&parisZDT, findTimeZone("America/Rio_Branco"));
    checkZonedDateTime(parisZDT, 2022, 3, 27, 1, 30, 0, 0, ZONE_PARIS);
    checkZonedDateTime(rioBrancoZDT, 2022, 3, 26, 19, 30, 0, 0, ZONE_RIO_BRANCO);

    /* March -> 03
    Paris: 02:30, 27 Mar. 2022  -> day light saving gap to +1 hour
    Rio_Branco: 20:30, 26 Mar. 2022 */
    dateTime = dateTimeOf(2022, 3, 27, 2, 30);
    parisZDT = zonedDateTimeOfDateTime(&dateTime, findTimeZone("Europe/Paris"));
    rioBrancoZDT = zonedDateTimeWithSameInstant(&parisZDT, findTimeZone("America/Rio_Branco"));
    checkZonedDateTime(parisZDT, 2022, 3, 27, 3, 30, 0, 0, ZONE_PARIS);
    checkZonedDateTime(rioBrancoZDT, 2022, 3, 26, 20, 30, 0, 0, ZONE_RIO_BRANCO);

    /* April -> 04
    Depart : 01:30, 27 Apr. 2022
    Arrive : 18:30, 26 Apr. 2022*/
    dateTime = dateTimeOf(2022, 4, 27, 1, 30);
    parisZDT = zonedDateTimeOfDateTime(&dateTime, findTimeZone("Europe/Paris"));
    rioBrancoZDT = zonedDateTimeWithSameInstant(&parisZDT, findTimeZone("America/Rio_Branco"));
    checkZonedDateTime(parisZDT, 2022, 4, 27, 1, 30, 0, 0, ZONE_PARIS);
    checkZonedDateTime(rioBrancoZDT, 2022, 4, 26, 18, 30, 0, 0, ZONE_RIO_BRANCO);

    /*October -> 10
    Depart : 01:30, 27 Oct. 2022    -> Overlap
    Arrive : 18:30, 26 Oct. 2022*/
    dateTime = dateTimeOf(2022, 10, 27, 1, 30);
    parisZDT = zonedDateTimeOfDateTime(&dateTime, findTimeZone("Europe/Paris"));
    rioBrancoZDT = zonedDateTimeWithSameInstant(&parisZDT, findTimeZone("America/Rio_Branco"));
    checkZonedDateTime(parisZDT, 2022, 10, 27, 1, 30, 0, 0, ZONE_PARIS);
    checkZonedDateTime(rioBrancoZDT, 2022, 10, 26, 18, 30, 0, 0, ZONE_RIO_BRANCO);

    /*February -> 02
    Depart : 01:30, 27 Feb. 2022
    Arrive : 19:30, 26 Feb. 2022*/
    dateTime = dateTimeOf(2022, 2, 27, 1, 30);
    parisZDT = zonedDateTimeOfDateTime(&dateTime, findTimeZone("Europe/Paris"));
    rioBrancoZDT = zonedDateTimeWithSameInstant(&parisZDT, findTimeZone("America/Rio_Branco"));
    checkZonedDateTime(parisZDT, 2022, 2, 27, 1, 30, 0, 0, ZONE_PARIS);
    checkZonedDateTime(rioBrancoZDT, 2022, 2, 26, 19, 30, 0, 0, ZONE_RIO_BRANCO);

    // null check
    ZonedDateTime test = zonedDateTimeWithSameInstant(NULL, findTimeZone("America/Rio_Branco"));
    assert_false(isDateTimeValid(&test.dateTime));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusYears(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusYears(&test, 1);
    dateTimePlusYears(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusMonths(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusMonths(&test, 1);
    dateTimePlusMonths(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusWeeks(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusWeeks(&test, 1);
    dateTimePlusWeeks(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusDays(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusDays(&test, 1);
    dateTimePlusDays(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusHours(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusHours(&test, 1);
    dateTimePlusHours(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusMinutes(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusMinutes(&test, 1);
    dateTimePlusMinutes(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusSeconds(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusSeconds(&test, 1);
    dateTimePlusSeconds(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDatePlusMillis(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimePlusMillis(&test, 123);
    dateTimePlusMillis(&dt, 123);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusYears(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusYears(&test, 1);
    dateTimeMinusYears(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusMonths(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusMonths(&test, 1);
    dateTimeMinusMonths(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusWeeks(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusWeeks(&test, 1);
    dateTimeMinusWeeks(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusDays(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusDays(&test, 1);
    dateTimeMinusDays(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusHours(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusHours(&test, 1);
    dateTimeMinusHours(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusMinutes(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusMinutes(&test, 1);
    dateTimeMinusMinutes(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusSeconds(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusSeconds(&test, 1);
    dateTimeMinusSeconds(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitResult testZonedDateMinusMillis(const MunitParameter params[], void *data) {
    DateTime dt = dateTimeOfWithMillis(2008, 6, 30, 23, 30, 59, 0);
    ZonedDateTime test = zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris"));
    zonedDateTimeMinusMillis(&test, 1);
    dateTimeMinusMillis(&dt, 1);
    assertZonedDateTimeEquals(test, zonedDateTimeOfDateTime(&dt, findTimeZone("Europe/Paris")));
    return MUNIT_OK;
}

static MunitTest zonedDateTimeTests[] = {
        {.name =  "Test zonedDateTimeOf() - should validate and create zoned date-time", .test = testZonedDateTimeFactories},
        {.name =  "Test zonedDateTimeWithSameInstant() - should create new zoned date-time according to rules", .test = testZonedDateTimeWithSameInstant},

        {.name =  "Test zonedDateTimePlusYears() - should correctly add years to zoned date-time", .test = testZonedDatePlusYears},
        {.name =  "Test zonedDateTimePlusMonths() - should correctly add months to zoned date-time", .test = testZonedDatePlusMonths},
        {.name =  "Test zonedDateTimePlusWeeks() - should correctly add weeks to zoned date-time", .test = testZonedDatePlusWeeks},
        {.name =  "Test zonedDateTimePlusDays() - should correctly add days to zoned date-time", .test = testZonedDatePlusDays},
        {.name =  "Test zonedDateTimePlusHours() - should correctly add hours to zoned date-time", .test = testZonedDatePlusHours},
        {.name =  "Test zonedDateTimePlusMinutes() - should correctly add minutes to zoned date-time", .test = testZonedDatePlusMinutes},
        {.name =  "Test zonedDateTimePlusSeconds() - should correctly add seconds to zoned date-time", .test = testZonedDatePlusSeconds},
        {.name =  "Test zonedDateTimePlusMillis() - should correctly add millis to zoned date-time", .test = testZonedDatePlusMillis},

        {.name =  "Test zonedDateTimeMinusYears() - should correctly subtract years from zoned date-time", .test = testZonedDateMinusYears},
        {.name =  "Test zonedDateTimeMinusMonths() - should correctly subtract months from zoned date-time", .test = testZonedDateMinusMonths},
        {.name =  "Test zonedDateTimeMinusWeeks() - should correctly subtract weeks from zoned date-time", .test = testZonedDateMinusWeeks},
        {.name =  "Test zonedDateTimeMinusDays() - should correctly subtract days from zoned date-time", .test = testZonedDateMinusDays},
        {.name =  "Test zonedDateTimeMinusHours() - should correctly subtract hours from zoned date-time", .test = testZonedDateMinusHours},
        {.name =  "Test zonedDateTimeMinusMinutes() - should correctly subtract minutes from zoned date-time", .test = testZonedDateMinusMinutes},
        {.name =  "Test zonedDateTimeMinusSeconds() - should correctly subtract seconds from zoned date-time", .test = testZonedDateMinusSeconds},
        {.name =  "Test zonedDateTimeMinusMillis() - should correctly subtract millis from zoned date-time", .test = testZonedDateMinusMillis},
        END_OF_TESTS
};

static const MunitSuite zonedDateTimeTestSuite = {
        .prefix = "ZonedDateTime: ",
        .tests = zonedDateTimeTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};
