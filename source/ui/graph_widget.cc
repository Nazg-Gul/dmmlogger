// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/graph_widget.h"

#include <QPainter>

GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent) {
}

void GraphWidget::Clear(void) {
  values_.clear();
}

void GraphWidget::AddValue(float value) {
  values_.push_back(value);
}

namespace {
void DrawAxises(QPainter *painter,
                const QRect &rectangle) {
  const int arrow_half_width = 4;
  const int arrow_length = 12;

  // Draw Y-axis.
  painter->drawLine(rectangle.left(),
                    rectangle.top(),
                    rectangle.left(),
                    rectangle.bottom());

  painter->drawLine(rectangle.left(),
                    rectangle.top(),
                    rectangle.left() - arrow_half_width,
                    rectangle.top() + arrow_length);
  painter->drawLine(rectangle.left(),
                    rectangle.top(),
                    rectangle.left() + arrow_half_width,
                    rectangle.top() + arrow_length);

  // Draw X-axis.
  painter->drawLine(rectangle.left(),
                    rectangle.bottom(),
                    rectangle.right(),
                    rectangle.bottom());

  painter->drawLine(rectangle.right(),
                    rectangle.bottom(),
                    rectangle.right() - arrow_length,
                    rectangle.bottom() - arrow_half_width);
  painter->drawLine(rectangle.right(),
                    rectangle.bottom(),
                    rectangle.right() - arrow_length,
                    rectangle.bottom() + arrow_half_width);
}

void DrawValues(QPainter *painter,
                const QRect &rectangle,
                const std::vector<float> &values) {
  if (values.size() < 2) {
    return;
  }
  // TODO(sergey): make smarter autoscale as short-term and
  //               configurable scale as longterm.
  const float scale_x = 1.0f;
  const float scale_y = 1.0f;
  const float origin_x = rectangle.left();
  const float origin_y = rectangle.bottom();
  float previous_x = 0.0f;
  float previous_y = values[0] * scale_y;
  for (int i = 1; i < values.size() - 1; i++) {
    float x = static_cast<float>(i) * scale_x;
    float y = values[i] * scale_y;

    painter->drawLine(origin_x + previous_x,
                      origin_y - previous_y,
                      origin_x + x,
                      origin_y - y);
    previous_x = x;
    previous_y = y;
  }
}
}  // namespace

void GraphWidget::paintEvent(QPaintEvent * /* event */) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  QRect rectangle = rect();

  // Initialize backgound to white.
  painter.fillRect(rectangle, Qt::white);

  // Draw axises.
  const int padding = 8;
  QRect axis_rectangle(rectangle.left() + padding,
                       rectangle.top() + padding,
                       rectangle.width() - 2 * padding,
                       rectangle.height() - 2 * padding);

  painter.setPen(Qt::black);
  DrawAxises(&painter, axis_rectangle);

  // Draw values line.
  painter.setPen(Qt::blue);
  DrawValues(&painter, axis_rectangle, values_);

  painter.end();
}
