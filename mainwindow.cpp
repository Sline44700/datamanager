#include <QMessageBox>
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "supervector.h"

// Формы:
//// 1) + На создание (название базы данных, если такая существует, то блокирует кнопку ОК и пишет ошибку внизу красным)
//// 2) + На открытие (-||-)
//// 3) + На слияние (2 таблицы и стрелочки << >> < >)
//// 4) + Поиск (комбобокс key и текстбокс value, возможно добавление новых, не более числа элементов модуля)
// extra:
// 1) Менеджер моделей (две таблицы вертикально, наверху модели, внизу состовляющие выделенного)

// TODO:
//// 1) Поиск +
//// 2) Вектор +
//// 3) База данных +/-
//// 3.1) Перейти на QString +
//// 4) Слияние !!!
// 4.1) Менеджер моделей !
// 4.2) Сделать звёздочку либо избавиться от saved
//// 4.3) Сделать спец поля для чисел в поиске !
//// 4.4) Перемещение записей
// 4.5) Помощь и about !!!
// 4.6) Сохранить как
// 4.7) Закрытие баз данных !
// 4.8) Удаление баз данных !
//// 4.9) Вкладка "Опции" (сделать размер по содержимому, мёрдж и остальные кнопки снизу) !
// 4.10) Написать в помощи о том, что можно использовать tab, чтобы перейти к следующему полю (например, в поиске)
// 4.11) Сделать большое кол-во тестовых схем !!!
// 4.12) Статус-бар !!!
// ПРИБРАТЬСЯ В КОДЕ
//// 1) В new не базу, а только то, что нужно, отдавать (имена) ???
//// 2) Написать деструктор !!!
//// 3) Сделать удаление и создание форм при нажатии на кнопку
//// 4) Проверить file.close

// Баги:
//// 1) Удаление последней записи приводит к вылету
//// 2) После поиска значения остаются
//// 3) Удаление записей при поиске приводит к вылету
//// 4) Если не находит, то не высвечивает ошибку
//// 5) Возможно открывать одну и ту же форму много раз
//// 6) Сохранение чисел не работает
// 7) При создании новой базы она не обновляется в списке
//// 8) Ломаются даблы, имеются траблы
//// 9) Проверка на то, есть ли база данных, указанная в .dbs

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->newAction, &QAction::triggered, this, &MainWindow::newAction);
  connect(ui->openAction, &QAction::triggered, this, &MainWindow::openAction);
  connect(ui->saveAction, &QAction::triggered, this, &MainWindow::saveAction);
  connect(ui->saveAsAction, &QAction::triggered, this, &MainWindow::saveAsAction);
  connect(ui->modelManagerAction, &QAction::triggered, this, &MainWindow::modelManagerAction);
  connect(ui->quitAction, &QAction::triggered, this, &MainWindow::quitAction);
  connect(ui->createEntryAction, &QAction::triggered, this, &MainWindow::on_create_clicked);
  connect(ui->removeEntryAction, &QAction::triggered, this, &MainWindow::on_remove_clicked);
  connect(ui->findAction, &QAction::triggered, this, &MainWindow::on_find_clicked);
  connect(ui->exportDataAction, &QAction::triggered, this, &MainWindow::on_merge_clicked);
  connect(ui->upAction, &QAction::triggered, this, &MainWindow::swapUpAction);
  connect(ui->downAction, &QAction::triggered, this, &MainWindow::swapDownAction);
  connect(ui->minimizeColumnSizeAction, &QAction::triggered, this, &MainWindow::minimizeColumnSize);
  connect(ui->helpAction, &QAction::triggered, this, &MainWindow::helpAction);
  connect(ui->aboutAction, &QAction::triggered, this, &MainWindow::aboutAction);

  modelsDir = setFilesDir("models", QString::fromStdString("*") + QString::fromStdString(MODEL_FORMAT));
  dataBasesDir = setFilesDir("databases", QString::fromStdString("*") + QString::fromStdString(DB_FORMAT));

  newDialog = new NewDialog(bases, modelsDir, dataBasesDir);
  openDialog = new OpenDialog(dataBasesDir);
  findDialog = new FindDialog(bases);
  mergeDialog = new MergeDialog(bases);
  aboutDialog = new AboutDialog;
  connect(newDialog, SIGNAL(getNameAndModel(QString, QString)), this, SLOT(addDataBase(QString, QString)));
  connect(openDialog, SIGNAL(getDataBase(QString)), this, SLOT(openDataBase(QString)));
  connect(findDialog, SIGNAL(getValue(QString, QString, int)), this, SLOT(find(QString, QString, int)));
  connect(mergeDialog, SIGNAL(getMergeSignal()), this, SLOT(refresh()));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::setButtonsEnabled(bool enabled, bool exceptFind) {
  ui->create->setEnabled(enabled);
  ui->remove->setEnabled(enabled);
  if (!exceptFind) ui->find->setEnabled(enabled);
  ui->merge->setEnabled(enabled);
  ui->saveAction->setEnabled(enabled);
}

