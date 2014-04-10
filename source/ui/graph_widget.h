// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef UI_GRAPH_WIDGET_H_
#define UI_GRAPH_WIDGET_H_

#include <QVector>

#include "qcustomplot/qcustomplot.h"

class GraphWidget : public QCustomPlot {
 public:
  GraphWidget(void);

  void Clear(void);
  void AddValue(float value);

 protected:
  float x_;
};

#endif
