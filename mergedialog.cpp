#include <QMessageBox>
#include "mergedialog.h"
#include "ui_mergedialog.h"

MergeDialog::MergeDialog(DataBasesMap *n_bases, QWidget *parent) :
  bases(n_bases),
  QDialog(parent),
  ui(new Ui::MergeDialog)
{
  ui->setupUi(this);
  connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &MergeDialog::onOkButton);
}

MergeDialog::~MergeDialog() {
  delete ui;
}

void MergeDialog::refresh(QString n_currentBaseName) {
  currentBaseName = n_currentBaseName;
  ui->comboBox->blockSignals(true);
  ui->comboBox->clear();
  ui->comboBox->blockSignals(false);
  ui->tableWidget->clear();
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(0);

  DataBase* currentDb = &bases->find(currentBaseName).value();
  for (QString baseName : bases->keys()) {
    DataBase* db = &bases->find(baseName).value();
    if (db->getModelName() == currentDb->getModelName() && baseName != currentBaseName)
      ui->comboBox->addItem(baseName);
  }
  if (ui->comboBox->count() == 0) {
    auto msg = QMessageBox::warning(this, "DataManager", "Открыта только одна база модели '" +
                                    currentDb->getModelName() + "'", QMessageBox::Ok);
    this->hide();
  }
  loadDataBase(currentBaseName, ui->tableWidget_2, true);
}

void MergeDialog::loadDataBase(const QString &dbName, QTableWidget* tableWidget, bool loadModelOnly) {
  QStringList titles;
  DataBase *db = &bases->find(dbName).value();
  int propertyNum;

  for (QString property : db->getModelNames())
    titles << property;
  tableWidget->clear();
  tableWidget->setRowCount(0);
  tableWidget->setColumnCount(db->getModelSize());
  tableWidget->setHorizontalHeaderLabels(titles);

  if (loadModelOnly) return;
  for (int i = 0; i < db->getFieldsNumber(); i++) {
    tableWidget->insertRow(tableWidget->rowCount());
    propertyNum = 0;
    for (QString property : db->getModelNames()) {
      tableWidget->setItem(tableWidget->rowCount() - 1, propertyNum,
                               new QTableWidgetItem(db->getItem(i, property)));
      propertyNum++;
    }
  }
}

void MergeDialog::on_comboBox_currentIndexChanged(const QString &dbName) {
  loadDataBase(dbName, ui->tableWidget);
}

void MergeDialog::moveItem(QTableWidget* tableWidget, QTableWidget* tableWidget_2, int currentRow) {
  tableWidget_2->insertRow(tableWidget_2->rowCount());
  for (int column = 0; column < tableWidget->columnCount(); column++) {
    tableWidget_2->setItem(tableWidget_2->rowCount() - 1, column,
                               new QTableWidgetItem(*tableWidget->item(currentRow, column)));
  }
  tableWidget->removeRow(currentRow);
}

void MergeDialog::on_right_clicked() {
  if (ui->tableWidget->currentRow() < 0) {
    auto msg = QMessageBox::warning(this, "DataManager", "Для перемещения выберете ячейку", QMessageBox::Ok);
    return;
  }
  moveItem(ui->tableWidget, ui->tableWidget_2, ui->tableWidget->currentRow());
}

void MergeDialog::on_left_clicked() {
  if (ui->tableWidget_2->currentRow() < 0) {
    auto msg = QMessageBox::warning(this, "DataManager", "Для перемещения выберете ячейку", QMessageBox::Ok);
    return;
  }
  moveItem(ui->tableWidget_2, ui->tableWidget, ui->tableWidget_2->currentRow());
}

void MergeDialog::on_rightFull_clicked() {
  int rowCount = ui->tableWidget->rowCount();
  for (int row = 0; row < rowCount; row++)
    moveItem(ui->tableWidget, ui->tableWidget_2, 0);
}

void MergeDialog::on_leftFull_clicked() {
  int rowCount = ui->tableWidget_2->rowCount();
  for (int row = 0; row < rowCount; row++)
    moveItem(ui->tableWidget_2, ui->tableWidget, 0);
}

void MergeDialog::onOkButton() {
  DataBase *db = &bases->find(currentBaseName).value();
  int rowCount = ui->tableWidget_2->rowCount();
  int columnCount = ui->tableWidget_2->columnCount();

  for (int row = 0; row < rowCount; row++) {
    db->addField();
    for (int column = 0; column < columnCount; column++) {
      db->setItem(db->getFieldsNumber() - 1, db->getPropertyById(column),
                  ui->tableWidget_2->item(row, column)->text());
    }
  }

  getMergeSignal();
}
