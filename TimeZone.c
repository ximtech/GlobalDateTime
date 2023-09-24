#include "TimeZone.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ZONE_NOT_FOUND  (-1)


static int32_t findZoneIndexById(const char *id);


const TimeZone *findTimeZone(const char *zoneId) {
    if (zoneId == NULL) return NULL;
    int32_t zoneIndex = findZoneIndexById(zoneId);
    return zoneIndex == ZONE_NOT_FOUND ? NULL : &ZONE_LIST[zoneIndex];
}

int32_t resolveTimeZoneOffset(int64_t epochSeconds, const TimeZone *zone) {
    ZoneTransition zoneTransition = findZoneRuleTransition(epochSeconds, zone);
    if (zoneTransition.after != NULL && zoneTransition.before != NULL) {
        if (isTransitionGap(&zoneTransition, epochSeconds)) {
            return zoneTransition.after->gmtOffset;
        }
        return zoneTransition.before->gmtOffset;
    }
    return zone->utcOffset;
}

const char *resolveTimeZoneName(int64_t epochSeconds, const TimeZone *zone, ZoneTextStyle textStyle) {
    ZoneTransition zoneTransition = findZoneRuleTransition(epochSeconds, zone);
    if (zoneTransition.before != NULL) {
        if (zoneTransition.before->isDaylightTime) {
            return (textStyle == ZONE_NAME_LONG) ? zone->names[ZONE_LONG_NAME_DTS_INDEX] : zone->names[ZONE_SHORT_NAME_DTS_INDEX];
        }
    }
    return (textStyle == ZONE_NAME_LONG) ? zone->names[ZONE_LONG_NAME_STD_INDEX] : zone->names[ZONE_SHORT_NAME_STD_INDEX];
}

ZoneTransition findZoneRuleTransition(int64_t epochSeconds, const TimeZone *zone) {
    ZoneTransition zoneTransition = {.after = NULL, .before = NULL};
    if (zone->rules != NULL) {
        uint32_t i = 0;
        const TimeZoneRule *rule = NULL;
        while ((rule = &zone->rules[i])->transition != 0) {
            if (rule->transition >= epochSeconds && i > 0) {
                zoneTransition.before = &zone->rules[i - 1];
                zoneTransition.after = &zone->rules[i];
                break;
            }
            i++;
        }
    }
    return zoneTransition;
}

bool isTransitionGap(ZoneTransition *transition, int64_t epochSeconds) {
    uint32_t secondsBefore = transition->before->transition;
    uint32_t offsetAfter = transition->after->gmtOffset;
    return (epochSeconds >= secondsBefore) && (epochSeconds < (secondsBefore + offsetAfter));
}

bool isTimeZoneEquals(const TimeZone *timeZone, const TimeZone *other) {
    return strcmp(timeZone->id, other->id) == 0 && timeZone->utcOffset == other->utcOffset;
}

static int32_t findZoneIndexById(const char *id) {  // binary search
    #ifdef ENABLE_TIME_ZONE_SUPPORT
    int32_t leftIndex = 0;
    int32_t rightIndex = ARRAY_SIZE(ZONE_LIST) - 1;
    while (leftIndex <= rightIndex) {
        int32_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;

        int32_t res = -1000;   // some random value assigned because if res is already 0 then it will always return 0
        if (strcmp(id, ZONE_LIST[middleIndex].id) == 0)
            res = 0;

        if (res == 0) { // Check if id is present at mid
            return middleIndex;
        }

        if (strcmp(id, ZONE_LIST[middleIndex].id) > 0) {    // If id greater, ignore leftIndex half
            leftIndex = middleIndex + 1;
        } else {
            rightIndex = middleIndex - 1;  // If x is smaller, ignore rightIndex half
        }
    }
    #endif
    return ZONE_NOT_FOUND;
}