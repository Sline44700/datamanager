#include "opendialog.h"
#include "ui_opendialog.h"
#include <QMessageBox>
#include <QPushButton>

OpenDialog::OpenDialog(QDir* n_dataBasesDir, QWidget *parent) :
  dataBasesDir(n_dataBasesDir),
  QDialog(parent),
  ui(new Ui::OpenDialog)
{
  ui->setupUi(this);
  connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &OpenDialog::onOkButton);
}

OpenDialog::~OpenDialog() {
  delete ui;
}

void OpenDialog::refresh() {
  ui->comboBox->clear();
  checkDataBases();

  for (QString dbName : dataBasesDir->entryList()) {
    ui->comboBox->addItem(dbName.split('.').first());
  }
}

void OpenDialog::checkDataBases() {
  if (dataBasesDir->isEmpty()) {
    auto msg = QMessageBox::warning(this, "DataManager", "Базы данных не найдены",
        QMessageBox::Ok);
    this->hide();
  }
}

void OpenDialog::onOkButton() {
  getDataBase(ui->comboBox->currentText());
}
