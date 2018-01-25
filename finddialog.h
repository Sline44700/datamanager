#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "defines.h"
#include "database.h"
#include "supervector.h"

namespace Ui {
  class FindDialog;
}

class FindDialog : public QDialog {
  Q_OBJECT

public:
  explicit FindDialog(DataBasesMap *n_bases, QWidget *parent = 0);
  ~FindDialog();
  void refresh(QString n_currentBaseName);

private slots:
  void onOkButton();
  void on_comboBox_currentIndexChanged(const QString &property);

private:
  Ui::FindDialog *ui;
  DataBasesMap *bases;
  QString currentBaseName = "";

signals:
  void getValue(QString property, QString value, int operation);
};

#endif // FINDDIALOG_H
