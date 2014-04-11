# DMMLogger - A logging software for digital multimeters
# Copyright 2013 Sergey Sharybin. All rights reserved.
#
# Author: sergey.vfx@gmail.com (Sergey Sharybin)

set(SRC
    continuous_trigger.cc
    dmm.cc
    dmm_factory.cc
    port.cc
    serial_dmm.cc
    settle_trigger.cc
    trigger.cc
    trigger_factory.cc
    ut60e.cc)

set(HDR
    continuous_trigger.h
    dmm.h
    dummy_dmm.h
    port.h
    serial_dmm.h
    settle_trigger.h
    trigger.h
    trigger_factory.h
    ut60e.h)

if(WITH_DUMMY_DMM)
  list(APPEND SRC
    dummy_dmm.cc
  )
  list(APPEND HDR
    dummy_dmm.h
  )
  add_definitions(-DWITH_DUMMY_DMM)
endif()

add_library(dmm ${SRC} ${HDR})

# Make the name of debug libraries end in _d.
set_target_properties(dmm PROPERTIES DEBUG_POSTFIX "_d")
