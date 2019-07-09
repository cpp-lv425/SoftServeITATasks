cmake_minimum_required (VERSION 3.12)
project (SoftServeITATasks)

find_package (Boost)

set (PROJECT_LIBRARY_DIRECTORIES
        ${PROJECT_LIBRARY_DIRECTORIES}
        ${Boost_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/tasksCode
        )

set (PROJECT_CODE
        ${PROJECT_CODE}
        ${PROJECT_SOURCE_DIR}/tasksCode/Body.cpp
        ${PROJECT_SOURCE_DIR}/tasksCode/StringList.cpp
        ${PROJECT_SOURCE_DIR}/tasksCode/AnyType.hpp
        ${PROJECT_SOURCE_DIR}/tasksCode/CountCodeLines.cpp
        )

include_directories(${PROJECT_LIBRARY_DIRECTORIES})
add_executable(SoftServeITATasks ${PROJECT_CODE})

target_link_libraries(SoftServeITATasks -static)
target_link_libraries(SoftServeITATasks ${Boost_LIBRARIES})