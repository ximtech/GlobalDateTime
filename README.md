# GlobalDateTime

[![tests](https://github.com/ximtech/GlobalDateTime/actions/workflows/cmake-ci.yml/badge.svg?branch=main)](https://github.com/ximtech/GlobalDateTime/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/GlobalDateTime/branch/main/graph/badge.svg?token=XjuO5pSKgq)](https://codecov.io/gh/ximtech/GlobalDateTime)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/de05a93d0982417792aef4534da05dd7)](https://www.codacy.com/gh/ximtech/GlobalDateTime/dashboard)


Java package `java.time.*` partition to low-level C library. Fully refactored and memory optimization allow use in small
embedded applications, designed for SMT32 microcontrollers. Written on ANSI C and no external dependencies has been
used

### Package includes

- `Instant.h` - Single instantaneous point on the time-line. This might be used to record event time-stamps in the
  application. The epoch-seconds are measured from the standard of 1970-01-01 00:00:00 where instants after the epoch
  have positive values, and earlier instants have negative values.
- `LocalTime.h` - A time without a time-zone in the ISO-8601 calendar system, such as 10:15:30. Represents a time, often
  viewed as hour-minute-second. Time is represented to milliseconds precision.
- `LocalDate.h` - A date without a time-zone in the ISO-8601 calendar system, such as 2007-12-03. Represents a date, often
  viewed as year-month-day. Other date data, such as day-of-year, day-of-week and week-of-year, can also be calculated
- `LocalDateTime.h` - A date-time without a time-zone in the ISO-8601 calendar system, such as 2007-12-03T10:15:30.
  Represents a date-time, often viewed as year-month-day-hour-minute-second. Other date and time data, such as
  day-of-year, day-of-week and week-of-year, can also be calculated
- `ZonedDateTime.h` - A date-time with a time-zone in the ISO-8601 calendar system, such as 2007-12-03T10:15:30+01:00
  Europe/Paris. This structure stores all date and time fields, to a precision of milliseconds, and a time-zone, with a
  zone offset used to handle ambiguous local date-times.
- `TimeZone.h` - Contains a time-zone offset from Greenwich/UTC, such as +02:00, or zone name Europe/Paris. Also
  contains historic rules for offset calculation
- `TimeZoneNames.h` - Contains all time zone naming conventions, short and full name.
- `TimeZoneRules.h` - The rules defining how the zone offset varies for a single time-zone.
- `ValueRange.h` - Date-time range values, used for data validation.
- `DateTimeMath.h` - Contains date-time calculation functions.
- `GlobalDateTime.h` - All includes in one and also formatter for printing and parsing strings to date-time structs

### Features

- User friendly API
- No dynamic or static memory allocations
- All time zone support
- Rich functionality for creating time zones
- Daylight savings with historic rules support
- Epoch date/time functions
- String to date-time and vice versa
- Flexible string parsing to date-time
- No `time.h` library dependencies
- Full test coverage

### Trade-offs

- Historic rules contains giant data capacity and can allocate up to 12Mb of flash memory. Such rules is used for
  daylight saving time calculation and can be disabled/reduced for memory saving. Also time zones can occupy large
  amount of memory, so them also can be disabled if not needed. By default, time zones and rules disabled and also rules
  has been reduced from 50 years to past and 50 years to future. So zone rules file size is not larger than 2.60 MB and
  can be stored in external memory.
- No locale support at now. So some date-time alignments hasn't implemented.

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME GlobalDateTime
        GITHUB_REPOSITORY ximtech/GlobalDateTime
        GIT_TAG origin/main
        OPTIONS
        "ENABLE_TIME_ZONE_SUPPORT ON"
        "ENABLE_TIME_ZONE_HISTORIC_RULES OFF"
)

target_link_libraries(${PROJECT_NAME} GlobalDateTime)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf GlobalDateTime)
```

## Usage

### Single include for all features 
**Note**: see package description if only particular implementations needed 

```c
  #include "GlobalDateTime.h"
```

### Get current date and time

**Note**: those function implementation should be at particular driver (RTC, Tick, Internet time) side.

```c
  Time time = timeNow();
  Date date = dateNow();
  Instant instant = instantNow();
  DateTime dateTime = dateTimeNow();
```

### Get current date at UTC

```c
  ZonedDateTime zDateTime = zonedDateTimeNow(&UTC); // zoned date-time creation with time zone alignment and predefined UTC zone
