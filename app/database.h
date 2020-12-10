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
    void rebuild();
    void close();
    void createUser(const char*,const char*);

};

#endif // DATABASE_H
