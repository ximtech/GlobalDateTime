#include "include/DateTimeZF.h"

#define MONTH_NAME_MAX_LENGTH 9
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define INVALID_NUMBER_VALUE (-1)
#define TEXT_NOT_PARSED 0

#define PATTERN_LENGTH_ONE   1
#define PATTERN_LENGTH_TWO   2
#define PATTERN_LENGTH_THREE 3
#define PATTERN_LENGTH_FOUR  4
#define PATTERN_LENGTH_FIVE  5

#define ISO8601_UTC_INDICATOR 'Z'
#define PREFIX_LENGTH 3
#define ZONE_NAME_MAX_LENGTH 32
#define ZONE_DOUBLE_DIGIT_COUNT 2
#define ZONE_SINGLE_DIGIT_COUNT 1
#define ZONE_DIGIT_COUNT_WITH_SECONDS_SEPARATOR 7
#define AM_PM_LENGTH 2
#define ABBREVIATED_DAY_OF_WEEK_LENGTH 3
#define WIDE_DAY_OF_WEEK_MAX_LENGTH 9
#define NARROW_DAY_OF_WEEK_LENGTH 1

#define CHRONO_VALUE(x,y)  (ChronoValue) {.field = (x), .maxLength = (y)}
#define UNINITIALIZED_ZONE (TimeZone){0}
#define UNINITIALIZED_DATE (Date){0}
#define UNINITIALIZED_TIME (Time){.hours = -1, .minutes = -1, .seconds = 0, .millis = 0}
#define UNINITIALIZED_DATE_TIME (DateTime){.date = {0}, .time = {.hours = -1, .minutes = -1, .seconds = -1, .millis = -1}}
#define UNINITIALIZED_ZONED_DATE_TIME (ZonedDateTime){ .dateTime.date = {0}, .dateTime.time = {.hours = -1, .minutes = -1, .seconds = -1, .millis = -1}, .zone = {.id = NULL}}


typedef struct {
    ChronoField field;
    uint8_t maxLength;
} ChronoValue;

typedef enum AmPmOfTime {
  TIME_AM, TIME_PM, TIME_NOT_AM_PM
} AmPmOfTime;


static void initDateTimeFormatter(DateTimeFormatter *formatter);
static void setFormatterError(DateTimeFormatter *formatter, const char *errorMessage, uint32_t errorIndex);

static bool dateTimeParse(const char *text, TemporalField *formatEntities, Date *date, Time *time, TimeZone *zone);

static bool haveDateTimeConflicts(TemporalField *formatEntities);
static bool containsField(TemporalField *formatEntities, ChronoField field);
static ChronoValue resolvePatternFieldChar(char patternChar);

static uint8_t parseYear(TemporalField *temporal, Date *date, const char *text);
static uint8_t parseMonth(TemporalField *temporal, Date *date, const char *text);
static uint8_t parseDayInMonth(TemporalField *temporal, Date *date, const char *text);
static uint8_t parseDayInWeek(TemporalField *temporal, Date *date, const char *text);
static uint8_t parseAmPmOfDay(AmPmOfTime *amPm, const char *text);
static uint8_t parseHourOfDay(TemporalField *temporal, Time *time, const char *text);
static uint8_t parseHourOfAmPm(TemporalField *temporal, Time *time, const char *text);
static uint8_t parseMinutesOfHour(TemporalField *temporal, Time *time, const char *text);
static uint8_t parseSecondsOfMinute(TemporalField *temporal, Time *time, const char *text);
static uint8_t parseMilliseconds(TemporalField *temporal, Time *time, const char *text);
static uint8_t parseTimeZone(TimeZone *zone, const char *text);

static uint32_t parseZoneDigits(const char *text, uint32_t textLength, TimeZone *zone, int8_t sign);
static uint32_t parseZoneByName(const char *text, uint32_t textLength, TimeZone *zone);
static int64_t extractTemporalNumber(TemporalField *temporal, const char *text, uint8_t *parsedLength);
static void resolveAmPmIfPresent(Time *time, AmPmOfTime amPm);
static bool checkThatDateValid(Date *date, TemporalField *formatEntities);

static void dateTimeFormat(Date *date, Time *time, const TimeZone *zone, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter);

