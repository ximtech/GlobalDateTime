#include "include/ZonedDateTime.h"

#define UTC_OFFSET 0

static const ZonedDateTime UNINITIALIZED_ZONED_DATE_TIME = {
        .dateTime.date = {.year = 0, .month = 0, .day = 0},
        .dateTime.time = {.hours = -1, .minutes = -1, .seconds = -1, .millis = -1},
        .zone = {.id = NULL}};

static void setLocalZoneDateTime(ZonedDateTime *zonedDateTime);


ZonedDateTime zonedDateTimeOf(int64_t year, Month month, uint8_t dayOfMonth, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t millis, const TimeZone *zone) {
    DateTime dateTime = dateTimeOfWithMillis(year, month, dayOfMonth, hours, minutes, seconds, millis);
    ZonedDateTime zonedDateTime = {.dateTime = dateTime, .zone = *zone};
    setLocalZoneDateTime(&zonedDateTime);
    return zonedDateTime;
}

ZonedDateTime zonedDateTimeFrom(Date *date, Time *time, const TimeZone *zone) {
    ZonedDateTime zonedDateTime = UNINITIALIZED_ZONED_DATE_TIME;
    if (isDateValid(date) && isTimeValid(time) && zone != NULL && zone->id != NULL) {
        DateTime dateTime = {.date = *date, .time = *time};
        zonedDateTime.dateTime = dateTime;
        zonedDateTime.zone = *zone;
        setLocalZoneDateTime(&zonedDateTime);
    }
    return zonedDateTime;
}

ZonedDateTime zonedDateTimeOfDateTime(DateTime *dateTime, const TimeZone *zone) {
    ZonedDateTime zonedDateTime = UNINITIALIZED_ZONED_DATE_TIME;
    if (isDateTimeValid(dateTime) && zone != NULL && zone->id != NULL) {
        zonedDateTime.dateTime = *dateTime;
        zonedDateTime.zone = *zone;
        setLocalZoneDateTime(&zonedDateTime);
    }
    return zonedDateTime;
}

ZonedDateTime zonedDateTimeOfInstant(Instant *instant, const TimeZone *zone) {
    ZonedDateTime zonedDateTime = UNINITIALIZED_ZONED_DATE_TIME;
    if (instant != NULL && zone != NULL && zone->id != NULL) {
        DateTime dateTime = dateTimeOfEpochSeconds(instant->seconds, instant->micros, UTC_OFFSET);
        zonedDateTime.dateTime = dateTime;
        zonedDateTime.zone = *zone;
        setLocalZoneDateTime(&zonedDateTime);
    }
    return zonedDateTime;
}

ZonedDateTime zonedDateTimeWithSameInstant(ZonedDateTime *zonedDateTime, const TimeZone *zone) {
    if (zonedDateTime != NULL && isDateTimeValid(&zonedDateTime->dateTime) && zone != NULL && zone->id != NULL) {
        int64_t epochSeconds = dateTimeToEpochSecond(&zonedDateTime->dateTime, zonedDateTime->offset);
        int32_t newZoneOffset = resolveTimeZoneOffset(epochSeconds, zone);
        int64_t micros = (zonedDateTime->dateTime.time.millis * MICROS_PER_MILLIS);
        DateTime dateTimeNew = dateTimeOfEpochSeconds(epochSeconds, micros, newZoneOffset);
        return (ZonedDateTime){.dateTime = dateTimeNew, .offset = newZoneOffset, .zone = *zone};
    }
    return UNINITIALIZED_ZONED_DATE_TIME;
}

