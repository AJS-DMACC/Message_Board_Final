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
#include "post.h"
#include "user.h"
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
    post getPostById(int);
    user getUserByPostId(int);
    int validateCredentials(string, string);
    void createComment(int ,const char*);
    string* getCommentsByPostId(int, size_t&);
};

#endif // DATABASE_H
