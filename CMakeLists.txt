cmake_minimum_required(VERSION 3.16)

project(FlightReservationSystem VERSION 1.0 LANGUAGES C CXX)

# ---------------------------
# Compiler settings
# ---------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-w)
endif()

# ---------------------------
# Include directories
# ---------------------------
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/json)

# ---------------------------
# SQLite static library
# ---------------------------
add_library(sqlite3_static STATIC ${CMAKE_SOURCE_DIR}/sqlite/sqlite3.c)
set_property(TARGET sqlite3_static PROPERTY LINKER_LANGUAGE C)
set_property(TARGET sqlite3_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(sqlite3_static PUBLIC ${CMAKE_SOURCE_DIR}/sqlite)

# ---------------------------
# Common sources (used by both executables)
# ---------------------------
set(COMMON_SOURCES
    # Entities (includes manager)
    entities/customer.cpp
    entities/flight.cpp
    entities/airport.cpp
    entities/plane.cpp
    entities/ticket.cpp
    entities/offer.cpp
    entities/manager.cpp

    # Utilities
    utilities/idGenerator.cpp
    utilities/dateTime.cpp
    utilities/validator.cpp

    # FileIO
    fileIO/textReader.cpp
    fileIO/textWriter.cpp
    fileIO/dbHandler.cpp
)

# ---------------------------
# Console version (with menu UI)
# ---------------------------
set(CONSOLE_SOURCES
    main.cpp
    ui/menu.cpp
    ${COMMON_SOURCES}
)

add_executable(flightSystem ${CONSOLE_SOURCES})
target_include_directories(flightSystem PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(flightSystem PRIVATE sqlite3_static)
target_compile_definitions(flightSystem PRIVATE DATA_DIR="${CMAKE_SOURCE_DIR}/data")

# ---------------------------
# JSON version (for Electron) - backend
# ---------------------------
set(JSON_SOURCES
    backend.cpp
    json/json-manager.cpp
    ${COMMON_SOURCES}
)

add_executable(flightSystemJSON ${JSON_SOURCES})
target_include_directories(flightSystemJSON PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/json)
target_link_libraries(flightSystemJSON PRIVATE sqlite3_static)
target_compile_definitions(flightSystemJSON PRIVATE DATA_DIR="${CMAKE_SOURCE_DIR}/data")

# ---------------------------
# Install rules
# ---------------------------
install(TARGETS flightSystem flightSystemJSON DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION .)

# ---------------------------
# Print build info
# ---------------------------
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Building two executables:")
message(STATUS "  - flightSystem (Console UI - main.cpp)")
message(STATUS "  - flightSystemJSON (Electron Backend - backend.cpp)")
message(STATUS "Database: system-data.db")