```

### Create instant from epoch seconds

```c
  Instant instant = instantOfEpochSeconds(1654540212); // Epoch time in seconds. GMT: Monday, 6 June 2022 year, 18:30:12
  Instant instant = instantOfEpochSecondsAdjust(1654540212, 28800 * MICROS_PER_SECOND); // plus 8 hour adjustment
  Instant instant = instantOfEpochMillis(1654559925246); // Epoch time in millis. UTC: Mon Jun 06 2022 23:58:45
```

### Create a time (hour, minute, second, millis)

```c
  Time time = timeOf(13, 22, 45); // hours, minutes, seconds
  Time time = timeOfWithMillis(13, 22, 45, 678); // hours, minutes, seconds, milliseconds
  Time time = timeOfHourAndMinutes(13, 22); // hours, minutes
```

### Create a time from Instant

```c
  Instant instant = instantOfEpochSeconds(1654540212); // GMT: Monday, 6 June 2022 year, 18:30:12
  Time time = timeOfInstant(&instant, &UTC);     // 18:30:12
```

### Create a time from seconds, millis and micros of day

```c
  Time time = timeOfSecondOfDay(86300);  // 23:58:20:000
  Time time = timeOfMillisOfDay(86300 * 1000);  // 23:58:20:001
  Time time = timeOfMicrosOfDay(86300 * 1000000LL);  // 23:58:20:001
```

### Create a date (year, month, day)

```c
  Date date = dateOf(2014, 7, 23);
  Date date = dateOf(2014, JULY, 23);  // with predefined month
```

### Create a date (additional functions)

```c
  Date date = dateOfEpochDay(3567); // 1979.10.08 Monday. Days count from 1970-01-01
  Date date = dateOfYearDay(2014, 187);  // 2014.07.06 Sunday. Days of year from 1 to 366
```

### Create a date from Instant

```c
  Instant instant = instantOfEpochSeconds(1654540212); // GMT: Monday, 6 June 2022 year, 18:30:12
  Date date = dateOfInstant(&instant, &UTC); // 2022.06.06 Monday
```

### Create a date-time (year, month, day, hour, minute, second, millisecond)

```c
  DateTime dateTime = dateTimeOf(2008, JUNE, 23, 14, 6);
  DateTime dateTime = dateTimeOfWithSeconds(2008, JUNE, 23, 14, 6, 45);
  DateTime dateTime = dateTimeOfWithMillis(2008, JUNE, 23, 14, 6, 45, 436);
```

### Create a date-time from Date and Time

```c
  Date date = dateOf(2008, JUNE, 23);
  Time time = timeOf(14, 6, 45);
  DateTime dateTime = dateTimeFrom(&date, &time); // 2008.06.23 Monday 14:06:45
```

### Create a date-time from Instant

```c
  Instant instant = instantOfEpochSeconds(1654540212); // GMT: Monday, 6 June 2022 year, 18:30:12
  DateTime dateTime = dateTimeOfInstant(&instant, &UTC); // 2022.06.06 Monday 18:30:12
```

### Create a date-time from epoch seconds, micro adjustment and time zone offset

```c
  DateTime dateTime = dateTimeOfEpochSeconds(1654540212, 1000000, UTC.utcOffset); // 2022.06.06 Monday 18:30:13
```

### Create a date-time at midnight

```c
  Date date = dateOf(2008, JUNE, 23);
  DateTime dateTime = dateTimeAtStartOfDay(&date);   // 2008.06.23 Monday 00:00:00
```

### Create time zone

```c
  TimeZone zone1 = timeZoneOf("Z");
  TimeZone zone2 = timeZoneOf("-0800");
  TimeZone zone3 = timeZoneOf("+0810");
  TimeZone zone4 = timeZoneOf("+08:10");
  TimeZone zone5 = timeZoneOf("+08:10:20");
  TimeZone zone6 = timeZoneOf("GMT+3");
  TimeZone zone7 = timeZoneOf("UTC-8");
  TimeZone zone8 = timeZoneOf("GMT+0800");
  TimeZone zone9 = timeZoneOf("GMT+0825");
  TimeZone zone10 = timeZoneOf("GMT+08:25");
  TimeZone zone11 = timeZoneOf("GMT+08:25:10");
  TimeZone zone12 = timeZoneOf("Europe/Paris");   // find by full name, also set zone DST rules and names
  
  TimeZone customZone = { "Custom_Zone", 28800/*in seconds*/, UTC_ZONE_NAMES, NULL };    // zone with UTC names, offset of 8 hours and no DST rules
