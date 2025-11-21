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

# ---------------------------
# SQLite static library
# ---------------------------
add_library(sqlite3_static STATIC ${CMAKE_SOURCE_DIR}/sqlite/sqlite3.c)
set_property(TARGET sqlite3_static PROPERTY LINKER_LANGUAGE C)
set_property(TARGET sqlite3_static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(sqlite3_static PUBLIC ${CMAKE_SOURCE_DIR}/sqlite)

# ---------------------------
# Collect all project sources
# ---------------------------
set(PROJECT_SOURCES
    main.cpp

    # Entities
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

    # UI
    ui/menu.cpp
)

# ---------------------------
# Build executable directly from all sources
# ---------------------------
add_executable(flightSystem ${PROJECT_SOURCES})

target_include_directories(flightSystem PRIVATE ${CMAKE_SOURCE_DIR})

# ---------------------------
# Link only SQLite static library
# ---------------------------
target_link_libraries(flightSystem PRIVATE sqlite3_static)

# ---------------------------
# Define DATA_DIR macro for runtime access
# ---------------------------
target_compile_definitions(flightSystem PRIVATE DATA_DIR="${CMAKE_SOURCE_DIR}/data")

# ---------------------------
# Install rules
# ---------------------------
install(TARGETS flightSystem DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION .)
