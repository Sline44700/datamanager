#include "finddialog.h"
#include "ui_finddialog.h"
#include <QMessageBox>
#include <QPushButton>
#include "database.h"

FindDialog::FindDialog(DataBasesMap *n_bases, QWidget *parent) :
  bases(n_bases),
  QDialog(parent),
  ui(new Ui::FindDialog)
{
  ui->setupUi(this);
  connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &FindDialog::onOkButton);
}

FindDialog::~FindDialog() {
  delete ui;
}

void FindDialog::refresh(QString n_currentBaseName) {
  ui->lineEdit->setText("");
  if (n_currentBaseName == "") {
    auto msg = QMessageBox::warning(this, "DataManager", "База данных не загружена",
        QMessageBox::Ok);
    this->hide();
    return;
  }
  currentBaseName = n_currentBaseName;
  DataBase *db = &bases->find(currentBaseName).value();

  ui->comboBox->clear();
  for (QString property : db->getModelNames()) {
    ui->comboBox->addItem(property);
  }
}

void FindDialog::onOkButton() {
  getValue(ui->comboBox->currentText(), ui->lineEdit->text(), ui->comboBox_2->currentIndex());
}

void FindDialog::on_comboBox_currentIndexChanged(const QString &property) {
  DataBase *db = &bases->find(currentBaseName).value();
  if (db->getPropertyType(property) == STRING) {
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("содержит");
    ui->comboBox_2->addItem("равно");
  }
  else if (db->getPropertyType(property) == NUMBER) {
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("==");
    ui->comboBox_2->addItem(">");
    ui->comboBox_2->addItem(">=");
    ui->comboBox_2->addItem("<");
    ui->comboBox_2->addItem("<=");
    ui->comboBox_2->addItem("!=");
  }
}