```

### Create a zoned date-time

```c
  TimeZone zone = timeZoneOf("Europe/Paris");
  ZonedDateTime zonedDateTime = zonedDateTimeOf(2014, AUGUST, 23, 16, 3, 56, 456, &zone);  // 2014.08.23 Saturday 16:03:56:456
```

### Create a zoned date-time from Date and Time

```c
  Date date = dateOf(2008, JUNE, 23);
  Time time = timeOf(14, 6, 45);
  TimeZone zone = timeZoneOf("-0800");
  ZonedDateTime zonedDateTime = zonedDateTimeFrom(&date, &time, &zone);
```

### Create a zoned date-time from DateTime

```c
  DateTime dateTime = dateTimeOf(2008, JUNE, 23, 14, 6);
  ZonedDateTime zonedDateTime = zonedDateTimeOfDateTime(&dateTime, &UTC);
```

### Create a zoned date-time from Instant

```c
  TimeZone zone2 = timeZoneOf("+03:00");
  Instant instant = instantOfEpochSeconds(1654540212); // GMT: Monday, 6 June 2022 year, 18:30:12
  ZonedDateTime zonedDateTime = zonedDateTimeOfInstant(&instant, &zone2); // 2022.06.06 Monday 18:30:12
```

### Create a zoned date-time from other zoned date-time (date and time adjustment according zone offset and DST rules)

```c
  TimeZone zoneParis = timeZoneOf("Europe/Paris");
  TimeZone zoneGmt = timeZoneOf("GMT-14");
  ZonedDateTime zonedDateTime4 = zonedDateTimeOf(2014, AUGUST, 23, 16, 3, 56, 456, &zoneParis); // UTC: 2014.08.23 Saturday 16:03:56:456
  ZonedDateTime gmtDateTime = zonedDateTimeWithSameInstant(&zonedDateTime4, &zoneGmt);          // GMT-14: 2014.08.23 Saturday 00:03:56:456
```

### Instant manipulations
Check header files for full function description
```c
  Instant first = instantOfEpochSeconds(1654540212);   // GMT: Monday, 6 June 2022 year, 18:30:12
  instantPlusSeconds(&first, 1000);
  instantPlusMillis(&first, 99999999);
  instantMinusMicros(&first, 999999999);  // 2022.06.07 22:16:51:999
  
  int64_t epochMillis = instantToEpochMillis(&first);
  Instant second = instantOfEpochMillis(epochMillis);
  instantMinusMillis(&second, 10000); // 2022.06.07 22:16:41:999
  
  printf("Is first after second: %s\n", isInstantAfter(&first, &second) ? "Yes" : "No");  // Yes
  printf("Is second before first: %s\n", isInstantBefore(&second, &first) ? "Yes" : "No"); // Yes
  printf("Is first equals second: %s\n", isInstantEquals(&first, &second) ? "Yes" : "No"); // No
```

### Date-time manipulations
**Note**: Time, Date and ZonedDateTime have same functionality
```c
  DateTime first = dateTimeOf(2007, FEBRUARY, 28, 12, 23);   // Non leap year
  dateTimePlusDays(&first, 1);    // February have 28 days. 2007.03.01 12:23
  
  dateTimePlusYears(&first, 1);       // Moving to leap year
  dateTimeMinusDays(&first, 1);       // February have 29 days. 2008.02.29 12:23
  dateTimePlusWeeks(&first, -2);      // 2008.02.15 12:23
  dateTimeMinusMonths(&first, 1);     // 2008.01.15 12:23
  dateTimePlusHours(&first, 36);      // 2008.01.17 00:23
  dateTimePlusMinutes(&first, 136);   // 2008.01.17 02:39
  dateTimePlusMillis(&first, 2345);   // 2008.01.17 02:39:02:345
  dateTimeMinusMillis(&first, 1000);  // 2008.01.17 02:39:01:345
  
  DateTime second = first;
  dateTimeMinusWeeks(&second, 4); // Move to the previous year. 2007.12.20 02:39:01:345
  printf("Is first after second: %s\n", isDateTimeAfter(&first, &second) ? "Yes" : "No");  // Yes
  printf("Is second before first: %s\n", isDateTimeBefore(&second, &first) ? "Yes" : "No"); // Yes
  printf("Is first equals second: %s\n", isDateTimeEquals(&first, &second) ? "Yes" : "No"); // No
