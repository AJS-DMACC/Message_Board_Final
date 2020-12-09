#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <memory>
using namespace std;

class Database {
private:
    QSqlDatabase db;
    long getLatestRowId();
    QString DRIVER;
public:
    Database();
    void open();
    void rebuildTestDatabase();
    void close();

};

#endif // DATABASE_H
