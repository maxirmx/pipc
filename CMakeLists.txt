#
#   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
#   This file is a part of pipc
#

cmake_minimum_required(VERSION 3.16)
project(pipc)

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Building ${CMAKE_BUILD_TYPE} configuration")

find_package(iceoryx_platform REQUIRED)
find_package(iceoryx_posh CONFIG REQUIRED)
find_package(iceoryx_hoofs CONFIG REQUIRED)

execute_process(
  COMMAND "php-config" "--extension-dir"
  RESULT_VARIABLE EXTENSION_DIR_RES
  OUTPUT_VARIABLE EXTENSION_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT (EXTENSION_DIR_RES EQUAL 0))
  message(FATAL_ERROR "'php-config --extension-dir' failed")
endif()
message(STATUS "EXTENSION_DIR: ${EXTENSION_DIR}")

execute_process(
  COMMAND "php-config" "--ini-dir"
  RESULT_VARIABLE INI_DIR_RES
  OUTPUT_VARIABLE INI_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT (INI_DIR_RES EQUAL 0))
  message(FATAL_ERROR "'php-config --ini-dir' failed")
endif()
message(STATUS "INI_DIR: ${INI_DIR}")

execute_process(
  COMMAND "php-config" "--include-dir"
  RESULT_VARIABLE PHP_INCLUDES_RES
  OUTPUT_VARIABLE PHP_INCLUDES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT (INI_DIR_RES EQUAL 0))
  message(FATAL_ERROR "'php-config --php-includes' failed")
message(STATUS "INI_DIR: ${INI_DIR}")
endif()
message(STATUS "PHP_INCLUDES: ${PHP_INCLUDES}")

add_library(pipc SHARED
  src/object.cpp
  src/pipc.cpp
  "include/message.h"
  "include/object.h"
  "include/publisher.h"
  "include/subscriber.h"
)

target_compile_features(pipc PUBLIC cxx_std_17)
target_compile_options(pipc PUBLIC -Wall -O2 -fpic -march=native)
target_include_directories(pipc PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  /usr/local/include/iceoryx/v
  /usr/local/include/iceoryx/v2.90.0
  ${PHP_INCLUDES})

target_link_libraries(pipc PRIVATE
  uuid
  phpcpp
  iceoryx_posh
  iceoryx_hoofs
  iceoryx_platform
)

set(INI pipc.ini)
set(INI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/ini/${INI})
set(INI_COPY_TARGET ${EXTENSION_DIR}/${INI})
set(INI_LINK_TARGET ${INI_DIR}/20-${INI})

install(TARGETS pipc DESTINATION "${EXTENSION_DIR}")
install(FILES ${INI_SOURCE} DESTINATION "${EXTENSION_DIR}")
install(CODE "execute_process( \ COMMAND ${CMAKE_COMMAND} -E rm -rf \ ${INI_LINK_TARGET} \ )" )
install(CODE "execute_process( \ COMMAND ${CMAKE_COMMAND} -E create_symlink \ ${INI_COPY_TARGET} \ ${INI_LINK_TARGET} \ )" )
