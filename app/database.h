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
#include <map>
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
    bool createPost(int, const char*, const char*);
    bool hasPosts();
    map<int, string>getPosts();
    int validateCredentials(string, string);

};

#endif // DATABASE_H
