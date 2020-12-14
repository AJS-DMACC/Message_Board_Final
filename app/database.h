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
#include <vector>
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
    map<int, string>getUserPosts(int);
    post getPostById(int);
    user getUserByPostId(int);
    int validateCredentials(string, string);
    void createComment(int ,const char*);
    vector<string> getCommentsByPostId(int);
    string getUsernameById(int);
};

#endif // DATABASE_H
