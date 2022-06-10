#pragma once

#include "BaseTestTemplate.h"
#include "GlobalDateTime.h"

typedef struct PatternCompileTest {
    const char *pattern;
    const char *errorMessage;
    uint32_t errorAtIndex;
} PatternCompileTest;

typedef struct FormatTest {
    const char *pattern;
    const char *formattedString;
} FormatTest;

typedef struct ParseTest {
    const char *text;
    const char *pattern;
    int dtValues[8];    // year, month, day, hour, minute, second, millis, zoneOffset
} ParseTest;

typedef struct ZoneTest {
    const char *text;
    char *id;
    int offset;
} ZoneTest;


static Time TIME_2_3_4_5 = {.hours = 2, .minutes = 3, .seconds = 4, .millis = 5};

static Date DATE_5_7_4 = {.year = 5, .month = 7, .day = 4, .weekDay = WEDNESDAY};

static DateTime DATE_TIME_2001_7_22_18_8_56_576 = {
        .date = {.year = 2001, .month = 7, .day = 22, .weekDay = WEDNESDAY},
        .time = {.hours = 18, .minutes = 8, .seconds = 56, .millis = 576}};

static ZonedDateTime ZDT_2001_7_4_12_8_56_456_LOS_ANGELES = {
        .dateTime.date = {.year = 2001, .month = 7, .day = 4, .weekDay = WEDNESDAY},
        .dateTime.time = {.hours = 12, .minutes = 8, .seconds = 56, .millis = 567},
        .zone = {.id = "America/Los_Angeles", .utcOffset = -28800, .names = PST_ZONE_NAMES, .rules = AMERICA_LOS_ANGELES_RULES}};

static const ZoneTest TIME_ZONE_PARSE_TEST[] = {
        {.text = "Z", .id = "UTC", .offset = 0},
        {.text = "UTC", .id = "UTC", .offset = 0},
        {.text = "utc", .id = "UTC", .offset = 0},
        {.text = "utC", .id = "UTC", .offset = 0},
        {.text = "GMT", .id = "GMT", .offset = 0},
        {.text = "gmt", .id = "GMT", .offset = 0},
        {.text = "gMt", .id = "GMT", .offset = 0},
        {.text = "GMT+3", .id = "GMT", .offset = 10800},
        {.text = "GMT+2", .id = "GMT", .offset = 7200},
        {.text = "GMT+8", .id = "GMT", .offset = 28800},
        {.text = "GMT-8", .id = "GMT", .offset = -28800},
        {.text = "GMT-2", .id = "GMT", .offset = -7200},
        {.text = "UTC-2", .id = "UTC", .offset = -7200},
        {.text = "UTC+8", .id = "UTC", .offset = 28800},
        {.text = "UTC+08:00", .id = "UTC", .offset = 28800},
        {.text = "UTC+08:25", .id = "UTC", .offset = 30300},
        {.text = "UTC+08:25:10", .id = "UTC", .offset = 30310},
        {.text = "UTC-2:00", .id = "UTC", .offset = -7200},
        {.text = "UTC-2:20", .id = "UTC", .offset = -8400},
        {.text = "UTC-2:20:10", .id = "UTC", .offset = -8410},
        {.text = "GMT+0800", .id = "GMT", .offset = 28800},
        {.text = "GMT+08:00", .id = "GMT", .offset = 28800},
        {.text = "GMT+08:15", .id = "GMT", .offset = 29700},
        {.text = "GMT+08:15:15", .id = "GMT", .offset = 29715},
        {.text = "+0800", .id = "UTC", .offset = 28800},
        {.text = "-0800", .id = "UTC", .offset = -28800},
        {.text = "-0810", .id = "UTC", .offset = -29400},
        {.text = "+08:00", .id = "UTC", .offset = 28800},
        {.text = "-08:00", .id = "UTC", .offset = -28800},
        {.text = "-08:10", .id = "UTC", .offset = -29400},
        {.text = "-08:10:15", .id = "UTC", .offset = -29415},
        {.text = "Europe/Paris", .id = "Europe/Paris", .offset = 7200},
        {.text = "Asia/Chita", .id = "Asia/Chita", .offset = 32400},
};

