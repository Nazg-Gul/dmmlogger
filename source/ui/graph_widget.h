// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef UI_GRAPH_WIDGET_H_
#define UI_GRAPH_WIDGET_H_

#include <vector>
#include <QWidget>

class GraphWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GraphWidget(QWidget *parent = 0);

  void Clear(void);
  void AddValue(float value);

 protected:
  void paintEvent(QPaintEvent *event);

  std::vector<float> values_;
};

#endif
