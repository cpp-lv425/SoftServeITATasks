cmake_minimum_required (VERSION 3.12)
project (SoftServeITATasks)

set(BOOST_ROOT       "C:/Program Files/boost")
set(BOOST_LIBRARYDIR "C:/Program Files/boost/lib")
set(BOOST_INCLUDEDIR "C:/Program Files/boost/include/boost-1_69")

find_package(Boost REQUIRED COMPONENTS filesystem thread atomic regex chrono date_time)

set (PROJECT_LIBRARY_DIRECTORIES
        ${PROJECT_LIBRARY_DIRECTORIES}
        ${PROJECT_SOURCE_DIR}/tasksCode
        "C:/Program Files/boost/include/boost-1_69"
        )

set (PROJECT_CODE_FILES
        ${PROJECT_CODE_FILES}
        ${PROJECT_SOURCE_DIR}/tasksCode/Body.cpp
        ${PROJECT_SOURCE_DIR}/tasksCode/StringList.cpp
        ${PROJECT_SOURCE_DIR}/tasksCode/AnyType.hpp
        ${PROJECT_SOURCE_DIR}/tasksCode/CountCodeLines.cpp
        )

include_directories(${PROJECT_LIBRARY_DIRECTORIES})
add_executable(SoftServeITATasks ${PROJECT_CODE_FILES})
target_link_libraries(SoftServeITATasks Boost::filesystem Boost::thread Boost::atomic Boost::regex Boost::chrono Boost::date_time)