void MainWindow::loadDataBase(QString dbName) {
  QStringList titles;
  DataBase *db = &bases->find(dbName).value();
  int propertyNum;

  codeChangesTable = true;
  findingMode = false; setButtonsEnabled(true);
  for (QString property : db->getModelNames())
    titles << property;
  ui->tableWidget->clear();
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(db->getModelSize());
  ui->tableWidget->setHorizontalHeaderLabels(titles);

  for (int i = 0; i < db->getFieldsNumber(); i++) {
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    propertyNum = 0;
    for (QString property : db->getModelNames()) {
      ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, propertyNum,
                               new QTableWidgetItem(db->getItem(i, property)));
      propertyNum++;
    }
  }

  minimizeColumnSize();
  currentBaseName = dbName;
  codeChangesTable = false;
}

void MainWindow::addDataBaseToComboBox(QString dbName) {
  ui->comboBox->addItem(dbName);
  ui->comboBox->setCurrentIndex(comboboxItemsNum);
  comboboxItemsNum++;
  setButtonsEnabled(true);
}

void MainWindow::addDataBase(QString dbName, QString modelName, bool loadFromFile) {
  try {
    DataBase* db = new DataBase(dbName, modelName);

    if (loadFromFile) {
      db->load(dbName);
      db->setSaved(true);
    }
    else
      db->addField();

    bases->insert(dbName, *db);
    addDataBaseToComboBox(dbName);
    statusBar()->showMessage("Создана база данных '" + dbName + "'");
  }
  catch (QString str) {
    auto msg = QMessageBox::critical(this, "DataManager", str, QMessageBox::Ok);
  }
}

void MainWindow::openDataBase(QString dbName) {
  QFile file("databases/" + dbName + DB_FORMAT);
  if (bases->contains(dbName)) {
    auto msg = QMessageBox::warning(this, "DataManager", "Данная база данных уже открыта", QMessageBox::Ok);
    return;
  }
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    auto msg = QMessageBox::warning(this, "DataManager", "База данных не найдена: " + dbName, QMessageBox::Ok);
    return;
  }
  QString modelName;
  QTextStream fin(&file);
  fin >> modelName;
  file.close();
  QFile modelFile("models/" + modelName + MODEL_FORMAT);
  if (!modelFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    auto msg = QMessageBox::warning(this, "DataManager", "Модель базы данных не найдена: " + modelName, QMessageBox::Ok);
    return;
  }
  addDataBase(dbName, modelName, true);
  statusBar()->showMessage("Загружена база данных '" + dbName + "'");
  modelFile.close();
}

void MainWindow::find(QString property, QString value, int operation) {
  DataBase *db = &bases->find(currentBaseName).value();
  auto found = db->findIdsByProperty(property, value, operation);
  int propertyNum;

  if (value == "") {
    if (!findingMode) {
      auto msg = QMessageBox::warning(this, "DataManager", "Вы оставили пустое поле для поиска",
                 QMessageBox::Ok);
      return;
    }
    else {
      findingMode = false; setButtonsEnabled(true);
      loadDataBase(currentBaseName);
      return;
    }
  }

  findingMode = true; setButtonsEnabled(false, true);
  codeChangesTable = true;
  ui->tableWidget->setRowCount(0);
  for (int i = 0; i < found.getTop(); i++) {
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    propertyNum = 0;
    for (QString property : db->getModelNames()) {
      ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, propertyNum++,
                               new QTableWidgetItem(db->getItem(found[i], property)));
    }
  }
  codeChangesTable = false;

  statusBar()->showMessage("Результаты поиска выведены в таблицу");
}

void MainWindow::refresh() {
  loadDataBase(currentBaseName);
  statusBar()->showMessage("Записи были импортированы из других баз данных");
}

void MainWindow::newAction() {
  newDialog->show();
  newDialog->refresh();
}

void MainWindow::openAction() {
  openDialog->show();
  openDialog->refresh();
}

void MainWindow::saveAction() {
  DataBase *db = &bases->find(currentBaseName).value();

  db->save(currentBaseName);
  db->setSaved(true);
}

void MainWindow::saveAsAction() {
  //
}

void MainWindow::modelManagerAction() {
  auto msg = QMessageBox::information(this, "DataManager", "Данная функция на разработке",
      QMessageBox::Ok);
}

void MainWindow::quitAction() {
  newDialog->close();
  openDialog->close();
  findDialog->close();
  close();
}

