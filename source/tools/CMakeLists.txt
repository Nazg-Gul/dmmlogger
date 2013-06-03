# DMMLogger - A logging software for digital multimeters
# Copyright 2013 Sergey Sharybin. All rights reserved.
#
# Author: sergey.vfx@gmail.com (Sergey Sharybin)

add_executable(read_dmm read_dmm.cc)
target_link_libraries(read_dmm
                      dmm)