static const PatternCompileTest INVALID_PATTERN_COMPILE_TEST[] = {
        {"hh:mm Y",                                                              "Unknown pattern character",                      6},
        {"mmmmmm",                                                               "Too many pattern letters",                       5},
        {"hh:mm {",                                                              "Pattern includes reserved character",            5},
        {"hh:{mm",                                                               "Pattern includes reserved character",            2},
        {"hh:#mm",                                                               "Pattern includes reserved character",            2},
        {"hh:mm 'asd",                                                           "Pattern ends with an incomplete string literal", 9},
        {"'toooooo looooooong teeeeeeeext paaaaaaaaaaaaaaaattttttttteeeeernnn'", "Pattern size too long",                          66},
};

static const FormatTest TIME_FORMAT_PATTERN_TEST[] = {
        {"H:m:s:S",       "2:3:4:5"},
        {"HH:mm:ss:SS",   "02:03:04:05"},
        {"h:mm:ss:SSS a", "2:03:04:005 AM"},
        {"hh:mm:ss:SS a", "02:03:04:05 AM"},
        {"kk:mm:ss:SS a", "02:03:04:05 AM"},
        {"k:mm:ss:S",     "2:03:04:5"},
        {"K:mm:ss:S",     "2:03:04:5"},
        {"KK:mm:ss:S",    "02:03:04:5"},
};

static const FormatTest DATE_FORMAT_PATTERN_TEST[] = {
        {"y.M.d E",           "5.7.4 Wed"},
        {"yy.MM.dd EE",       "05.07.04 Wed"},
        {"yyy.MMM.dd EEE",    "005.Jul.04 Wed"},
        {"yyyy.MMMM.dd EEEE", "0005.July.04 Wednesday"},
        {"[w] [ww]",          "[27] [27]"},
        {"[W]",               "[1]"},
        {"[D] [DD] [DDD]",    "[185] [185] [185]"},
        {"[F]",               "[1]"},
};

static const FormatTest DATE_TIME_FORMAT_PATTERN_TEST[] = {
        {"E, y/M/d h:m:s S a",                        "Wed, 2001/7/22 6:8:56 576 PM"},
        {"EE, yy/MM/dd hh:mm:ss SS a",                "Wed, 01/07/22 06:08:56 576 PM"},
        {"EEE, yyy/MMM/dd hh:mm:ss SSS a",            "Wed, 2001/Jul/22 06:08:56 576 PM"},
        {"EEEE, yyyy/MMMM/dd hh:mm:ss SSS",           "Wednesday, 2001/July/22 06:08:56 576"},
        {"EEEE, yyyyy/MMMM/dd H:mm:ss SSS",           "Wednesday, 02001/July/22 18:08:56 576"},
        {"EEEEE, yyyy/MMMM/dd HH:mm:ss SSS",          "W, 2001/July/22 18:08:56 576"},
        {"EEEEE, yyyy/MMMM/dd [W] HH:mm:ss SSS",      "W, 2001/July/22 [4] 18:08:56 576"},
        {"EEEEE, yyyy/MMMM/dd [W]/[ww] HH:mm:ss SSS", "W, 2001/July/22 [4]/[29] 18:08:56 576"},
        {"EEEEE, yyyy/MMMM/dd [D]/[F] HH:mm:ss SSS",  "W, 2001/July/22 [203]/[7] 18:08:56 576"},
};