static uint32_t formatEra(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatYear(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatMonth(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatWeekInYear(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatWeekInMoth(Date *date, char *resultBuffer);
static uint32_t formatDayInYear(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatDayInMonth(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatDayOfWeekInMonth(Date *date, char *resultBuffer);
static uint32_t formatDayInWeek(TemporalField *temporal, Date *date, char *resultBuffer);
static uint32_t formatAmPmOfDay(Time *time, char *resultBuffer);
static uint32_t formatTimeValues(TemporalField *temporal, uint32_t timeValue, char *resultBuffer);
static uint32_t formatTextTimeZone(TemporalField *temporal, Date *date, Time *time,  const TimeZone *zone, char *resultBuffer);
static uint32_t formatDigitTimeZone(TemporalField *temporal, Date *date, Time *time, const TimeZone *zone, char *resultBuffer);

static inline bool isLongNumberValid(int64_t number, const char *valuePointer, const char *endPointer) {
    return ((valuePointer == endPointer) ||              // no digits found
            (errno == ERANGE && number == LLONG_MIN) ||   // underflow occurred
            (errno == ERANGE && number == LLONG_MAX)      // overflow occurred
            ? false : true);
}

static inline int8_t charToOffsetSign(char signChar) {
    if (signChar == '+') {
        return 1;
    } else if (signChar == '-') {
        return -1;
    }
    return 0;
}

TimeZone timeZoneOf(const char *zoneId) {
    TimeZone zone = UNINITIALIZED_ZONE;
    static TemporalField timeZoneFormat[2] = {CHRONO_FIELD_DIGIT_TIME_ZONE, CHRONO_END_OF_PATTERN};
    return dateTimeParse(zoneId, timeZoneFormat, NULL, NULL, &zone) ? zone : UNINITIALIZED_ZONE;
}

void parseDateTimePattern(DateTimeFormatter *formatter, const char *pattern) {
    initDateTimeFormatter(formatter);
    uint32_t patternLength = (pattern == NULL || formatter == NULL) ? 0 : strlen(pattern);
    uint32_t formatterIndex = 0;
    for (uint32_t i = 0; i < patternLength; i++) {
        char patternChar = pattern[i];

        if ((patternChar >= 'A' && patternChar <= 'Z') || (patternChar >= 'a' && patternChar <= 'z')) {
            uint32_t start = i++;
            for (; i < patternLength && pattern[i] == patternChar; i++);  // short loop, counting same pattern chars

            uint8_t count = i - start;
            ChronoValue value = resolvePatternFieldChar(patternChar);
            if (value.field == CHRONO_END_OF_PATTERN) {
                setFormatterError(formatter, FORMATTER_ERROR_UNKNOWN_CHAR, --i);
                return;
            }

            if (count > value.maxLength) {
                setFormatterError(formatter, FORMATTER_ERROR_PATTERN_LENGTH, --i);
                return;
            }

            if (formatterIndex < DATE_TIME_FORMAT_SIZE) {
                TemporalField *temporal = &formatter->formatEntities[formatterIndex];
                temporal->field = value.field;
                temporal->length = count;
                temporal->literal = patternChar;
                formatterIndex++;
            }
            i--;

        } else if (patternChar == '\'') {   // parse escaped literals
            for (i = (i + 1); i < patternLength; i++) {     // i + 1 -> skip '\''
                char literal = pattern[i];
                if (literal == '\'') {
                    if ((i + 1 < patternLength) && (pattern[i + 1] == '\'')) {
                        i++;    // skip closed quote
                    } else {
                        break;  // end of literal
                    }
                }

                if (formatterIndex < DATE_TIME_FORMAT_SIZE) {
                    TemporalField *temporal = &formatter->formatEntities[formatterIndex];
                    temporal->field = CHRONO_FIELD_LITERAL;
                    temporal->literal = literal;
                    temporal->length++;
                    formatterIndex++;
                }
            }

            if (i >= patternLength) {
                setFormatterError(formatter, FORMATTER_ERROR_INCOMPLETE_LITERAL, --i);
                return;
            }

        } else if (patternChar == '{' || patternChar == '}' || patternChar == '#') {
            setFormatterError(formatter, FORMATTER_ERROR_RESERVED_CHAR, --i);
            return;

        } else if (formatterIndex < DATE_TIME_FORMAT_SIZE) {   // add separators
            TemporalField *temporal = &formatter->formatEntities[formatterIndex];
            temporal->field = CHRONO_FIELD_LITERAL;
            temporal->literal = patternChar;
            temporal->length++;
            formatterIndex++;
        }

        if (formatterIndex >= DATE_TIME_FORMAT_SIZE) {
            setFormatterError(formatter, FORMATTER_ERROR_OVERFLOW, --i);
            return;
        }
    }
}

Time parseToTime(const char *text, DateTimeFormatter *formatter) {
    if (formatter != NULL && formatter->isFormatValid) {
        Time time = {0};
        if (dateTimeParse(text, formatter->formatEntities, NULL, &time, NULL)) {
            return time;
        }
    }
    return UNINITIALIZED_TIME;
}

Date parseToDate(const char *text, DateTimeFormatter *formatter) {
    if (formatter != NULL && formatter->isFormatValid) {
        Date date = {0};
        if (dateTimeParse(text, formatter->formatEntities, &date, NULL, NULL)) {
            return date;
        }
    }
    return UNINITIALIZED_DATE;
}

DateTime parseToDateTime(const char *text, DateTimeFormatter *formatter) {
    if (formatter != NULL && formatter->isFormatValid) {
        DateTime dateTime = {0};
        if (dateTimeParse(text, formatter->formatEntities, &dateTime.date, &dateTime.time, NULL)) {
            return dateTime;
        }
    }
    return UNINITIALIZED_DATE_TIME;
}

ZonedDateTime parseToZonedDateTime(const char *text, DateTimeFormatter *formatter) {
    if (formatter != NULL && formatter->isFormatValid) {
        ZonedDateTime zonedDateTime = {0};
        if (dateTimeParse(text, formatter->formatEntities, &zonedDateTime.dateTime.date, &zonedDateTime.dateTime.time, &zonedDateTime.zone)) {
            return zonedDateTime;
        }
    }
    return UNINITIALIZED_ZONED_DATE_TIME;
}

void formatTime(Time *time, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter) {
    dateTimeFormat(NULL, time, NULL, resultBuffer, length, formatter);
}

void formatDate(Date *date, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter) {
    dateTimeFormat(date, NULL, NULL, resultBuffer, length, formatter);
}

void formatDateTime(DateTime *dateTime, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter) {
    dateTimeFormat(&dateTime->date, &dateTime->time, NULL, resultBuffer, length, formatter);
}

void formatZonedDateTime(ZonedDateTime *zonedDateTime, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter) {
    dateTimeFormat(&zonedDateTime->dateTime.date, &zonedDateTime->dateTime.time, &zonedDateTime->zone, resultBuffer, length, formatter);
}

static bool dateTimeParse(const char *text, TemporalField *formatEntities, Date *date, Time *time, TimeZone *zone) {
    if (text == NULL || formatEntities == NULL || haveDateTimeConflicts(formatEntities)) return false;

    uint32_t i = 0;
    uint8_t parsedLength = 0;
    AmPmOfTime amPm = TIME_NOT_AM_PM;
    while (i < DATE_TIME_FORMAT_SIZE && *text != '\0') {
        TemporalField *temporal = &formatEntities[i];

        switch (temporal->field) {
            case CHRONO_FIELD_YEAR_OF_ERA:
            case CHRONO_FIELD_YEAR:
                parsedLength = parseYear(temporal, date, text);
                break;
            case CHRONO_FIELD_MONTH_IN_YEAR:
                parsedLength = parseMonth(temporal, date, text);
                break;
            case CHRONO_FIELD_DAY_IN_MONTH:
                parsedLength = parseDayInMonth(temporal, date, text);
                break;
            case CHRONO_FIELD_DAY_IN_WEEK:
                parsedLength = parseDayInWeek(temporal, date, text);
                break;
            case CHRONO_FIELD_AMPM_OF_DAY:
                parsedLength = parseAmPmOfDay(&amPm, text);
                break;
            case CHRONO_FIELD_HOUR_OF_DAY:
                parsedLength = parseHourOfDay(temporal, time, text);
                break;
            case CHRONO_FIELD_CLOCK_HOUR_OF_AMPM:
                parsedLength = parseHourOfAmPm(temporal, time, text);
                break;
            case CHRONO_FIELD_MINUTE_OF_HOUR:
                parsedLength = parseMinutesOfHour(temporal, time, text);
                break;
            case CHRONO_FIELD_SECOND_OF_MINUTE:
                parsedLength = parseSecondsOfMinute(temporal, time, text);
                break;
            case CHRONO_FIELD_MILLISECOND:
                parsedLength = parseMilliseconds(temporal, time, text);
                break;
            case CHRONO_FIELD_DIGIT_TIME_ZONE:
                parsedLength = parseTimeZone(zone, text);
                break;
            case CHRONO_FIELD_LITERAL:
                parsedLength = temporal->length;      // skip literal pattern
                break;
            default:
                return false;// unsupported pattern
        }

        if (parsedLength == TEXT_NOT_PARSED || temporal->field == CHRONO_END_OF_PATTERN) {
            break;
        }
        text += parsedLength;
        i++;
    }

    bool isFormatPatternEnd = (formatEntities[i].field == CHRONO_END_OF_PATTERN);
    bool isParsed = (parsedLength > 0) && (*text == '\0') && isFormatPatternEnd;
    if (isParsed) {
        if (time != NULL) { // check that parsed time is valid
            resolveAmPmIfPresent(time, amPm);
            isParsed = isTimeValid(time);
        }

        if (isParsed && date != NULL) { // check that parsed date is valid
            isParsed = checkThatDateValid(date, formatEntities);
        }
    }
    return isParsed;
}

static void dateTimeFormat(Date *date, Time *time, const TimeZone *zone, char *resultBuffer, uint32_t length, DateTimeFormatter *formatter) {
    if (resultBuffer == NULL || length == 0 || formatter == NULL || !formatter->isFormatValid) return;
    memset(resultBuffer, 0, length);

    uint32_t printedSize = 0;
    for (uint32_t i = 0; i < DATE_TIME_FORMAT_SIZE && length > printedSize; i++) {
        TemporalField *temporal = &formatter->formatEntities[i];
        uint32_t paddingLength;

        switch (temporal->field) {
            case CHRONO_FIELD_ERA:
                paddingLength = formatEra(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_YEAR_OF_ERA:
            case CHRONO_FIELD_YEAR:
                paddingLength = formatYear(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_MONTH_IN_YEAR:
                paddingLength = formatMonth(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_WEEK_IN_YEAR:
                paddingLength = formatWeekInYear(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_WEEK_IN_MONTH:
                paddingLength = formatWeekInMoth(date, resultBuffer);
                break;
            case CHRONO_FIELD_DAY_IN_YEAR:
                paddingLength = formatDayInYear(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_DAY_IN_MONTH:
                paddingLength = formatDayInMonth(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_DAY_OF_WEEK_IN_MONTH:
                paddingLength = formatDayOfWeekInMonth(date, resultBuffer);
                break;
            case CHRONO_FIELD_DAY_IN_WEEK:
                paddingLength = formatDayInWeek(temporal, date, resultBuffer);
                break;
            case CHRONO_FIELD_AMPM_OF_DAY:
                paddingLength = formatAmPmOfDay(time, resultBuffer);
                break;
            case CHRONO_FIELD_HOUR_OF_DAY:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, time->hours, resultBuffer) : 0;
                break;
            case CHRONO_FIELD_CLOCK_HOUR_OF_DAY:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, (time->hours == 0 ? 24 : time->hours), resultBuffer) : 0;
                break;
            case CHRONO_FIELD_HOUR_OF_AMPM:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, (time->hours % 12), resultBuffer) : 0;
                break;
            case CHRONO_FIELD_CLOCK_HOUR_OF_AMPM:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, ((time->hours == 0 || time->hours == 12) ? 12 : time->hours % 12), resultBuffer) : 0;
                break;
            case CHRONO_FIELD_MINUTE_OF_HOUR:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, time->minutes, resultBuffer) : 0;
                break;
            case CHRONO_FIELD_SECOND_OF_MINUTE:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, time->seconds, resultBuffer) : 0;
                break;
            case CHRONO_FIELD_MILLISECOND:
                paddingLength = isTimeValid(time) ? formatTimeValues(temporal, time->millis, resultBuffer) : 0;
                break;
            case CHRONO_FIELD_TEXT_TIME_ZONE:
                paddingLength = formatTextTimeZone(temporal, date, time, zone, resultBuffer);
                break;
            case CHRONO_FIELD_DIGIT_TIME_ZONE:
                paddingLength = formatDigitTimeZone(temporal, date, time, zone, resultBuffer);
                break;
            case CHRONO_FIELD_LITERAL:
                paddingLength = sprintf(resultBuffer, "%c", temporal->literal);
                break;
            default:    // end of pattern
                return;
        }
        printedSize += paddingLength;
        resultBuffer += paddingLength;
    }
}

static void initDateTimeFormatter(DateTimeFormatter *formatter) {
    if (formatter == NULL) return;
    for (int i = 0; i < DATE_TIME_FORMAT_SIZE; i++) {
        TemporalField *temporal = &formatter->formatEntities[i];
        temporal->field = CHRONO_END_OF_PATTERN;
        temporal->length = 0;
        temporal->literal = '\0';
    }
    formatter->isFormatValid = true;
    formatter->errorMessage = NULL;
    formatter->errorAtIndex = 0;
}

static void setFormatterError(DateTimeFormatter *formatter, const char *errorMessage, uint32_t errorIndex) {
    formatter->errorMessage = errorMessage;
    formatter->isFormatValid = false;
    formatter->errorAtIndex = errorIndex;
}

static bool haveDateTimeConflicts(TemporalField *formatEntities) {
    bool haveMissingAmPm = containsField(formatEntities, CHRONO_FIELD_CLOCK_HOUR_OF_AMPM) &&
                           !containsField(formatEntities, CHRONO_FIELD_AMPM_OF_DAY);
    bool amPmWithHourOfDay = containsField(formatEntities, CHRONO_FIELD_AMPM_OF_DAY) &&
                             containsField(formatEntities, CHRONO_FIELD_HOUR_OF_DAY);
    bool hourOfDayWithAmPmHours = containsField(formatEntities, CHRONO_FIELD_CLOCK_HOUR_OF_AMPM) &&
                                  containsField(formatEntities, CHRONO_FIELD_HOUR_OF_DAY);

    return haveMissingAmPm || amPmWithHourOfDay || hourOfDayWithAmPmHours;
}

static bool containsField(TemporalField *formatEntities, ChronoField field) {
    for (uint32_t i = 0; i < DATE_TIME_FORMAT_SIZE; i++) {
        TemporalField *temporal = &formatEntities[i];
        if (temporal->field == CHRONO_END_OF_PATTERN) {
            return false;
        } else if (temporal->field == field) {
            return true;
        }
    }
    return false;
}

static ChronoValue resolvePatternFieldChar(char patternChar) {
    switch (patternChar) {
        case 'G': return CHRONO_VALUE(CHRONO_FIELD_ERA, 4);
        case 'y': return CHRONO_VALUE(CHRONO_FIELD_YEAR_OF_ERA, 5);
        case 'u': return CHRONO_VALUE(CHRONO_FIELD_YEAR, 5);
        case 'M': return CHRONO_VALUE(CHRONO_FIELD_MONTH_IN_YEAR, 5);
        case 'w': return CHRONO_VALUE(CHRONO_FIELD_WEEK_IN_YEAR, 2);
        case 'W': return CHRONO_VALUE(CHRONO_FIELD_WEEK_IN_MONTH, 1);
        case 'D': return CHRONO_VALUE(CHRONO_FIELD_DAY_IN_YEAR, 3);
        case 'd': return CHRONO_VALUE(CHRONO_FIELD_DAY_IN_MONTH, 2);
        case 'F': return CHRONO_VALUE(CHRONO_FIELD_DAY_OF_WEEK_IN_MONTH, 1);
        case 'E': return CHRONO_VALUE(CHRONO_FIELD_DAY_IN_WEEK, 5);
        case 'a': return CHRONO_VALUE(CHRONO_FIELD_AMPM_OF_DAY, 1);
        case 'H': return CHRONO_VALUE(CHRONO_FIELD_HOUR_OF_DAY, 2);
        case 'k': return CHRONO_VALUE(CHRONO_FIELD_CLOCK_HOUR_OF_DAY, 2);
        case 'K': return CHRONO_VALUE(CHRONO_FIELD_HOUR_OF_AMPM, 2);
        case 'h': return CHRONO_VALUE(CHRONO_FIELD_CLOCK_HOUR_OF_AMPM, 2);
        case 'm': return CHRONO_VALUE(CHRONO_FIELD_MINUTE_OF_HOUR, 2);
        case 's': return CHRONO_VALUE(CHRONO_FIELD_SECOND_OF_MINUTE, 2);
        case 'S': return CHRONO_VALUE(CHRONO_FIELD_MILLISECOND, 3);
        case 'z': return CHRONO_VALUE(CHRONO_FIELD_TEXT_TIME_ZONE, 4);
        case 'Z': return CHRONO_VALUE(CHRONO_FIELD_DIGIT_TIME_ZONE, 5);
        default:  return CHRONO_VALUE(CHRONO_END_OF_PATTERN, 0);
    }
}

static uint8_t parseYear(TemporalField *temporal, Date *date, const char *text) {
    if (date == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t year = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(YEAR_RANGE, year)) {
        date->year = year;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseMonth(TemporalField *temporal, Date *date, const char *text) {
    if (date == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    if (temporal->length == PATTERN_LENGTH_ONE) {
        for (int i = 0; isdigit(text[i]); i++) {
            parsedLength++;
        }
    } else if (temporal->length == PATTERN_LENGTH_FOUR) {
        for (int i = 0; isalpha(text[i]); i++) {
            parsedLength++;
        }
    }

    if (parsedLength > MONTH_NAME_MAX_LENGTH) {
        return TEXT_NOT_PARSED;
    }

    char monthBuffer[16] = {0};
    if (temporal->length == PATTERN_LENGTH_ONE || temporal->length == PATTERN_LENGTH_TWO) {    // Parse numeric value
        strncpy(monthBuffer, text, parsedLength);
        Month month = extractTemporalNumber(temporal, text, &parsedLength);
        if (isValidValue(MONTH_OF_YEAR_RANGE, month)) {
            date->month = month;
            return parsedLength;
        }
        return TEXT_NOT_PARSED;
    }

    if (temporal->length == PATTERN_LENGTH_THREE) {    // Parse by month short name
        strncpy(monthBuffer, text, temporal->length);
        for (uint32_t i = 0; i < ARRAY_SIZE(MONTH_NAME_SHORT); i++) {
            if (strcasecmp(monthBuffer, MONTH_NAME_SHORT[i]) == 0) {
                date->month = i + 1;
                return temporal->length;
            }
        }
        return TEXT_NOT_PARSED;

    } else {                            // Parse by month full name
        strncpy(monthBuffer, text, parsedLength);
        for (uint32_t i = 0; i < ARRAY_SIZE(MONTH_NAME_SHORT); i++) {
            if (strcasecmp(monthBuffer, MONTH_NAME_LONG[i]) == 0) {
                date->month = i + 1;
                return parsedLength;
            }
        }
        return TEXT_NOT_PARSED;
    }
}

static uint8_t parseDayInMonth(TemporalField *temporal, Date *date, const char *text) {
    if (date == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t day = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(DAY_OF_MONTH_RANGE, day)) {
        date->day = (int8_t) day;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseDayInWeek(TemporalField *temporal, Date *date, const char *text) {
    if (date == NULL) return TEXT_NOT_PARSED;
    char weekDayBuffer[WIDE_DAY_OF_WEEK_MAX_LENGTH + 1] = {0};
    if (temporal->length >= PATTERN_LENGTH_ONE && temporal->length <= PATTERN_LENGTH_THREE) {   // parse short week name like: "Tue"
        strncpy(weekDayBuffer, text, ABBREVIATED_DAY_OF_WEEK_LENGTH);
        for (uint32_t i = 0; i < ARRAY_SIZE(WEEK_DAY_NAME_SHORT); i++) {
            if (strcasecmp(weekDayBuffer, WEEK_DAY_NAME_SHORT[i]) == 0) {
                date->weekDay = (DayOfWeek) i + 1;
                return ABBREVIATED_DAY_OF_WEEK_LENGTH;
            }
        }

    } else if (temporal->length == PATTERN_LENGTH_FOUR) {   // parse long week name like: "Tuesday"
        for (uint8_t i = 0; i < WIDE_DAY_OF_WEEK_MAX_LENGTH; i++) {
            char dayOfWeekChar = text[i];
            if (!isalpha(dayOfWeekChar)) break;
            weekDayBuffer[i] = dayOfWeekChar;
        }
        for (uint32_t i = 0; i < ARRAY_SIZE(WEEK_DAY_NAME_LONG); i++) {
            if (strcasecmp(weekDayBuffer, WEEK_DAY_NAME_LONG[i]) == 0) {
                date->weekDay = (DayOfWeek) i + 1;
                return strlen(weekDayBuffer);
            }
        }

    } else if (temporal->length == PATTERN_LENGTH_FIVE) {   // parse by first char like: "T"
        char dayOfWeekChar = (char) toupper(*text);
        for (uint32_t i = 0; i < ARRAY_SIZE(WEEK_DAY_NAME_SHORT); i++) {
            if (dayOfWeekChar == WEEK_DAY_NAME_SHORT[i][0]) {
                date->weekDay = (DayOfWeek) i + 1;
                return NARROW_DAY_OF_WEEK_LENGTH;
            }
        }
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseAmPmOfDay(AmPmOfTime *amPm, const char *text) {
    uint32_t textLength = strlen(text);
    if (textLength >= AM_PM_LENGTH) {
        if (strncasecmp(text, "AM", AM_PM_LENGTH) == 0) {
            *amPm = TIME_AM;
        } else if (strncasecmp(text, "PM", AM_PM_LENGTH) == 0) {
            *amPm = TIME_PM;
        } else {
            return TEXT_NOT_PARSED;
        }
        return AM_PM_LENGTH;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseHourOfDay(TemporalField *temporal, Time *time, const char *text) {
    if (time == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t hours = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(HOUR_OF_DAY_RANGE, hours)) {
        time->hours = (int8_t) hours;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseHourOfAmPm(TemporalField *temporal, Time *time, const char *text) {
    if (time == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t hours = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(HOUR_OF_AM_PM, hours)) {
        time->hours = (int8_t) hours;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseMinutesOfHour(TemporalField *temporal, Time *time, const char *text) {
    if (time == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t minutes = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(MINUTE_OF_HOUR_RANGE, minutes)) {
        time->minutes = (int8_t) minutes;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseSecondsOfMinute(TemporalField *temporal, Time *time, const char *text) {
    if (time == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t seconds = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(SECOND_OF_MINUTE_RANGE, seconds)) {
        time->seconds = (int8_t) seconds;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseMilliseconds(TemporalField *temporal, Time *time, const char *text) {
    if (time == NULL) return TEXT_NOT_PARSED;
    uint8_t parsedLength = 0;
    int64_t millis = extractTemporalNumber(temporal, text, &parsedLength);
    if (isValidValue(MILLIS_OF_SECOND_RANGE, millis)) {
        time->millis = (int16_t) millis;
        return parsedLength;
    }
    return TEXT_NOT_PARSED;
}

static uint8_t parseTimeZone(TimeZone *zone, const char *text) {
    if (zone == NULL) return TEXT_NOT_PARSED;

    if (*text == ISO8601_UTC_INDICATOR) {
        zone->id = UTC.id;
        zone->utcOffset = UTC.utcOffset;
        zone->names = UTC.names;
        zone->rules = UTC.rules;
        return 1;
    }

    uint32_t textLength = strlen(text);
    const char *textStart = text;
    int8_t sign = charToOffsetSign(*text);

    if (sign == 0) {    // Try parsing a custom time zone "GMT+hh:mm" or "GMT".
        bool haveGMTPrefix = strncasecmp(text, GMT.id, PREFIX_LENGTH) == 0;
        bool haveUTCPrefix = strncasecmp(text, UTC.id, PREFIX_LENGTH) == 0;
        if (haveGMTPrefix || haveUTCPrefix) {
            text += PREFIX_LENGTH;

            bool haveOffsetChars = textLength - (text - textStart) > 0;
            if (haveOffsetChars) {
                sign = charToOffsetSign(*text);
            }

            bool isPrefixWithoutOffset = (sign == 0);
            if (isPrefixWithoutOffset) {
                *zone = haveGMTPrefix ? GMT : UTC;
                return (text - textStart);
            }

            uint32_t parsedLength = parseZoneDigits(text, (textLength - PREFIX_LENGTH), zone, sign);
            if (parsedLength > 0) { // check that zone offset parsed
                if (haveGMTPrefix) {   // set "GMT", because prefix was specified
                    zone->id = GMT.id;
                    zone->names = GMT.names;
                }
                return parsedLength + PREFIX_LENGTH;
            }
            return parsedLength;

        } else {    // Try parsing the text as a time zone name or abbreviation.
            return parseZoneByName(text, textLength, zone);
        }
    }
    // Parse the rest as "hh:mm" or "hhmm" (RFC 822), also check optional seconds
    return parseZoneDigits(text, textLength, zone, sign);
}

static uint32_t parseZoneDigits(const char *text, uint32_t textLength, TimeZone *zone, int8_t sign) {
    const char *textStart = text;
    char timeBuffer[ZONE_DOUBLE_DIGIT_COUNT + 1] = {0};
    text++; // skip sign
    textLength--;

    errno = 0;
    char *endPointer = NULL;
    strncpy(timeBuffer, text, ZONE_DOUBLE_DIGIT_COUNT);
    int64_t hours = strtoll(timeBuffer, &endPointer, 10);
    if (!isLongNumberValid(hours, timeBuffer, endPointer)) {
        return TEXT_NOT_PARSED;
    }

    int32_t offset = (int32_t) (hours * SECONDS_PER_HOUR);
    text = (textLength >= ZONE_DOUBLE_DIGIT_COUNT) ? text + ZONE_DOUBLE_DIGIT_COUNT : text + ZONE_SINGLE_DIGIT_COUNT;
    if (*text == '\0') {    // only hours without minutes
        zone->id = UTC.id;
        zone->utcOffset = offset * sign;
        zone->names = UTC.names;
        zone->rules = UTC.rules;
        return (text - textStart);
    }

    if (*text == ':') {
        text++;
    }

    errno = 0;
    strncpy(timeBuffer, text, ZONE_DOUBLE_DIGIT_COUNT);
    int64_t minutes = strtoll(timeBuffer, &endPointer, 10);
    if (!isLongNumberValid(minutes, timeBuffer, endPointer)) {
        return TEXT_NOT_PARSED;
    }
    offset += (int32_t) (minutes * SECONDS_PER_MINUTE);
    text += ZONE_DOUBLE_DIGIT_COUNT;

    if (textLength >= ZONE_DIGIT_COUNT_WITH_SECONDS_SEPARATOR) {
        if (*text == ':') {
            text++;
        }

        errno = 0;
        strncpy(timeBuffer, text, ZONE_DOUBLE_DIGIT_COUNT);
        int64_t seconds = strtoll(timeBuffer, &endPointer, 10);
        if (isLongNumberValid(seconds, timeBuffer, endPointer)) {
            offset += (uint8_t) seconds;
            text += ZONE_DOUBLE_DIGIT_COUNT;
        }
    }

    zone->id = UTC.id;
    zone->utcOffset = offset * sign;
    zone->names = UTC.names;
    zone->rules = UTC.rules;
    return (text - textStart);
}

static uint32_t parseZoneByName(const char *text, uint32_t textLength, TimeZone *zone) {
    const char *textStart = text;
    static char zoneNameBuffer[ZONE_NAME_MAX_LENGTH + 2];
    memset(zoneNameBuffer, 0, ZONE_NAME_MAX_LENGTH + 2);
    for (uint8_t i = 0; i < ZONE_NAME_MAX_LENGTH && i < textLength; i++) {
        char zoneNameChar = *text;
        bool isValidZoneChar = isalnum(zoneNameChar) || zoneNameChar == '/' || zoneNameChar == '-' || zoneNameChar == '_';
        if (!isValidZoneChar) {
            break;
        }
        zoneNameBuffer[i] = zoneNameChar;
        text++;
    }

    const TimeZone *zoneId = findTimeZone(zoneNameBuffer);
    if (zoneId != NULL) {
        zone->id = zoneId->id;
        zone->utcOffset = zoneId->utcOffset;
        zone->names = zoneId->names;
        zone->rules = zoneId->rules;
        return (text - textStart);
    }
    return TEXT_NOT_PARSED;
}

static int64_t extractTemporalNumber(TemporalField *temporal, const char *text, uint8_t *parsedLength) {
    static char buffer[16];
    memset(buffer, 0, 16);

    int32_t maxLength = resolvePatternFieldChar(temporal->literal).maxLength;
    uint8_t length = (temporal->length == PATTERN_LENGTH_ONE) ? maxLength : temporal->length;
    for (uint8_t i = 0; i < length && *text != '\0'; i++) {
        if (!isdigit(*text)) {
            break;
        }
        buffer[i] = *text;
        text++;
    }

    errno = 0;
    char *endPointer = NULL;
    int64_t number = strtoll(buffer, &endPointer, 10);
    uint32_t resultLength = strlen(buffer);
    *parsedLength = resultLength;
    bool isLengthValid = (temporal->length == PATTERN_LENGTH_ONE) || (resultLength == length);   // if length is 1 then length is not strictly determined
    return (isLengthValid && isLongNumberValid(number, buffer, endPointer)) ? number : INVALID_NUMBER_VALUE;
}

static void resolveAmPmIfPresent(Time *time, AmPmOfTime amPm) {
    if (amPm == TIME_PM && time->hours != 0 && time->hours != 12) {
        time->hours += 12;
    } else if (amPm == TIME_AM && time->hours == 12) {
        time->hours = 0;
    }
}

static bool checkThatDateValid(Date *date, TemporalField *formatEntities) {
    if(!isDateValid(date)) return false;
    bool haveDayOfWeekField = containsField(formatEntities, CHRONO_FIELD_DAY_IN_WEEK);
    DayOfWeek calculatedDayOfWeek = getDayOfWeek(date);
    if (haveDayOfWeekField) {   // check that parsed week date is correct
        if (calculatedDayOfWeek == 0 || calculatedDayOfWeek != date->weekDay) {
            return false;
        }
    }
    date->weekDay = calculatedDayOfWeek;
    return true;
}

static uint32_t formatEra(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    return sprintf(resultBuffer, "%s", temporal->length == PATTERN_LENGTH_FOUR ? getIsoEraLong(date->year) : getIsoEraShort(date->year));
}

static uint32_t formatYear(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    if (temporal->length == PATTERN_LENGTH_ONE) {
        return sprintf(resultBuffer, "%lld", date->year);
    }
    int64_t year = date->year;
    uint8_t yearDigitCount = 0;
    while (year != 0) {
        year /= 10;
        yearDigitCount++;
    }

    uint8_t firstDigit = date->year % 10;
    uint8_t secondDigit = date->year % 100 / 10;
    if (temporal->length == PATTERN_LENGTH_TWO) {
        return sprintf(resultBuffer, "%d%d", secondDigit, firstDigit);
    }

    uint8_t thirdDigit = date->year % 1000 / 100;
    if (temporal->length == PATTERN_LENGTH_THREE) {
        return yearDigitCount >= PATTERN_LENGTH_THREE ? sprintf(resultBuffer, "%lld", date->year) : sprintf(resultBuffer, "%d%d%d", thirdDigit, secondDigit, firstDigit);
    }

    uint8_t fourthDigit = date->year % 10000 / 1000;
    if (temporal->length == PATTERN_LENGTH_FOUR) {
        return yearDigitCount >= PATTERN_LENGTH_FOUR ? sprintf(resultBuffer, "%lld", date->year) : sprintf(resultBuffer, "%d%d%d%d", fourthDigit, thirdDigit, secondDigit, firstDigit);
    }

    uint8_t fifthDigit = date->year % 100000 / 10000;
    return yearDigitCount >= PATTERN_LENGTH_FIVE ? sprintf(resultBuffer, "%lld", date->year) : sprintf(resultBuffer, "%d%d%d%d%d", fifthDigit, fourthDigit, thirdDigit, secondDigit, firstDigit);
}

static uint32_t formatMonth(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    Month month = date->month;
    switch (temporal->length) {
        case PATTERN_LENGTH_ONE:
            return sprintf(resultBuffer, "%d", month);   // The numeric month of the year. A single M will use '1' for January.
        case PATTERN_LENGTH_TWO:
            return sprintf(resultBuffer, "%02d", month); // The numeric month of the year. A double M will use '01' for January.
        case PATTERN_LENGTH_THREE:
            return sprintf(resultBuffer, "%s", getMonthNameShort(month));           // The shorthand name of the month
        case PATTERN_LENGTH_FOUR:
            return sprintf(resultBuffer, "%s", getMonthNameLong(month));        // Full name of the month
        case PATTERN_LENGTH_FIVE:
            return sprintf(resultBuffer, "%c", getMonthNameShort(month)[0]);    // Narrow name of the month, first char
        default:
            return 0;
    }
}

static uint32_t formatWeekInYear(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    uint8_t weekOfYear = getWeekOfYear(date);
    return (temporal->length == PATTERN_LENGTH_ONE) ? sprintf(resultBuffer, "%d", weekOfYear) : sprintf(resultBuffer, "%02d", weekOfYear);
}

static uint32_t formatWeekInMoth(Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    uint8_t weekOfMonth = getWeekOfMonth(date);
    return sprintf(resultBuffer, "%d", weekOfMonth);
}

static uint32_t formatDayInYear(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    uint32_t dayOfYear = getDayOfYear(date);
    if (temporal->length == PATTERN_LENGTH_ONE) {
        return sprintf(resultBuffer, "%d", dayOfYear);
    } else if (temporal->length == PATTERN_LENGTH_TWO) {
        return sprintf(resultBuffer, "%02d", dayOfYear);
    } else {
        return sprintf(resultBuffer, "%03d", dayOfYear);
    }
}

static uint32_t formatDayInMonth(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    return temporal->length == PATTERN_LENGTH_ONE ? sprintf(resultBuffer, "%d", date->day) : sprintf(resultBuffer, "%02d", date->day);
}

static uint32_t formatDayOfWeekInMonth(Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    DayOfWeek dayOfWeek = getDayOfWeek(date);
    return sprintf(resultBuffer, "%d", dayOfWeek);
}

static uint32_t formatDayInWeek(TemporalField *temporal, Date *date, char *resultBuffer) {
    if (!isDateValid(date)) return 0;
    DayOfWeek dayOfWeek = date->weekDay;
    if (temporal->length <= PATTERN_LENGTH_THREE) {
        return sprintf(resultBuffer, "%s", getWeekDayNameShort(dayOfWeek));
    } else if (temporal->length == PATTERN_LENGTH_FOUR) {
        return sprintf(resultBuffer, "%s", getWeekDayNameLong(dayOfWeek));
    } else if (temporal->length == PATTERN_LENGTH_FIVE) {
        return sprintf(resultBuffer, "%c", getWeekDayNameShort(dayOfWeek)[0]);
    }
    return 0;
}

static uint32_t formatAmPmOfDay(Time *time, char *resultBuffer) {
    if (!isTimeValid(time)) return 0;
    return sprintf(resultBuffer, "%s", time->hours >= 12 ? "PM" : "AM");
}

static uint32_t formatTimeValues(TemporalField *temporal, uint32_t timeValue, char *resultBuffer) {
    if (temporal->length == PATTERN_LENGTH_ONE) {
        return sprintf(resultBuffer, "%d", timeValue);
    } else if (temporal->length == PATTERN_LENGTH_TWO) {
        return sprintf(resultBuffer, "%02d", timeValue);
    } else {
        return sprintf(resultBuffer, "%03d", timeValue);
    }
}

static uint32_t formatTextTimeZone(TemporalField *temporal, Date *date, Time *time,  const TimeZone *zone, char *resultBuffer) {
    if (zone == NULL || !isDateValid(date) || !isTimeValid(time)) return 0;
    DateTime datTime = dateTimeFrom(date, time);
    int64_t epochSeconds = dateTimeToEpochSecond(&datTime, 0);
    ZoneTextStyle textStyle = (temporal->length < PATTERN_LENGTH_FOUR) ? ZONE_NAME_SHORT : ZONE_NAME_LONG;
    return sprintf(resultBuffer, "%s", resolveTimeZoneName(epochSeconds, zone, textStyle));
}

static uint32_t formatDigitTimeZone(TemporalField *temporal, Date *date, Time *time, const TimeZone *zone, char *resultBuffer) {
    if (zone == NULL || !isDateValid(date) || !isTimeValid(time)) return 0;
    DateTime datTime = dateTimeFrom(date, time);
    int64_t epochSeconds = dateTimeToEpochSecond(&datTime, 0);
    int32_t zoneUtcOffset = resolveTimeZoneOffset(epochSeconds, zone);

    int32_t hours = zoneUtcOffset / SECONDS_PER_HOUR;
    int32_t minutes = (zoneUtcOffset / SECONDS_PER_MINUTE) % MINUTES_PER_HOUR;

    if (temporal->length < PATTERN_LENGTH_FOUR) {
        return sprintf(resultBuffer, "%+03d%02d", hours, minutes);
    } else if (temporal->length == PATTERN_LENGTH_FOUR) {
        return sprintf(resultBuffer, "GMT%+03d:%02d", hours, minutes);
    }
    int32_t seconds = (abs(zoneUtcOffset) - (abs(hours) * SECONDS_PER_HOUR)) - (minutes * SECONDS_PER_MINUTE);
    if (seconds > 0) {
        return sprintf(resultBuffer, "%+03d:%02d:%02d", hours, minutes, seconds);
    }
    return sprintf(resultBuffer, "%+03d:%02d", hours, minutes);
}