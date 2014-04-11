# DMMLogger - A logging software for digital multimeters
# Copyright 2013 Sergey Sharybin. All rights reserved.
#
# Author: sergey.vfx@gmail.com (Sergey Sharybin)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(../third_party)

SET(SRC
    graph_widget.cc
    main.cc
    main_window.cc
    main_window_ui.cc
    reader_thread.cc)

if(WITH_DUMMY_DMM)
  add_definitions(-DWITH_DUMMY_DMM)
endif()

QT4_WRAP_CPP(SRC
             main_window.h)
QT4_ADD_RESOURCES(SRC dmmlogger.qrc)

add_executable(qt-dmmlogger ${SRC})
target_link_libraries(qt-dmmlogger
                      qcustomplot
                      dmm
                      ${QT_LIBRARIES})