static const FormatTest ZONED_DATE_TIME_FORMAT_PATTERN_TEST[] = {
        {"HH:mm",                                                    "12:08"},
        {"HH:mm:ss",                                                 "12:08:56"},
        {"HH:mm:ss:SSS",                                             "12:08:56:567"},
        {"yyyy.MM.dd G 'at' HH:mm:ss z",                             "2001.07.04 CE at 12:08:56 PDT"},
        {"y.MM.dd G 'at' HH:mm:ss z",                                "2001.07.04 CE at 12:08:56 PDT"},
        {"yyy.MM.dd G 'at' HH:mm:ss z",                              "2001.07.04 CE at 12:08:56 PDT"},
        {"uuuu.MM.dd G 'at' HH:mm:ss z",                             "2001.07.04 CE at 12:08:56 PDT"},
        {"u.MM.dd G 'at' HH:mm:ss z",                                "2001.07.04 CE at 12:08:56 PDT"},
        {"h:mm a",                                                   "12:08 PM"},
        {"E, M d, yy",                                               "Wed, 7 4, 01"},
        {"EE, MM d, yy",                                             "Wed, 07 4, 01"},
        {"EEE, MMM d, yy",                                           "Wed, Jul 4, 01"},
        {"EEEE, MMMM d, yy",                                         "Wednesday, July 4, 01"},
        {"EEEEE, MMMMM d, yy",                                       "W, J 4, 01"},
        {"hh 'o''clock' a, zzzz",                                    "12 o'clock PM, Pacific Daylight Time"},
        {"yyyyy.MMMMM.dd GGG hh:mm a",                               "02001.J.04 CE 12:08 PM"},
        {"EEE, d MMMM yyyy HH:mm:ss Z",                              "Wed, 4 July 2001 12:08:56 -0700"},
        {"yyMMddHHmmssZ",                                            "010704120856-0700"},
        {"yyyy-MM-dd'T'HH:mm:ss.SSS Z",                              "2001-07-04T12:08:56.567 -0700"},
        {"yyyy-MM-dd'T'HH:mm:ss.SSS ZZZZ",                           "2001-07-04T12:08:56.567 GMT-07:00"},
        {"yyyy-MM-dd'T'HH:mm:ss.SSS ZZZZZ",                          "2001-07-04T12:08:56.567 -07:00"},
        {"'Week of year(num)': [w] [ww], 'Week of month(num)': [W]", "Week of year(num): [27] [27], Week of month(num): [1]"},
        {"'Day of year(num)': [D] [DD] [DDD]",                       "Day of year(num): [185] [185] [185]"},
        {"'Day of Week in Month(num)': [F]",                         "Day of Week in Month(num): [3]"},
        {"'Hour [0-11]': [k] [kk]. 'Hour [1-24]': [K] [KK]",         "Hour [0-11]: [12] [12]. Hour [1-24]: [0] [00]"},
};

static const ParseTest TIME_PARSE_TEST[] = {
        {.text = "1:4:5:6", .pattern = "H:m:s:S", .dtValues = {1, 4, 5, 6}},
        {.text = "13:45:56:67", .pattern = "HH:mm:ss:SS", .dtValues = {13, 45, 56, 67}},
        {.text = "13:45:56:678", .pattern = "HH:mm:ss:SSS", .dtValues = {13, 45, 56, 678}},
        {.text = "1:45:56:678 PM", .pattern = "h:mm:ss:SSS a", .dtValues = {13, 45, 56, 678}},
        {.text = "11:45:56:678 AM", .pattern = "hh:mm:ss:SSS a", .dtValues = {11, 45, 56, 678}},
};

static const ParseTest DATE_PARSE_TEST[] = {
        {.text = "Tue 1/7/3", .pattern = "E y/M/d", .dtValues = {1, 7, 3}},
        {.text = "Sat 11/07/23", .pattern = "EE yy/MM/dd", .dtValues = {11, 7, 23}},
        {.text = "Thu 111/Jul/23", .pattern = "EEE yyy/MMM/dd", .dtValues = {111, 7, 23}},
        {.text = "Saturday 2011/July/23", .pattern = "EEEE yyyy/MMMM/dd", .dtValues = {2011, 7, 23}},
        {.text = "S 2011/July/23", .pattern = "EEEEE yyyy/MMMM/dd", .dtValues = {2011, 7, 23}},
};