void MainWindow::minimizeColumnSize() {
  if (ui->minimizeColumnSizeAction->isChecked())
    ui->tableWidget->resizeColumnsToContents();
  else
    for (int column = 0; column <= ui->tableWidget->columnCount(); column++)
     ui->tableWidget->setColumnWidth(column, 100);
}

void MainWindow::swapUpAction() {
  DataBase *db = &bases->find(currentBaseName).value();
  int currentRow = ui->tableWidget->currentRow();
  QTableWidgetItem* item;

  if (currentRow <= 0) {
    auto msg = QMessageBox::warning(this, "DataManager", "Сдвиг вверх невозможен", QMessageBox::Ok);
    return;
  }

  db->swap(currentRow, currentRow - 1);

  for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
    item = ui->tableWidget->takeItem(currentRow - 1, column);
    ui->tableWidget->setItem(currentRow - 1, column, ui->tableWidget->takeItem(currentRow, column));
    ui->tableWidget->setItem(currentRow, column, item);
  }
}

void MainWindow::swapDownAction() {
  DataBase *db = &bases->find(currentBaseName).value();
  int currentRow = ui->tableWidget->currentRow();
  QTableWidgetItem* item;

  if (currentRow >= ui->tableWidget->rowCount() - 1) {
    auto msg = QMessageBox::warning(this, "DataManager", "Сдвиг вниз невозможен", QMessageBox::Ok);
    return;
  }

  db->swap(currentRow, currentRow + 1);

  for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
    item = ui->tableWidget->takeItem(currentRow + 1, column);
    ui->tableWidget->setItem(currentRow + 1, column, ui->tableWidget->takeItem(currentRow, column));
    ui->tableWidget->setItem(currentRow, column, item);
  }
}

void MainWindow::helpAction() {
  //
}

void MainWindow::aboutAction() {
  aboutDialog->show();
}

QDir* MainWindow::setFilesDir(QString dirName, QString extension) {
  QDir* dir = new QDir;
  QStringList filters;

  *dir = QDir::current();
  if (!dir->cd(dirName)) {
    dir->mkdir(dirName);
    dir->cd(dirName);
  }
  filters << extension;
  dir->setNameFilters(filters);

  return dir;
}

void MainWindow::on_create_clicked() {
  if (currentBaseName == "") {
    auto msg = QMessageBox::warning(this, "DataManager", "Нет открытой базы данных",
        QMessageBox::Ok);
    return;
  }

  DataBase *db = &bases->find(currentBaseName).value();

  db->addField();
  ui->tableWidget->insertRow(ui->tableWidget->rowCount());
  ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(""));
  for (QString property : db->getModelNames()) {
    if (db->getPropertyType(property) == NUMBER) {
      try {
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, db->getIdByProperty(property),
                                 new QTableWidgetItem("0"));
      }
      catch (QString err) {
        auto msg = QMessageBox::information(this, "DataManager", err, QMessageBox::Ok);
      }
    }
  }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &dbName) {
  loadDataBase(dbName);
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) {
  if (codeChangesTable) return; // если код изменяет таблицу, то игнорируем это изменение

  int column = item->column();
  int row = item->row();
  DataBase *db = &bases->find(currentBaseName).value();
  QString model = db->getModelNames()[column];
  QString text = item->text();

  if (db->getPropertyType(model) == NUMBER && text.toDouble() == 0 && text != "0") {
    auto msg = QMessageBox::warning(this, "DataManager", "Данное свойство должно быть числом", QMessageBox::Ok);
    text = "0";
    item->setText(text);
  }

  db->setItem(row, model, text);
  db->setSaved(false);
  minimizeColumnSize();
}

void MainWindow::on_remove_clicked() {
  DataBase *db = &bases->find(currentBaseName).value();
  if (ui->tableWidget->currentRow() < 0) {
    auto msg = QMessageBox::warning(this, "DataManager", "Для удаления выберете ячейку", QMessageBox::Ok);
    return;
  }

  if (findingMode) loadDataBase(currentBaseName);
  db->removeField(ui->tableWidget->currentRow());
  db->setSaved(false);

  if (ui->tableWidget->currentRow() == 0 && ui->tableWidget->rowCount() <= 1) {
    on_create_clicked();
    on_remove_clicked();
    return;
  }
  ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::on_find_clicked() {
  findDialog->show();
  findDialog->refresh(currentBaseName);
}

void MainWindow::on_merge_clicked() {
  if (currentBaseName == "") {
    auto msg = QMessageBox::warning(this, "DataManager", "Нет текущей открытой базы данных", QMessageBox::Ok);
    return;
  }
  mergeDialog->show();
  mergeDialog->refresh(currentBaseName);
}
