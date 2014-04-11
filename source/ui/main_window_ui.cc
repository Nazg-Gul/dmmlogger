// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/main_window.h"

#include <QFrame>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

void MainWindow::FillModelsList(void) {
  std::vector<DMMFactory::DMMModel> models = dmm_factory_.GetModels();
  for (int i = 0; i < models.size(); ++i) {
    DMMFactory::DMMModel &model = models[i];
    model_chooser_->addItem(QString(model.name.c_str()),
                            QVariant(static_cast<int>(model.type)));
  }
  // TODO(sergey): Restore previously used multimeter.
}

void MainWindow::FillTriggersList(void) {
  std::vector<TriggerFactory::TriggerInfo> triggers =
      trigger_factory_.GetTriggers();
  for (int i = 0; i < triggers.size(); ++i) {
    TriggerFactory::TriggerInfo &trigger = triggers[i];
    trigger_chooser_->addItem(QString(trigger.name.c_str()),
                              QVariant(static_cast<int>(trigger.type)));
  }
  // TODO(sergey): Restore previously used multimeter.
}

void MainWindow::CreateToolBar(void) {
  // Create tool bar itself.
  QToolBar *toolbar = addToolBar("Main Toolbar");

  // Drop-down list for the DMM model selection.
  model_chooser_ = new QComboBox();
  toolbar->addWidget(model_chooser_);
  FillModelsList();

  // Drop-down list for the device selection.
  device_chooser_ = new QComboBox();
  toolbar->addWidget(device_chooser_);
  device_chooser_->setEditable(true);

  // TODO(sergey): Need to support other platforms here?
#ifndef WIN32
  device_chooser_->setEditText("/dev/ttyUSB0");
  device_chooser_->setMinimumContentsLength(12);
#else
  device_chooser_->setEditText("COM1");
  device_chooser_->setMinimumContentsLength(6);
#endif

  // Drop-down list for the trigger selection.
  trigger_chooser_ = new QComboBox();
  toolbar->addWidget(trigger_chooser_);
  FillTriggersList();

  // TODO(sergey): Fill in the combo box with more choises.
  // TODO(sergey): Also support saving the last used device.

  // Connect to the device button.
  connect_action_ =
      toolbar->addAction(QIcon(":/connect"),
                         "Connect",
                         this,
                         SLOT(ToggleConnection(bool /* connect */)));
  connect_action_->setCheckable(true);
}

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

void MainWindow::SetDMMSettingsEnabled(bool enabled) {
  model_chooser_->setEnabled(enabled);
  device_chooser_->setEnabled(enabled);
  trigger_chooser_->setEnabled(enabled);
}