```
### Formatting date-time and parsing string(Date, Time, DateTime, ZonedDateTime, TimeZone)
See below all available parser and format types
```c
  char buffer[64] = {0};
  DateTimeFormatter formatter;
  parseDateTimePattern(&formatter, "yyyy-MM-dd'T'HH:mm:ss.SSS ZZZZZ");

  // ZonedDateTime to string
  TimeZone zone = timeZoneOf("UTC+8");
  ZonedDateTime zonedDateTime = zonedDateTimeOf(2008, JULY, 23, 14, 34, 23, 456, &zone);
  formatZonedDateTime(&zonedDateTime, buffer, 64, &formatter);
  printf("%s\n", buffer); // 2008-07-23T14:34:23.456 +08:00
  
  //Convert String to DateTime
  char *date = "Mon 2001/07/23 13:44:54:456";
  DateTimeFormatter dateParser;
  parseDateTimePattern(&dateParser, "EEE yyyy/MM/dd HH:mm:ss:SSS");
  DateTime ldt = parseToDateTime(date, &dateParser);
  if (isDateTimeValid(&ldt)) {        // check that date-time parsed
      printf("%s %lld-%02d-%02d %d:%d:%d:%d",     // Monday 2001-07-23 13:44:54:456
             getWeekDayNameLong(ldt.date.weekDay),
             ldt.date.year,
             ldt.date.month,
             ldt.date.day,
             ldt.time.hours,
             ldt.time.minutes,
             ldt.time.seconds,
             ldt.time.millis);
  }
```

### Zoned date-time feature (DST rules support)
- [Daylight Saving Time (DST)](https://en.wikipedia.org/wiki/Daylight_saving_time)
- Paris, normally UTC+1, has DST (add one hour = UTC+2) from 27/Mar to 30/Oct 2016. Review the above output, and the API can calculate and handle the DST correctly.

```c
  char buffer[64] = {0};
  DateTimeFormatter formatter;
  parseDateTimePattern(&formatter, "HHmm, dd MMM yyyy ZZZZ");
  
  // Convert String to DateTime
  char *date = "2016-08-22 14:30";
  DateTimeFormatter dateParser;
  parseDateTimePattern(&dateParser, "yyyy-MM-dd HH:mm");
  DateTime ldt = parseToDateTime(date, &dateParser);
  
  // Paris, 2016 Apr-Oct = DST, UTC+2, other months UTC+1
  TimeZone parisZone = timeZoneOf("Europe/Paris");
  ZonedDateTime parisDateTime = zonedDateTimeOfDateTime(&ldt, &parisZone);
  
  // hard code a zone offset like this, UTC-5
  TimeZone nyZone = timeZoneOf("-05:00");
  ZonedDateTime nyDateTime = zonedDateTimeWithSameInstant(&parisDateTime, &nyZone);
  zonedDateTimePlusHours(&nyDateTime, 8);
  zonedDateTimePlusMinutes(&nyDateTime, 10);
  
  formatZonedDateTime(&parisDateTime, buffer, 64, &formatter);
  printf("Depart : %s\n", buffer);    // Depart : 1430, 22 Aug 2016 GMT+02:00
  
  memset(buffer, 0, 64);
  formatZonedDateTime(&nyDateTime, buffer, 64, &formatter);
  printf("Arrive : %s\n", buffer);    // Arrive : 1540, 22 Aug 2016 GMT-05:00
