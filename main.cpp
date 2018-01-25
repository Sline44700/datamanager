#include "mainwindow.h"
#include <QApplication>
#include <QStatusBar>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;

//  w.statusBar()->showMessage("  © Святослав Царев ИКПИ-63");
//  w.statusBar()->setFont(QFont("Times New Roman", 10));
  w.show();

  return a.exec();
}
