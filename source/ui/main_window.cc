// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/main_window.h"

#include <cassert>
#include <QAction>
#include <QPainter>

#include "dmm/continuous_trigger.h"
#include "dmm/settle_trigger.h"

#include <stdio.h>

MainWindow::MainWindow(void)
    : dmm_(NULL) {
  connect(&timer_, SIGNAL(timeout()), SLOT(HandleTimer()));

  setWindowTitle("DMM Logger");
  setMinimumSize(320, 240);

  CreateToolBar();

  QWidget *central_widget = CreateCentralWidget();
  setCentralWidget(central_widget);
}

MainWindow::~MainWindow(void) {
  // If dmm_ is not NULL it means we're still connected to it,
  // and we need to disconnect from it before we close the
  // application.
  if (dmm_ != NULL) {
    DisconnectFromDMM();
  }
}

void MainWindow::ToggleConnection(bool connect) {
  if (connect) {
    ConnectToDMM();
    if (dmm_ == NULL) {
      connect_action_->setChecked(false);
    }
  } else {
    DisconnectFromDMM();
  }
}

void MainWindow::HandleTimer(void) {
  assert(dmm_ != NULL);
  DMMState state = dmm_->state();
  if (trigger_->triggered()) {
    trigger_->Reset();
    // TODO(sergey): it is possible next state was read from
    //               the multimeter after trigger was set,
    //               so perhaps state shall be stored in trigger,
    LogState(state);
  }
  UpdateDisplay(state);
}

void MainWindow::ConnectToDMM(void) {
  assert(dmm_ == NULL);
  int current_model_index = model_chooser_->currentIndex();
  QVariant model_type_variant = model_chooser_->itemData(current_model_index);
  DMMFactory::DMMModelType model_type =
      (DMMFactory::DMMModelType) model_type_variant.toInt();
  std::string device = device_chooser_->currentText().toStdString();
  dmm_ = dmm_factory_.CreateDMM(model_type, device);
  if (dmm_ == NULL) {
    assert(!"Factory should never return NULL");
    return;
  }
  if (!dmm_->Connect()) {
    // Error happened while connecting to the DMM.
    // TODO(sergey): Show error message.
    delete dmm_;
    dmm_ = NULL;
    return;
  }

  // TODO(sergey): De-duplicate with DMM creation.
  int current_trigger_index = trigger_chooser_->currentIndex();
  QVariant trigger_type_variant =
      trigger_chooser_->itemData(current_trigger_index);
  TriggerFactory::TriggerType trigger_type =
      (TriggerFactory::TriggerType) trigger_type_variant.toInt();
  trigger_ = trigger_factory_.CreateTrigger(trigger_type);

  SetDMMSettingsEnabled(false);
  reader_thread_ = new ReaderThread(dmm_, trigger_);
  reader_thread_->start();
  timer_.start(100);
}

void MainWindow::DisconnectFromDMM(void) {
  assert(dmm_ != NULL);
  timer_.stop();
  dmm_->RequestStop();
  reader_thread_->wait();
  delete reader_thread_;
  reader_thread_ = NULL;
  delete dmm_;
  delete trigger_;
  trigger_ = NULL;
  dmm_ = NULL;
  SetDMMSettingsEnabled(true);
}

void MainWindow::UpdateDisplay(const DMMState &state) {
  QString display_text;
  // TODO(sergey): replace stupid float->string conversion.
  display_text =
      QString("%1").arg(state.raw_value)
      + " "
      + MultiplierStringify(state.multiplier)
      + UnitsFromMode(state.mode);

  value_display_label_->setText(display_text);
}

void MainWindow::LogState(const DMMState &state) {
  int rowsCount = values_table_->rowCount();
  // Insert new row.
  values_table_->setRowCount(rowsCount + 1);

  // Fill in new row with items.
  values_table_->setItem(rowsCount, 0,
      new QTableWidgetItem(QString("%1").arg(rowsCount + 1)));
  values_table_->setItem(rowsCount, 1,
      new QTableWidgetItem(QString("%1").arg(state.raw_value)));
  values_table_->setItem(rowsCount, 2,
      new QTableWidgetItem(QString(MultiplierStringify(state.multiplier)) +
                           UnitsFromMode(state.mode)));

  // Make sure new row is visible on the screen.
  values_table_->scrollToItem(values_table_->item(rowsCount, 0));

  // TODO(sergey): use scaled value, bot raw one.
  graph_display_->AddValue(state.raw_value);
  graph_display_->update();
}