```


### Supported format operators

<table>
<thead>
  <tr>
    <th>Field Type</th>
    <th>Symbol</th>
    <th>Field patterns</th>
    <th>Examples</th>
    <th>Description</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="5">Year</td>
    <td rowspan="5">y</td>
    <td>y</td>
    <td>2, 20, 201, 2017, 20173</td>
    <td rowspan="5">Calendar year (numeric)</td>
  </tr>
  <tr>
    <td>yy</td>
    <td>02, 20, 01, 17, 73</td>
  </tr>
  <tr>
    <td>yyy</td>
    <td>002, 020, 201, 2017, 20173</td>
  </tr>
  <tr>
    <td>yyyy</td>
    <td>0002, 0020, 0201, 2017, 20173</td>
  </tr>
  <tr>
    <td>yyyyy</td>
    <td>00002, 00020, 00201, 02017, 20173</td>
  </tr>
  <tr>
    <td></td>
    <td>u</td>
    <td>u...uuuu</td>
    <td>4601</td>
    <td>Extended year (numeric). Works same as&nbsp;&nbsp;'y' pattern</td>
  </tr>
  <tr>
    <td rowspan="4">Month</td>
    <td rowspan="4">M</td>
    <td>M</td>
    <td>9, 12</td>
    <td rowspan="4">Month number/name</td>
  </tr>
  <tr>
    <td>MM</td>
    <td>09, 12</td>
  </tr>
  <tr>
    <td>MMM</td>
    <td>Sep</td>
  </tr>
  <tr>
    <td>MMMM</td>
    <td>September</td>
  </tr>
  <tr>
    <td rowspan="3">Week</td>
    <td rowspan="2">w</td>
    <td>w</td>
    <td>8, 27</td>
    <td rowspan="2">Week of Year (numeric)</td>
  </tr>
  <tr>
    <td>ww</td>
    <td>08, 27</td>
  </tr>
  <tr>
    <td>W</td>
    <td>W</td>
    <td>3</td>
    <td>Week of Month (numeric)</td>
  </tr>
  <tr>
    <td rowspan="4">Day</td>
    <td rowspan="2">d</td>
    <td>d</td>
    <td>1</td>
    <td rowspan="2">Day of month (numeric)</td>
  </tr>
  <tr>
    <td>dd</td>
    <td>01</td>
  </tr>
  <tr>
    <td>D</td>
    <td>D...DDD</td>
    <td>345</td>
    <td>Day of year (numeric)</td>
  </tr>
  <tr>
    <td>F</td>
    <td>F</td>
    <td>2</td>
    <td>Day of Week in Month (numeric). The example is for the 2nd Wed in July</td>
  </tr>
  <tr>
    <td rowspan="3">Week day</td>
    <td rowspan="3">E</td>
    <td>E...EEE</td>
    <td>Tue</td>
    <td rowspan="3">Day of week name</td>
  </tr>
  <tr>
    <td>EEEE</td>
    <td>Tuesday</td>
  </tr>
  <tr>
    <td>EEEEE</td>
    <td>T</td>
  </tr>
  <tr>
    <td>Period</td>
    <td>a</td>
    <td>a</td>
    <td>AM,PM</td>
    <td>Uppercase by default</td>
  </tr>
  <tr>
    <td rowspan="8">Hour</td>
    <td rowspan="2">h</td>
    <td>h</td>
    <td>1, 12</td>
    <td rowspan="2">Hour [1-12]</td>
  </tr>
  <tr>
    <td>hh</td>
    <td>01, 12</td>
  </tr>
  <tr>
    <td rowspan="2">H</td>
    <td>H</td>
    <td>0, 23</td>
    <td rowspan="2">Hour [0-23]</td>
  </tr>
  <tr>
    <td>HH</td>
    <td>00, 23</td>
  </tr>
  <tr>
    <td rowspan="2">K</td>
    <td>K</td>
    <td>0, 11</td>
    <td rowspan="2">Hour [0-11]</td>
  </tr>
  <tr>
    <td>KK</td>
    <td>00, 11</td>
  </tr>
  <tr>
    <td rowspan="2">k</td>
    <td>k</td>
    <td>1, 24</td>
    <td rowspan="2">Hour [1-24]</td>
  </tr>
  <tr>
    <td>kk</td>
    <td>01, 24</td>
  </tr>
  <tr>
    <td rowspan="2">Minute</td>
    <td rowspan="2">m</td>
    <td>m</td>
    <td>8, 59</td>
    <td rowspan="2">Minute (numeric)</td>
  </tr>
  <tr>
    <td>mm</td>
    <td>08, 59</td>
  </tr>
  <tr>
    <td rowspan="2">Second</td>
    <td rowspan="2">s</td>
    <td>s</td>
    <td>8, 12</td>
    <td rowspan="2">Second (numeric)</td>
  </tr>
  <tr>
    <td>ss</td>
    <td>08, 12</td>
  </tr>
  <tr>
    <td rowspan="3">Millisecond</td>
    <td rowspan="3">S</td>
    <td>S</td>
    <td>1, 56, 678</td>
    <td rowspan="3">Fractional second (numeric)</td>
  </tr>
  <tr>
    <td>SS</td>
    <td>01, 56, 678</td>
  </tr>
  <tr>
    <td>SSS</td>
    <td>001, 056, 678</td>
  </tr>
  <tr>
    <td rowspan="5">Zone</td>
    <td rowspan="2">z</td>
    <td>z...zzz</td>
    <td>PDT</td>
    <td>Short non-location format</td>
  </tr>
  <tr>
    <td>zzzz</td>
    <td>Pacific Daylight Time</td>
    <td>Long non-location format</td>
  </tr>
  <tr>
    <td rowspan="3">Z</td>
    <td>Z...ZZZ</td>
    <td>-0800</td>
    <td>The ISO8601 basic format with hours</td>
  </tr>
  <tr>
    <td>ZZZZ</td>
    <td>GMT-8:00</td>
    <td>The long localized GMT format</td>
  </tr>
  <tr>
    <td>ZZZZZ</td>
    <td>-08:00, -07:52:58</td>
    <td>The ISO8601 extended format with hours, minutes and optional seconds fields</td>
  </tr>
</tbody>
</table>

### Supported string parse operators

<table>
<thead>
  <tr>
    <th>Field Type</th>
    <th>Symbol</th>
    <th>Field patterns</th>
    <th>Examples</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="5">Year</td>
    <td rowspan="5">y</td>
    <td>y</td>
    <td>2</td>
  </tr>
  <tr>
    <td>yy</td>
    <td>02, 20</td>
  </tr>
  <tr>
    <td>yyy</td>
    <td>002, 020, 201</td>
  </tr>
  <tr>
    <td>yyyy</td>
    <td>0002, 0020, 0201, 2017</td>
  </tr>
  <tr>
    <td>yyyyy</td>
    <td>00002, 00020, 00201, 02017, 23017</td>
  </tr>
  <tr>
    <td rowspan="4">Month</td>
    <td rowspan="4">M</td>
    <td>M</td>
    <td>9</td>
  </tr>
  <tr>
    <td>MM</td>
    <td>09, 12</td>
  </tr>
  <tr>
    <td>MMM</td>
    <td>Sep</td>
  </tr>
  <tr>
    <td>MMMM</td>
    <td>September</td>
  </tr>
  <tr>
    <td rowspan="2">Day</td>
    <td rowspan="2">d</td>
    <td>d</td>
    <td>1</td>
  </tr>
  <tr>
    <td>dd</td>
    <td>01, 23</td>
  </tr>
  <tr>
    <td rowspan="3">Week day</td>
    <td rowspan="3">E</td>
    <td>E...EEE</td>
    <td>Tue</td>
  </tr>
  <tr>
    <td>EEEE</td>
    <td>Tuesday</td>
  </tr>
  <tr>
    <td>EEEEE</td>
    <td>T</td>
  </tr>
  <tr>
    <td>Period</td>
    <td>a</td>
    <td>a</td>
    <td>AM, PM, am, pm</td>
  </tr>
  <tr>
    <td rowspan="4">Hour</td>
    <td rowspan="2">h</td>
    <td>h</td>
    <td>1</td>
  </tr>
  <tr>
    <td>hh</td>
    <td>01, 12</td>
  </tr>
  <tr>
    <td rowspan="2">H</td>
    <td>H</td>
    <td>0</td>
  </tr>
  <tr>
    <td>HH</td>
    <td>00, 23</td>
  </tr>
  <tr>
    <td rowspan="2">Minute</td>
    <td rowspan="2">m</td>
    <td>m</td>
    <td>8</td>
  </tr>
  <tr>
    <td>mm</td>
    <td>08, 59</td>
  </tr>
  <tr>
    <td rowspan="2">Second</td>
    <td rowspan="2">s</td>
    <td>s</td>
    <td>8</td>
  </tr>
  <tr>
    <td>ss</td>
    <td>08, 12</td>
  </tr>
  <tr>
    <td rowspan="3">Millisecond</td>
    <td rowspan="3">S</td>
    <td>S</td>
    <td>1</td>
  </tr>
  <tr>
    <td>SS</td>
    <td>01, 56</td>
  </tr>
  <tr>
    <td>SSS</td>
    <td>001, 056, 678</td>
  </tr>
  <tr>
    <td>Zone</td>
    <td>Z</td>
    <td>Z</td>
    <td>Z, <br>GMT<br>+0800, <br>GMT-8:00, <br>-08:00, <br>-07:52:58,<br>America/Los_Angeles</td>
  </tr>
</tbody>
</table>

