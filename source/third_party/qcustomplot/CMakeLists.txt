# DMMLogger - A logging software for digital multimeters
# Copyright 2013 Sergey Sharybin. All rights reserved.
#
# Author: sergey.vfx@gmail.com (Sergey Sharybin)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

SET(SRC
    qcustomplot.cpp)
QT4_WRAP_CPP(SRC
             qcustomplot.h)

add_library(qcustomplot ${SRC})
target_link_libraries(qcustomplot
                      ${QT_LIBRARIES})
