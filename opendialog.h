#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include <QDir>
#include "database.h"
#include "supervector.h"

namespace Ui {
  class OpenDialog;
}

class OpenDialog : public QDialog
{
  Q_OBJECT

public:
  explicit OpenDialog(QDir* n_dataBasesDir, QWidget *parent = 0);
  ~OpenDialog();
  void refresh();

private:
  Ui::OpenDialog *ui;
  QDir* dataBasesDir;

  void checkDataBases();

private slots:
  void onOkButton();

signals:
  void getDataBase(QString dbName);
};

#endif // OPENDIALOG_H
