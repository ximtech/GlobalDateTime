cmake_minimum_required(VERSION 3.20)
project(DateTimeZF VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(ENABLE_TIME_ZONE_SUPPORT OFF)
set(ENABLE_TIME_ZONE_HISTORY_SUPPORT OFF)

set(SOURCE_FILES
        Date.c
        DateTime.c
        DateTimeZF.c
        Instant.c
        Time.c
        TimeZone.c
        ZonedDateTime.c
        include/DateTimeMath.h
        include/TimeZone.h
        include/TimeZoneNames.h
        include/TimeZoneRules.h
        include/ValueRange.h
        include/Instant.h
        include/Date.h
        include/Time.h
        include/DateTime.h
        include/ZonedDateTime.h
        include/DateTimeZF.h)

# Establish the project options
option(ENABLE_TIME_ZONE_SUPPORT "Set to ON to enable time zone list" ${ENABLE_TIME_ZONE_SUPPORT})
option(ENABLE_TIME_ZONE_HISTORIC_RULES "Set to ON to enable zone historic rules" ${ENABLE_TIME_ZONE_HISTORY_SUPPORT})

if (ENABLE_TIME_ZONE_SUPPORT)
    add_compile_definitions(ENABLE_TIME_ZONE_SUPPORT)
endif()

if (ENABLE_TIME_ZONE_HISTORIC_RULES)
    add_compile_definitions(ENABLE_TIME_ZONE_HISTORIC_RULES)
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Export
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Export
        FILE ${PROJECT_NAME}-config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})