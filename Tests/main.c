#include "DateTime/LocalTimeTest.h"
#include "DateTime/LocalDateTest.h"
#include "DateTime/IntantTest.h"
#include "DateTime/LocalDateTimeTest.h"
#include "DateTime/ZonedDateTimeTest.h"
#include "DateTime/TimeZoneTest.h"
#include "DateTime/GlobalDateTimeTest.h"

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    MunitTest emptyTests[] = {END_OF_TESTS};
    MunitSuite testSuitArray[] = {
            timeTestSuite,
            dateTestSuite,
            instantTestSuite,
            dateTimeTestSuite,
            zonedDateTimeTestSuite,
            timeZoneTestSuite,
            dateTimeFormatterTestSuite
    };

    MunitSuite baseSuite = {
            .prefix = "",
            .tests = emptyTests,
            .suites = testSuitArray,
            .iterations = 1,
            .options = MUNIT_SUITE_OPTION_NONE
    };
    return munit_suite_main(&baseSuite, (void *) "µnit", argc, argv);
}