static const ParseTest DATE_TIME_PARSE_TEST[] = {
        {.text = "Mon 2001/7/23 13:44:54:456", .pattern = "E yyyy/M/dd HH:mm:ss:SSS", .dtValues = {2001, 7, 23, 13, 44, 54, 456}},
        {.text = "MoN 02001/07/23 01:44:54:456 PM", .pattern = "EE yyyyy/MM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 13, 44, 54, 456}},
        {.text = "mon 2001/Jul/23 01:44:54:456 PM", .pattern = "EEE yyyy/MMM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 13, 44, 54, 456}},
        {.text = "Monday 2001/july/23 01:44:54:456 PM", .pattern = "EEEE yyyy/MMMM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 13, 44, 54, 456}},
        {.text = "mOndAy 2001/JuLy/23 01:44:54:456 pm", .pattern = "EEEE yyyy/MMMM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 13, 44, 54, 456}},
        {.text = "M 2001/JULY/23 01:44:54:456 AM", .pattern = "EEEEE yyyy/MMMM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 1, 44, 54, 456}},
        {.text = "M 2001/JULY/23 12:44:54:456 am", .pattern = "EEEEE yyyy/MMMM/dd hh:mm:ss:SSS a", .dtValues = {2001, 7, 23, 0, 44, 54, 456}},
};

static const ParseTest DATE_TIME_INVALID_PARSE_TEST[] = {
        {.text = "M 2001/JULY/23 01:44:54:456 AMM", .pattern = "EEEEE yyyy/MMMM/dd hh:mm:ss:SSS a"},        // invalid am pm
        {.text = "M 2001/JULY/23 01:44:54:456 A", .pattern = "EEEEE yyyy/MMMM/dd hh:mm:ss:SSS a"},          // invalid am
        {.text = "M 2001/JULY/23 01:44:54:456 ZM", .pattern = "EEEEE yyyy/MMMM/dd hh:mm:ss:SSS a"},         // unknown am pm
        {.text = "Mon 2001/Julianusss/23 01:44:54:456 PM", .pattern = "EEE yyyy/MMMM/dd hh:mm:ss:SSS a"},   // invalid month full name
        {.text = "Mon 2001/Juk/23 01:44:54:456 PM", .pattern = "EEE yyyy/MMM/dd hh:mm:ss:SSS a"},           // invalid month short name
        {.text = "Mon 2001/13/23 01:44:54:456 PM", .pattern = "EEE yyyy/MM/dd hh:mm:ss:SSS a"},             // invalid month value
        {.text = "Thu 2001/07/23 01:44:54:456 PM", .pattern = "EEE yyyy/MM/dd hh:mm:ss:SSS a"},             // invalid week day, should be monday
        {.text = "Mon 2001/07/23 01:44:54:456", .pattern = "EEE yyyy/MM/dd hh:mm:ss:SSS"},                  // missing am pm
        {.text = "Mon 2001/07/23 01:44:54:456 PM", .pattern = "EEE yyyy/MM/dd HH:mm:ss:SSS a"},             // hours[0-23] with am pm
        {.text = "Mon 2001/07/23 01:22:54:456 PM", .pattern = "EEE yyyy/MM/dd HH:hh:ss:SSS a"},             // hours[0-23] with hours[1-12]
};

static const ParseTest ZONED_DATE_TIME_PARSE_TEST[] = {
        {.text = "Mon 2001/7/23 13:44:54:456 Z", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 0}},
        {.text = "Mon 2001/7/23 13:44:54:456 GMT", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 0}},
        {.text = "Mon 2001/7/23 13:44:54:456 +0800", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 28800}},
        {.text = "Mon 2001/7/23 13:44:54:456 +0825", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 -0800", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, -28800}},
        {.text = "Mon 2001/7/23 13:44:54:456 -0825", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, -30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 GMT+0800", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 28800}},
        {.text = "Mon 2001/7/23 13:44:54:456 GMT+0825", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 GMT-0800", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, -28800}},
        {.text = "Mon 2001/7/23 13:44:54:456 GMT-0825", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, -30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 +08:25", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 -08:25", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, -30300}},
        {.text = "Mon 2001/7/23 13:44:54:456 +02:45:45", .pattern = "E yyyy/M/dd HH:mm:ss:SSS Z", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 9945}},
        {.text = "Mon 2001/7/23 13:44:54:456 [Europe/Zurich]", .pattern = "E yyyy/M/dd HH:mm:ss:SSS [Z]", .dtValues = {2001, 7, 23, 13, 44, 54, 456, 7200}},
};

static MunitResult testTimeZoneOf(const MunitParameter params[], void *data) {
    for (int i = 0; i < ARRAY_SIZE(TIME_ZONE_PARSE_TEST); i++) {
        ZoneTest expected = TIME_ZONE_PARSE_TEST[i];
        TimeZone test = timeZoneOf(expected.text);
        assert_not_null(test.id);
        assert_string_equal(test.id, expected.id);
        assert_int(test.utcOffset, ==, expected.offset);
    }
    return MUNIT_OK;
}


static MunitResult testInvalidFormatPattern(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(INVALID_PATTERN_COMPILE_TEST); i++) {
        parseDateTimePattern(&formatter, INVALID_PATTERN_COMPILE_TEST[i].pattern);
        assert_false(formatter.isFormatValid);
        assert_string_equal(formatter.errorMessage, INVALID_PATTERN_COMPILE_TEST[i].errorMessage);
        assert_int(formatter.errorAtIndex, ==, INVALID_PATTERN_COMPILE_TEST[i].errorAtIndex);
    }
    return MUNIT_OK;
}

static MunitResult testTimeFormat(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    char buffer[64];
    for (int i = 0; i < ARRAY_SIZE(TIME_FORMAT_PATTERN_TEST); i++) {
        const char *pattern = TIME_FORMAT_PATTERN_TEST[i].pattern;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        memset(buffer, 0, 64);
        formatTime(&TIME_2_3_4_5, buffer, 64, &formatter);
        assert_string_equal(buffer, TIME_FORMAT_PATTERN_TEST[i].formattedString);
    }
    return MUNIT_OK;
}

static MunitResult testDateFormat(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    char buffer[64];
    for (int i = 0; i < ARRAY_SIZE(DATE_FORMAT_PATTERN_TEST); i++) {
        const char *pattern = DATE_FORMAT_PATTERN_TEST[i].pattern;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        memset(buffer, 0, 64);
        formatDate(&DATE_5_7_4, buffer, 64, &formatter);
        assert_string_equal(buffer, DATE_FORMAT_PATTERN_TEST[i].formattedString);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeFormat(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    char buffer[64];
    for (int i = 0; i < ARRAY_SIZE(DATE_TIME_FORMAT_PATTERN_TEST); i++) {
        const char *pattern = DATE_TIME_FORMAT_PATTERN_TEST[i].pattern;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        memset(buffer, 0, 64);
        formatDateTime(&DATE_TIME_2001_7_22_18_8_56_576, buffer, 64, &formatter);
        assert_string_equal(buffer, DATE_TIME_FORMAT_PATTERN_TEST[i].formattedString);
    }
    return MUNIT_OK;
}

static MunitResult testZDTFormatPattern(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    char buffer[64];
    for (int i = 0; i < ARRAY_SIZE(ZONED_DATE_TIME_FORMAT_PATTERN_TEST); i++) {
        const char *pattern = ZONED_DATE_TIME_FORMAT_PATTERN_TEST[i].pattern;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        memset(buffer, 0, 64);
        formatZonedDateTime(&ZDT_2001_7_4_12_8_56_456_LOS_ANGELES, buffer, 64, &formatter);
        assert_string_equal(buffer, ZONED_DATE_TIME_FORMAT_PATTERN_TEST[i].formattedString);
    }

    // Check for seconds zone '+' sign
    TimeZone zoneWithMinutes = {.id = "Custom_Zone", .utcOffset = 30900, NULL, NULL};    // 8h 35m
    ZonedDateTime test = zonedDateTimeOf(2011, JULY, 23, 15, 45, 34, 456, &zoneWithMinutes);
    parseDateTimePattern(&formatter, "yyyy-MM-dd'T'HH:mm:ss.SSS ZZZZZ");
    assert_true(formatter.isFormatValid);

    memset(buffer, 0, 64);
    formatZonedDateTime(&test, buffer, 64, &formatter);
    assert_string_equal(buffer, "2011-07-23T15:45:34.456 +08:35");

    // Check for seconds zone: +02:35:45
    TimeZone zoneWithSeconds = {.id = "Custom_Zone", .utcOffset = 9345, NULL, NULL};    // 2h 35m 45s
    test = zonedDateTimeOf(2011, JULY, 23, 15, 45, 34, 456, &zoneWithSeconds);
    parseDateTimePattern(&formatter, "yyyy-MM-dd'T'HH:mm:ss.SSS ZZZZZ");
    assert_true(formatter.isFormatValid);

    memset(buffer, 0, 64);
    formatZonedDateTime(&test, buffer, 64, &formatter);
    assert_string_equal(buffer, "2011-07-23T15:45:34.456 +02:35:45");

    return MUNIT_OK;
}

static MunitResult testTimeParse(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(TIME_PARSE_TEST); i++) {
        const char *pattern = TIME_PARSE_TEST[i].pattern;
        const char *text = TIME_PARSE_TEST[i].text;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        Time test = parseToTime(text, &formatter);
        assert_true(isTimeValid(&test));
        Time expected = timeOfWithMillis(
                TIME_PARSE_TEST[i].dtValues[0],
                TIME_PARSE_TEST[i].dtValues[1],
                TIME_PARSE_TEST[i].dtValues[2],
                TIME_PARSE_TEST[i].dtValues[3]);
        assertTimeEquals(test, expected);
    }
    return MUNIT_OK;
}

static MunitResult testDateParse(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(DATE_PARSE_TEST); i++) {
        const char *pattern = DATE_PARSE_TEST[i].pattern;
        const char *text = DATE_PARSE_TEST[i].text;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        Date test = parseToDate(text, &formatter);
        assert_true(isDateValid(&test));
        Date expected = dateOf(
                DATE_PARSE_TEST[i].dtValues[0],
                DATE_PARSE_TEST[i].dtValues[1],
                DATE_PARSE_TEST[i].dtValues[2]);
        assertDatesEquals(test, expected);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeParse(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(DATE_TIME_PARSE_TEST); i++) {
        const char *pattern = DATE_TIME_PARSE_TEST[i].pattern;
        const char *text = DATE_TIME_PARSE_TEST[i].text;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        DateTime test = parseToDateTime(text, &formatter);
        assert_true(isDateTimeValid(&test));
        DateTime expected = dateTimeOfWithMillis(
                DATE_TIME_PARSE_TEST[i].dtValues[0],
                DATE_TIME_PARSE_TEST[i].dtValues[1],
                DATE_TIME_PARSE_TEST[i].dtValues[2],
                DATE_TIME_PARSE_TEST[i].dtValues[3],
                DATE_TIME_PARSE_TEST[i].dtValues[4],
                DATE_TIME_PARSE_TEST[i].dtValues[5],
                DATE_TIME_PARSE_TEST[i].dtValues[6]);
        assertDateTimeEquals(test, expected);
        assert_int(test.date.weekDay, ==, expected.date.weekDay);
    }
    return MUNIT_OK;
}

static MunitResult testDateTimeParseInvalid(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(DATE_TIME_INVALID_PARSE_TEST); i++) {
        const char *pattern = DATE_TIME_INVALID_PARSE_TEST[i].pattern;
        const char *text = DATE_TIME_INVALID_PARSE_TEST[i].text;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);
        DateTime test = parseToDateTime(text, &formatter);
        assert_false(isDateTimeValid(&test));
    }
    return MUNIT_OK;
}

static MunitResult testZDTParseByPattern(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    for (int i = 0; i < ARRAY_SIZE(ZONED_DATE_TIME_PARSE_TEST); i++) {
        const char *pattern = ZONED_DATE_TIME_PARSE_TEST[i].pattern;
        const char *text = ZONED_DATE_TIME_PARSE_TEST[i].text;
        parseDateTimePattern(&formatter, pattern);
        assert_true(formatter.isFormatValid);

        ZonedDateTime test = parseToZonedDateTime(text, &formatter);
        assert_true(isDateTimeValid(&test.dateTime));
        DateTime expected = dateTimeOfWithMillis(
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[0],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[1],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[2],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[3],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[4],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[5],
                ZONED_DATE_TIME_PARSE_TEST[i].dtValues[6]);
        assertDateTimeEquals(test.dateTime, expected);
        assert_int(test.zone.utcOffset, ==, ZONED_DATE_TIME_PARSE_TEST[i].dtValues[7]);
    }
    return MUNIT_OK;
}

static MunitResult testZDTParseConflicts(const MunitParameter params[], void *data) {
    DateTimeFormatter formatter;
    parseDateTimePattern(&formatter, "yyyy.MM.dd hh:mm:ss:SSS Z");
    assert_true(formatter.isFormatValid);
    ZonedDateTime test = parseToZonedDateTime("2001.07.04 12:08:56:567 -0800", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // Am Pm missing

    parseDateTimePattern(&formatter, "yyyy.MM.dd HH:mm:ss:SSS a Z");
    assert_true(formatter.isFormatValid);
    test = parseToZonedDateTime("2001.07.04 12:08:56:567 AM -0800", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // Am Pm with hour of day [0-23]

    parseDateTimePattern(&formatter, "yyyy.MM.dd hh:HH:mm:ss:SSS a Z");
    assert_true(formatter.isFormatValid);
    test = parseToZonedDateTime("2001.07.04 01:12:08:56:567 AM -0800", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // hour of day with Am Pm hours

    parseDateTimePattern(&formatter, "yyyy.MM.dd HH:mm:ss:SSS Z");
    assert_true(formatter.isFormatValid);
    test = parseToZonedDateTime("2001.07.04 01:12:08:567 -08:a0", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // invalid zone digits

    parseDateTimePattern(&formatter, "yyyy.MM.dd HH:mm:ss:SSS Z");
    assert_true(formatter.isFormatValid);
    test = parseToZonedDateTime("2001.07.04 01:12:08:567 +AA:25", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // invalid zone digits

    parseDateTimePattern(&formatter, "yyyy.MM.dd HH:mm:ss:SSS Z");
    assert_true(formatter.isFormatValid);
    test = parseToZonedDateTime("2001.07.04 01:12:08:567 Indian/MayotteZ", &formatter);
    assert_false(isDateTimeValid(&test.dateTime)); // invalid zone full name
    return MUNIT_OK;
}

static MunitTest dateTimeFormatterTests[] = {
        {.name =  "Test timeZoneOf() - should correctly parse and return time zone", .test = testTimeZoneOf},
        {.name =  "Test parseDateTimePattern() - when invalid pattern should fail", .test = testInvalidFormatPattern},

        {.name =  "Test formatTime() - should correctly format Time struct to string", .test = testTimeFormat},
        {.name =  "Test formatDate() - should correctly format Date struct to string", .test = testDateFormat},
        {.name =  "Test formatDateTime() - should correctly format DateTime struct to string", .test = testDateTimeFormat},
        {.name =  "Test formatZonedDateTime() - should correctly format ZonedDateTime struct to string", .test = testZDTFormatPattern},

        {.name =  "Test parseToTime() - should correctly parse string to Time struct", .test = testTimeParse},
        {.name =  "Test parseToDate() - should correctly parse string to Date struct", .test = testDateParse},
        {.name =  "Test parseToDateTime() - should correctly parse string to DateTime struct", .test = testDateTimeParse},
        {.name =  "Test parseToDateTime() - should fail for invalid string", .test = testDateTimeParseInvalid},
        {.name =  "Test parseToZonedDateTime() - should correctly parse string to ZonedDateTime struct", .test = testZDTParseByPattern},
        {.name =  "Test parseToZonedDateTime() - check for parsing date-time conflicts", .test = testZDTParseConflicts},
        END_OF_TESTS
};

static const MunitSuite dateTimeFormatterTestSuite = {
        .prefix = "DateTimeZF: ",
        .tests = dateTimeFormatterTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};








