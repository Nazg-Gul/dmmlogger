// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include "ui/main_window.h"

#include <cassert>
#include <QAction>
#include <QPainter>
#include <QToolBar>

#include "dmm/dummy_dmm.h"
#include "dmm/ut60e.h"

#include "dmm/continuous_trigger.h"
#include "dmm/settle_trigger.h"

#include <stdio.h>

MainWindow::MainWindow(void)
    : dmm_(NULL) {
  connect(&timer_, SIGNAL(timeout()), SLOT(HandleTimer()));

  setWindowTitle("DMM Logger");
  setMinimumSize(320, 240);

  QToolBar *toolbar = addToolBar("Main Toolbar");

  connect_action_ =
      toolbar->addAction(QIcon(":/connect"),
                         "Connect",
                         this,
                         SLOT(ToggleConnection(bool /* connect */)));
  connect_action_->setCheckable(true);

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
  dmm_ = new UT60E("/dev/ttyUSB0");
  // dmm_ = new DummyDMM();
  if (!dmm_->Connect()) {
    // Error happened while connecting to the DMM.
    // TODO(sergey): Show error message.
    delete dmm_;
    dmm_ = NULL;
    return;
  }

  trigger_ = new ContinuousTrigger();

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
