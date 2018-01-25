#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTableWidgetItem>
#include <vector>
#include "defines.h"
#include "database.h"
#include "newdialog.h"
#include "opendialog.h"
#include "finddialog.h"
#include "mergedialog.h"
#include "aboutdialog.h"
#include "supervector.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void addDataBase(QString dbName, QString modelName, bool loadFromFile = false); // функция, выполняющаяся после окна создания
  void openDataBase(QString dbName); // функция, выполняющаяся после окна открытия
  void find(QString property, QString value, int operation); // функция поиска
  void refresh(); // функция, обновляющая таблицу
  // file
  void newAction();
  void openAction();
  void saveAction();
  void saveAsAction();
  void modelManagerAction();
  void quitAction();
  // edit
  void swapUpAction();
  void swapDownAction();
  void minimizeColumnSize();
  // help
  void helpAction();
  void aboutAction();

  void on_create_clicked();
  void on_comboBox_currentIndexChanged(const QString &dbName); // при смене базы данных
  void on_tableWidget_itemChanged(QTableWidgetItem *item); // при изменении ячейки таблицы
  void on_remove_clicked();
  void on_find_clicked();
  void on_merge_clicked();

private:
  Ui::MainWindow *ui;
  DataBasesMap *bases = new DataBasesMap;
  NewDialog* newDialog;
  OpenDialog* openDialog;
  FindDialog* findDialog;
  MergeDialog* mergeDialog;
  AboutDialog* aboutDialog;
  QDir* modelsDir;
  QDir* dataBasesDir;
  QString currentBaseName = "";
  int comboboxItemsNum = 0;
  bool codeChangesTable = false;
  bool findingMode = false;

  void setButtonsEnabled(bool enabled, bool exceptFind = false); // включает/выключает кнопки для работы с базой данных
  void loadDataBase(QString dbName); // загружает базу данных в таблицу
  void addDataBaseToComboBox(QString dbName); // добавляет базу данных в ComboBox
  QDir* setFilesDir(QString dirName, QString extension);
};

#endif // MAINWINDOW_H
