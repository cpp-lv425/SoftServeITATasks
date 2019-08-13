cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 14)
project (SoftServeITATasks)

find_package(Boost REQUIRED COMPONENTS filesystem thread atomic)

set (PROJECT_LIBRARY_DIRECTORIES
        ${PROJECT_LIBRARY_DIRECTORIES}
		${Boost_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/tasksCode
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
target_link_libraries(SoftServeITATasks ${Boost_LIBRARIES})