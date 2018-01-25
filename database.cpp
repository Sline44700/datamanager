#include "database.h"
#include "defines.h"
#include <QFile>
#include <QTextStream>

DataBase::DataBase(QString n_name, QString n_modelName) : modelName(n_modelName), name(n_name) {
  QFile file("models/" + modelName + MODEL_FORMAT);
  QString line;
  QStringList list;
  int type;

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    delete this;
    throw "Model doesn't exist: " + modelName;
  }
  QTextStream fin(&file);

  while (!fin.atEnd()) {
    line = fin.readLine();
    list = line.split(':');
    if (list.first() == "string")
      type = STRING;
    else if (list.first() == "number")
      type = NUMBER;
    modelTypes.insert(list.last(), type);
    model.push_back(list.last());
  }
  file.close();
}

void DataBase::setItem(int id, QString property, QString value) {
  if (id > data.getTop())
    throw "setItem error: id > size";

  if (getPropertyType(property) == STRING)
    data.getValuePtr(id)->stringValues.insert(property, value);
  else if (getPropertyType(property) == NUMBER)
    data.getValuePtr(id)->numberValues.insert(property, value.toDouble());
}

QString DataBase::getItem(int id, QString property) {
  if (getPropertyType(property) == STRING)
    return data[id].stringValues[property];
  else if (getPropertyType(property) == NUMBER)
    return QString::number(data[id].numberValues[property]);
}

int DataBase::addField() {
  int id = data.getTop();
  DataBaseRow *field = new DataBaseRow();

  for (QString property : modelTypes.keys()) {
    if (getPropertyType(property) == STRING)
      field->stringValues.insert(property, "");
    else if (getPropertyType(property) == NUMBER)
      field->numberValues.insert(property, 0);
  }
  data.push_back(*field);
  delete field;

  return id;
}

SuperVector <int> DataBase::findIdsByProperty(QString property, QString value, int operation) {
  SuperVector <int> foundIds;

  if (getPropertyType(property) == STRING) {
    for (int i = 0; i < data.getTop(); i++) {
      switch (operation) {
        case STR_CONTAINS: {
          if (data[i].stringValues[property].contains(value))
            foundIds.push_back(i);
          break;
        }
        case STR_EQUAL: {
          if (data[i].stringValues[property] == value)
            foundIds.push_back(i);
          break;
        }
      }
    }
  }
  else if (getPropertyType(property) == NUMBER) {
    for (int i = 0; i < data.getTop(); i++) {
      switch (operation) {
        case EQUAL: {
            if (data[i].numberValues[property] == value.toDouble())
              foundIds.push_back(i);
            break;
        }
        case MORE: {
            if (data[i].numberValues[property] > value.toDouble())
              foundIds.push_back(i);
            break;
        }
        case MORE_OR_EQUAL: {
            if (data[i].numberValues[property] >= value.toDouble())
              foundIds.push_back(i);
            break;
        }
        case LESS: {
            if (data[i].numberValues[property] < value.toDouble())
              foundIds.push_back(i);
            break;
        }
        case LESS_OR_EQUAL: {
            if (data[i].numberValues[property] <= value.toDouble())
              foundIds.push_back(i);
            break;
        }
        case NOT_EQUAL: {
            if (data[i].numberValues[property] != value.toDouble())
              foundIds.push_back(i);
            break;
        }
      }

    }
  }

  return foundIds;
}

int DataBase::getIdByProperty(QString property) {
  for (int id = 0; id < model.size(); id++) {
    if (model[id] == property)
      return id;
  }

  throw "Can't find id by property: " + property;
}

void DataBase::save(QString fileName) {
  QFile file("databases/" + fileName + DB_FORMAT);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw "Can't save database: " + fileName;
  }
  QTextStream fout(&file);
  QString property;

  fout << modelName << endl;
  for (int i = 0; i < data.getTop(); i++) {
    for (int propertyNum = 0; propertyNum < model.size(); propertyNum++) {
      property = model[propertyNum];
      if (propertyNum < getModelSize() - 1) {
        if (getPropertyType(property) == STRING)
          fout << data[i].stringValues[property] << ",";
        else if (getPropertyType(property) == NUMBER)
          fout << data[i].numberValues[property] << ",";
      }
      else
        if (getPropertyType(property) == STRING)
          fout << data[i].stringValues[property] << endl;
        else if (getPropertyType(property) == NUMBER)
          fout << data[i].numberValues[property] << endl;
    }
  }
  file.close();
}

void DataBase::load(QString fileName) {
  QFile file("databases/" + fileName + DB_FORMAT);
  QStringList list;

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw "Database doesn't exist: " + fileName;
  }
  QTextStream fin(&file);

  QString line = fin.readLine();
  if (line != modelName)
    throw "Database of wrong model loaded ('" + line + "' loaded; '" + modelName + "' expected)";

  while (!fin.atEnd()) {
    DataBaseRow* field = new DataBaseRow;
    line = fin.readLine();
    list = line.split(',');
    for (int i = 0; i < model.size(); i++) {
      if (i >= list.size()) continue;
      if (getPropertyType(model[i]) == STRING)
        field->stringValues.insert(model[i], list[i]);
      else if (getPropertyType(model[i]) == NUMBER)
        field->numberValues.insert(model[i], list[i].toDouble());
    }
    data.push_back(*field);
  }
  file.close();
}
