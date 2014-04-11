// DMMLogger - A logging software for digital multimeters
// Copyright 2013 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef UI_MAIN_H_
#define UI_MAIN_H_

#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include <QWidget>

#include "dmm/dmm.h"
#include "dmm/trigger.h"
#include "ui/graph_widget.h"
#include "ui/reader_thread.h"

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(void);
  ~MainWindow(void);

 public slots:
  void ToggleConnection(bool connect);
  void HandleTimer(void);

 private:
  void CreateToolBar(void);
  QWidget *CreateCentralWidget(void);

  void ConnectToDMM(void);
  void DisconnectFromDMM(void);
  void UpdateDisplay(const DMMState &state);
  void UpdateGraph(void);
  void LogState(const DMMState &state);

  // Main DMM logic variables.
  DMM *dmm_;
  Trigger *trigger_;

  // Reader threads routines.
  ReaderThread *reader_thread_;
  QTimer timer_;

  // Interface elements.
  QLabel *value_display_label_;
  QAction *connect_action_;
  QComboBox *device_chooser_;
  GraphWidget *graph_display_;
  QTableWidget *values_table_;
};
#endif
