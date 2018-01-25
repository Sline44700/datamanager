#ifndef DEFINES_H
#define DEFINES_H
#include <QMap>
#include "database.h"

// db types
#define STRING 0
#define NUMBER 1

// string search
#define STR_CONTAINS 0
#define STR_EQUAL 1

// number search
#define EQUAL 0
#define MORE 1
#define MORE_OR_EQUAL 2
#define LESS 3
#define LESS_OR_EQUAL 4
#define NOT_EQUAL 5

// main
#define PROGRAM_NAME "DataManager"
#define MODEL_FORMAT ".md"
#define DB_FORMAT ".dbs"

typedef QMap <QString, DataBase> DataBasesMap;

#endif // DEFINES_H
