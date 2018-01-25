#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "defines.h"
#include "database.h"

namespace Ui {
  class MergeDialog;
}

class MergeDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MergeDialog(DataBasesMap *n_bases, QWidget *parent = 0);
  ~MergeDialog();
  void refresh(QString n_currentBaseName);
  void loadDataBase(const QString &dbName, QTableWidget* tableWidget, bool loadModelOnly = false);
  void moveItem(QTableWidget* tableWidget, QTableWidget* tableWidget_2, int currentRow);

private slots:
  void onOkButton();
  void on_comboBox_currentIndexChanged(const QString &dbName);
  void on_right_clicked();
  void on_left_clicked();
  void on_rightFull_clicked();
  void on_leftFull_clicked();

private:
  Ui::MergeDialog *ui;
  DataBasesMap *bases;
  QString currentBaseName;

signals:
  void getMergeSignal();
};

#endif // MERGEDIALOG_H
