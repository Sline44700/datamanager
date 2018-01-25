#include "newdialog.h"
#include "ui_newdialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QDir>

NewDialog::NewDialog(DataBasesMap *n_bases, QDir* n_modelsDir, QDir* n_dataBasesDir, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewDialog),
  bases(n_bases),
  modelsDir(n_modelsDir),
  dataBasesDir(n_dataBasesDir)
{
  ui->setupUi(this);
  connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &NewDialog::onOkButton);
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

NewDialog::~NewDialog() {
  delete ui;
}

void NewDialog::refresh() {
  ui->lineEdit->setText("");
  ui->label_2->setText("");
  ui->comboBox->clear();
  checkModels();

  for (QString modelName : modelsDir->entryList()) {
    ui->comboBox->addItem(modelName.split('.').first());
  }
}

void NewDialog::checkModels() {
  if (modelsDir->isEmpty()) {
    auto msg = QMessageBox::warning(this, "DataManager",
                                     "Модели не найдены; создайте хотя бы одну модель в редакторе моделей",
        QMessageBox::Ok);
    this->hide();
  }
}

void NewDialog::canBeDone(QPushButton* Ok) {
  if (!Ok->isEnabled()) {
    Ok->setEnabled(true);
    ui->label_2->setText("");
  }
}

void NewDialog::on_lineEdit_textChanged(const QString &dbName) {
  QPushButton* Ok = ui->buttonBox->button(QDialogButtonBox::Ok);

  if (bases->contains(dbName) || dataBasesDir->entryList().contains(dbName + DB_FORMAT)) {
    Ok->setEnabled(false);
    ui->label_2->setText("Такая база данных уже существует");
    return;
  }
  else
    canBeDone(Ok);

  if (dbName.contains(' ')) {
    Ok->setEnabled(false);
    ui->label_2->setText("Название не должно содержать пробелов");
    return;
  }
  else
    canBeDone(Ok);

  if (dbName.size() == 0) {
    Ok->setEnabled(false);
    ui->label_2->setText("Название должно содержать хотя бы один символ");
    return;
  }
  else
    canBeDone(Ok);

  // проверка на латинские буквы

}

void NewDialog::onOkButton() {
  QString dbName = ui->lineEdit->text();

  if (dbName.size() == 0) {
    return;
  }

  getNameAndModel(dbName, ui->comboBox->currentText());
}
