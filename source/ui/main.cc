// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include <QApplication>

#include "ui/main_window.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  MainWindow window;
  window.show();
  return application.exec();
}