void zonedDateTimePlusYears(ZonedDateTime *zonedDateTime, int64_t years) {
    dateTimePlusYears(&zonedDateTime->dateTime, years);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusMonths(ZonedDateTime *zonedDateTime, int64_t months) {
    dateTimePlusMonths(&zonedDateTime->dateTime, months);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusWeeks(ZonedDateTime *zonedDateTime, int64_t weeks) {
    dateTimePlusWeeks(&zonedDateTime->dateTime, weeks);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusDays(ZonedDateTime *zonedDateTime, int64_t days) {
    dateTimePlusDays(&zonedDateTime->dateTime, days);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusHours(ZonedDateTime *zonedDateTime, int64_t hours) {
    dateTimePlusHours(&zonedDateTime->dateTime, hours);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusMinutes(ZonedDateTime *zonedDateTime, int64_t minutes) {
    dateTimePlusMinutes(&zonedDateTime->dateTime, minutes);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusSeconds(ZonedDateTime *zonedDateTime, int64_t seconds) {
    dateTimePlusSeconds(&zonedDateTime->dateTime, seconds);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimePlusMillis(ZonedDateTime *zonedDateTime, int64_t millis) {
    dateTimePlusMillis(&zonedDateTime->dateTime, millis);
    return setLocalZoneDateTime(zonedDateTime);
}

void zonedDateTimeMinusYears(ZonedDateTime *zonedDateTime, int64_t years) {
    zonedDateTimePlusYears(zonedDateTime, -years);
}

void zonedDateTimeMinusMonths(ZonedDateTime *zonedDateTime, int64_t months) {
    zonedDateTimePlusMonths(zonedDateTime, -months);
}

void zonedDateTimeMinusWeeks(ZonedDateTime *zonedDateTime, int64_t weeks) {
    zonedDateTimePlusWeeks(zonedDateTime, -weeks);
}

void zonedDateTimeMinusDays(ZonedDateTime *zonedDateTime, int64_t days) {
    zonedDateTimePlusDays(zonedDateTime, -days);
}

void zonedDateTimeMinusHours(ZonedDateTime *zonedDateTime, int64_t hours) {
    zonedDateTimePlusHours(zonedDateTime, -hours);
}

void zonedDateTimeMinusMinutes(ZonedDateTime *zonedDateTime, int64_t minutes) {
    zonedDateTimePlusMinutes(zonedDateTime, -minutes);
}

void zonedDateTimeMinusSeconds(ZonedDateTime *zonedDateTime, int64_t seconds) {
    zonedDateTimePlusSeconds(zonedDateTime, -seconds);
}

void zonedDateTimeMinusMillis(ZonedDateTime *zonedDateTime, int64_t millis) {
    zonedDateTimePlusMillis(zonedDateTime, -millis);
}

bool isZonedDateTimeEquals(ZonedDateTime *zonedDateTime, ZonedDateTime *other) {
    if (zonedDateTime == NULL || other == NULL) return false;
    return isDateTimeEquals(&zonedDateTime->dateTime, &other->dateTime) &&
           zonedDateTime->offset == other->offset &&
           isTimeZoneEquals(&zonedDateTime->zone, &other->zone);
}

static void setLocalZoneDateTime(ZonedDateTime *zonedDateTime) {
    int64_t epochSeconds = dateTimeToEpochSecond(&zonedDateTime->dateTime, UTC_OFFSET);

    int32_t storedOffset = zonedDateTime->zone.utcOffset;
    ZoneTransition zoneTransition = findZoneRuleTransition(epochSeconds, &zonedDateTime->zone);
    if (zoneTransition.after != NULL && zoneTransition.before != NULL) {
        storedOffset = isTransitionGap(&zoneTransition, epochSeconds) ? zoneTransition.after->gmtOffset : zoneTransition.before->gmtOffset;

        uint32_t gapStartDate = zoneTransition.before->transition + zoneTransition.after->gmtOffset;
        uint32_t gapEndDate = gapStartDate + zoneTransition.after->gmtOffset;
        bool isLocalDateInTimeGap = (epochSeconds >= gapStartDate) && (epochSeconds < gapEndDate);
        if (isLocalDateInTimeGap) {
            dateTimePlusSeconds(&zonedDateTime->dateTime, zoneTransition.after->gmtOffset);
        }
    }
    zonedDateTime->offset = storedOffset;
}