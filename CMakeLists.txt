CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (PLSA)

SET (SRC_DIR "${PROJECT_SOURCE_DIR}/src")

INCLUDE_DIRECTORIES (${SRC_DIR})

SET (SOURCE_FILES
    ${SRC_DIR}/main.cc
    ${SRC_DIR}/plsa.cc
    ${SRC_DIR}/corpora.cc
    ${SRC_DIR}/util.cc)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_EXECUTABLE (plsa ${SOURCE_FILES})
