#pragma once

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "ZonedDateTime.h"

#define DATE_TIME_FORMAT_SIZE 64

// parse error messages
static const char * const FORMATTER_ERROR_UNKNOWN_CHAR = "Unknown pattern character";
static const char * const FORMATTER_ERROR_PATTERN_LENGTH = "Too many pattern letters";
static const char * const FORMATTER_ERROR_RESERVED_CHAR = "Pattern includes reserved character";
static const char * const FORMATTER_ERROR_INCOMPLETE_LITERAL = "Pattern ends with an incomplete string literal";
static const char * const FORMATTER_ERROR_OVERFLOW = "Pattern size too long";

// Available date time patterns
typedef enum ChronoField {
    CHRONO_END_OF_PATTERN,
    CHRONO_FIELD_ERA,                   // 'G' [4]
    CHRONO_FIELD_YEAR_OF_ERA,           // 'y' [5]
    CHRONO_FIELD_MONTH_IN_YEAR,         // 'M' [5]
    CHRONO_FIELD_WEEK_IN_YEAR,          // 'w' [2]
    CHRONO_FIELD_WEEK_IN_MONTH,         // 'W' [1]
    CHRONO_FIELD_DAY_IN_YEAR,           // 'D' [3]
    CHRONO_FIELD_DAY_IN_MONTH,          // 'd' [2]
    CHRONO_FIELD_DAY_OF_WEEK_IN_MONTH,  // 'F' [1]
    CHRONO_FIELD_DAY_IN_WEEK,           // 'E' [5]
    CHRONO_FIELD_YEAR,                  // 'u' [5]
    CHRONO_FIELD_AMPM_OF_DAY,           // 'a' [1]
    CHRONO_FIELD_HOUR_OF_DAY,           // 'H' [2]
    CHRONO_FIELD_CLOCK_HOUR_OF_DAY,     // 'k' [2]
    CHRONO_FIELD_HOUR_OF_AMPM,          // 'K' [2]
    CHRONO_FIELD_CLOCK_HOUR_OF_AMPM,    // 'h' [2]
    CHRONO_FIELD_MINUTE_OF_HOUR,        // 'm' [2]
    CHRONO_FIELD_SECOND_OF_MINUTE,      // 's' [2]
    CHRONO_FIELD_MILLISECOND,           // 'S' [3]
    CHRONO_FIELD_TEXT_TIME_ZONE,        // 'z' [4]
    CHRONO_FIELD_DIGIT_TIME_ZONE,       // 'Z' [5]
    CHRONO_FIELD_LITERAL,               // '.', ':', ',' etc. Also all escaped literals
} ChronoField;

typedef struct TemporalField {
    ChronoField field;
    uint8_t length;
    char literal;
} TemporalField;

typedef struct DateTimeFormatter {
    TemporalField formatEntities[DATE_TIME_FORMAT_SIZE];
    bool isFormatValid;
    const char *errorMessage;
    uint32_t errorAtIndex;
} DateTimeFormatter;

/* This method parses the ID producing a TimeZone struct.
 * A TimeZone is also returned if the ID is 'Z', or starts with '+' or '-'*/
TimeZone timeZoneOf(const char *zoneId);

void parseDateTimePattern(DateTimeFormatter *formatter, const char *pattern);

Time parseToTime(const char *text, DateTimeFormatter *formatter);
Date parseToDate(const char *text, DateTimeFormatter *formatter);
DateTime parseToDateTime(const char *text, DateTimeFormatter *formatter);
ZonedDateTime parseToZonedDateTime(const char *text, DateTimeFormatter *formatter);

void formatTime(Time *time, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter);
void formatDate(Date *date, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter);
void formatDateTime(DateTime *dateTime, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter);
void formatZonedDateTime(ZonedDateTime *zonedDateTime, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter);