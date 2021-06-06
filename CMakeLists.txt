cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(jack_response)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(FFTW3)
find_package(JACK)

include_directories(include ${FFTW3_INCLUDE_DIR} ${JACK_INCLUDE_DIR})

add_executable(record_frequency_response
    src/record_frequency_response.cpp
)

target_link_libraries(record_frequency_response ${FFTW3_LIBRARY} ${JACK_LIBRARY})

install(TARGETS record_frequency_response DESTINATION bin)