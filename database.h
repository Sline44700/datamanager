#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <QMap>
#include <QString>
#include <QVector>
#include "supervector.h"

#define STRING 0
#define NUMBER 1

using namespace std;

struct DataBaseRow {
  QMap <QString, QString> stringValues;
  QMap <QString, double> numberValues;
};

class DataBase {
private:
  SuperVector <DataBaseRow> data;
  QMap <QString, int> modelTypes;
  QVector <QString> model;
  QString modelName;
  QString name; // not used
  bool saved = false; // not used

public:
  DataBase(QString n_name, QString n_modelPath);
  int addField(); // returns id; creates empty field
  inline void removeField(int id) { data.remove(id); }
  SuperVector <int> findIdsByProperty(QString property, QString value, int operation);
  void save(QString fileName);
  void load(QString fileName);

  // getters/setters
  void setItem(int id, QString property, QString value);
  QString getItem(int id, QString property);
//  inline QString getStringItem(int id, QString property) { return data[id].stringValues[property]; }
//  inline double getNumberItem(int id, QString property) { return data[id].numberValues[property]; }
  inline QString getModelName() { return modelName; }
  inline int getFieldsNumber() const { return data.getTop(); }
  inline int getModelSize() const { return model.size(); }
  inline auto getModelNames() const { return model; }
  inline int getPropertyType(QString property) const { return modelTypes[property]; }
  int getIdByProperty(QString property);
  inline QString getPropertyById(int id) { return model[id]; }
  inline void swap(const int& pos1, const int& pos2) { data.swap(pos1, pos2); }
//  inline QString getName() { return name; }
  inline void setSaved(bool value) { saved = value; }
  inline bool getSaved() { return saved; }
};

#endif // DATABASE_H
