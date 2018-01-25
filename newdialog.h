#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QMap>
#include <QDir>
#include <QString>
#include "defines.h"
#include "database.h"

namespace Ui {
  class NewDialog;
}

class NewDialog : public QDialog {
  Q_OBJECT

public:
  explicit NewDialog(DataBasesMap *n_bases, QDir* n_modelsDir, QDir* n_dataBasesDir, QWidget *parent = 0);
  ~NewDialog();
  void refresh();

private slots:
  void on_lineEdit_textChanged(const QString &dbName);

private:
  Ui::NewDialog *ui;
  DataBasesMap *bases;
  QDir* modelsDir;
  QDir* dataBasesDir;
  void checkModels();
  inline void canBeDone(QPushButton* Ok);
  void onOkButton();
signals:
  void getNameAndModel(QString dataBaseName, QString modelName);
};

#endif // NEWDIALOG_H
