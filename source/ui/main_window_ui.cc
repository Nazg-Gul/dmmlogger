// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/main_window.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

QWidget *MainWindow::CreateCentralWidget(void) {
  //
  // +-[ Main Widget ]------------------------------+
  // |+-[ Main Layout [-----+----------------------+|
  // ||+-[ Vert Layout W ]-+|                      ||
  // |||+-[ Vert Layout ]-+||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // |||+-----------------+||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // ||||                 |||                      ||
  // |||+-----------------+||                      ||
  // ||+-------------------+|                      ||
  // |+---------------------+----------------------+|
  // +----------------------------------------------+
  //

  QWidget *central_widget = new QWidget();

  QHBoxLayout *main_layout = new QHBoxLayout();
  main_layout->setMargin(0);
  central_widget->setLayout(main_layout);

  QVBoxLayout *vertical_layout = new QVBoxLayout();
  vertical_layout->setMargin(0);
  QWidget *vertical_layout_widget = new QWidget();
  vertical_layout_widget->setLayout(vertical_layout);
  main_layout->addWidget(vertical_layout_widget);

  // TODO(sergey): What da heck TableWIdget culd not be smaller 255px?
  const int left_column_width = 255;

  QFrame *display_frame = new QFrame();
  display_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  display_frame->setFrameShape(QFrame::Panel);
  display_frame->setMinimumSize(left_column_width, 64);
  vertical_layout->addWidget(display_frame);

  const int padding = 8;
  value_display_label_ = new QLabel(display_frame);
  value_display_label_->setText("-0-");
  value_display_label_->setGeometry(
      padding, padding,
      display_frame->minimumSize().width() - padding * 2,
      display_frame->minimumSize().height() - padding * 2);
  value_display_label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  QFont font = value_display_label_->font();
  font.setPointSize(14);
  font.setBold(true);
  value_display_label_->setFont(font);

  // Create table used to display logged valued.
  values_table_ = new QTableWidget();
  values_table_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  values_table_->setMinimumSize(left_column_width, 48);
  values_table_->setColumnCount(3);
  values_table_->setColumnWidth(0, 35);
  QStringList header_labels;
  header_labels.append("#");
  header_labels.append("Value");
  header_labels.append("Units");
  values_table_->setHorizontalHeaderLabels(header_labels);
  vertical_layout->addWidget(values_table_);

  // Create graph display widget.
  graph_display_ = new GraphWidget();
  graph_display_->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
  main_layout->addWidget(graph_display_);

  return central_widget;
}
