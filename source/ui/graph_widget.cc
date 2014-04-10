// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/graph_widget.h"

GraphWidget::GraphWidget(void) : x_(0.0f) {
  addGraph();
  xAxis->setRange(0, 250);
  yAxis->setRange(-50, 50);
}

void GraphWidget::Clear(void) {
  x_ = 0.0f;
  graph(0)->data()->clear();
}

void GraphWidget::AddValue(float value) {
  QCPData new_data;
  new_data.key = x_;
  new_data.value = value;
  graph(0)->data()->insertMulti(new_data.key, new_data);
  replot();
  x_ += 1.0f;
}
