cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME cppcms_simple)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

project(${PROJECT_NAME})
add_definitions(-std=c++11)

# Locate required libraries
find_library(LIB_BOOSTER booster)
find_library(LIB_CPPCMS cppcms HINTS /usr/local/lib)
find_library(LIB_CPPDB cppdb HINTS /usr/local/lib)

find_path(INC_BOOSTER booster/config.h)
find_path(INC_CPPCMS cppcms/config.h HINTS /usr/local/include)
find_path(INC_CPPDB cppdb/backend.h HINTS /usr/local/include)

# Add project source files
set(API_SRC
    main.cpp
    includes/controllers/User.cpp
    includes/controllers/Auth.cpp
    includes/data/Connection.cpp
    includes/data/Master.cpp
)

# Include directories
include_directories(${INC_BOOSTER} ${INC_CPPCMS} ${INC_CPPDB})
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${THIRD_PARTY_DIR}/picojson)

# Link directories
link_directories(/usr/local/lib /usr/lib)

# Build the executable
add_executable(${PROJECT_NAME} ${API_SRC})

# Ensure cppdb is properly linked
target_link_libraries(${PROJECT_NAME}
    ${LIB_BOOSTER}
    ${LIB_CPPCMS}
    ${LIB_CPPDB}
    pthread
)
