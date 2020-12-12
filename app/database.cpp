#include "database.h"

Database::Database()
{
    DRIVER = QString("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER)) {
        qWarning() << "ERROR: driver not available";
    }
}

void Database::open() {
    db = QSqlDatabase::addDatabase(DRIVER);
    db.setDatabaseName("C:\\Users\\aserck21.HS-LAB1SERCK\\Desktop\\forum.sqlite");
    if(!db.open()) {
        qWarning() << "ERROR: " << db.lastError();
    }
}

void Database::close() {
    db.commit();
    db.close();
    QSqlDatabase::removeDatabase(DRIVER);
}

void Database::rebuild(){
    QSqlQuery query1(db);
    cout << query1.exec("DROP TABLE IF EXISTS Users") << endl;
    cout << query1.exec("DROP TABLE IF EXISTS Posts") << endl;
    cout << query1.exec("DROP TABLE IF EXISTS Comments") << endl;
    QSqlQuery query(db);
    cout << query.exec("CREATE TABLE IF NOT EXISTS Users(USER_ID integer primary key, USERNAME varchar(30) not null, PASSWORD varchar(30) not null);") << endl;
    cout << query.exec("CREATE TABLE IF NOT EXISTS Posts(POST_ID integer primary key, USER_ID int not null, TITLE varchar(30) not null, BODY varchar(255) not null);") << endl;
    cout << query.exec("CREATE TABLE IF NOT EXISTS Comments(COMMENT_ID integer primary key, POST_ID int not null,TEXT varchar(255) not null);") << endl;
}

void Database::createUser(const char* username,const char* password){

      QSqlQuery insertUser(db);
    insertUser.prepare("INSERT INTO Users (USERNAME, PASSWORD) VALUES (:username, :password)");
    insertUser.bindValue(":username", username);
    insertUser.bindValue(":password",  password);
    insertUser.exec();

    /*
    cout<<"Creating User and Query"<<endl;
   QSqlQuery get(db);
   cout << get.exec("SELECT USERNAME, PASSWORD, USER_ID FROM users;") << endl;
    while (get.next()) {
          // QString country = get.value(0).toString();
           cout << "Person:" <<get.value(0).toString().toStdString()<< endl;
           cout << "pass:" <<get.value(1).toString().toStdString()<< endl;
           cout << "userid:" <<get.value(2).toInt()<< endl;
    }*/
}

int Database::validateCredentials(string username, string password){
    QSqlQuery login(db);
    login.prepare("SELECT PASSWORD, USER_ID FROM Users WHERE USERNAME = :username");
    login.bindValue(":username", username.c_str());
    cout << "validating username: " << username << login.exec() << endl;
    while(login.next()){
        cout << login.value(0).toString().toStdString()<< endl;
        if(password.compare(login.value(0).toString().toStdString())){
            cout << "Password Matches, ID: " << login.value(1).toString().toStdString() << endl;
            return login.value(1).toInt();
        }
    }
    return -1;
}

bool Database::createPost(int userId,const char* title, const char* body){
    QSqlQuery insertPost(db);
    //cout<< "TITLE: " << title<< endl;
    insertPost.prepare("INSERT INTO Posts (USER_ID, TITLE, BODY) VALUES (:userId, :title, :body)");
    insertPost.bindValue(":userId", userId);
    insertPost.bindValue(":title",  title);
    insertPost.bindValue(":body",  body);
    cout<< "Post Sucess" << insertPost.exec();


    QSqlQuery get(db);
    cout << get.exec("SELECT  title, POST_ID FROM Posts;") << endl;
     while (get.next()) {
           // QString country = get.value(0).toString();
            cout << "id:" <<get.value(0).toString().toStdString()<< endl;
            cout << "title:" <<get.value(1).toString().toStdString()<< endl;
     }
    return true;
}

bool Database::hasPosts(){
    QSqlQuery getPosts;
    getPosts.prepare("SELECT POST_ID FROM Posts;");
    return getPosts.exec();//if it selects successfully, then it returns true
}

map<int, string> Database::getPosts(){
    map<int, string> posts;
    QSqlQuery getPosts;
    cout << "Get Posts Sucess" << getPosts.exec("SELECT POST_ID, TITLE FROM Posts;") << endl;
    while(getPosts.next()){
        cout << getPosts.value(1).toString().toStdString();
        posts.insert(pair<int,string>(getPosts.value(0).toInt(), getPosts.value(1).toString().toStdString()));
    }
    return posts;
}

post Database::getPostById(int id){
    QSqlQuery getPost;
    getPost.prepare("SELECT USER_ID, TITLE, BODY FROM Posts WHERE POST_ID = :postId");
    getPost.bindValue(":postId",id);
    getPost.exec();
    getPost.next();
    long userId = getPost.value(0).toInt();
    string title = getPost.value(1).toString().toStdString();
    string body = getPost.value(2).toString().toStdString();
    post returnPost(id, userId, title, body);
    return returnPost;
}

user Database::getUserByPostId(int id){
    QSqlQuery getPost;
    getPost.prepare("SELECT USER_ID FROM Posts WHERE POST_ID = :postId");
    getPost.bindValue(":postId",id);
    getPost.exec();
    getPost.next();
    int userId = getPost.value(0).toInt();
    QSqlQuery getUser;
    getUser.prepare("SELECT USERNAME, PASSWORD FROM Users WHERE USER_ID = :userId");
    getUser.bindValue(":userId",userId);
    getUser.exec();
    getUser.next();
    string username = getPost.value(0).toString().toStdString();
    string password = getPost.value(1).toString().toStdString();
    user returnUser(userId, username, password);
    return returnUser;
}

string[] Database::getCommentsByPostId(int id){
    QSqlQuery getPost;
    getPost.prepare("SELECT USER_ID FROM Posts WHERE POST_ID = :postId");
    getPost.bindValue(":postId",id);
    getPost.exec();
    getPost.next();
    int userId = getPost.value(0).toInt();
    QSqlQuery getUser;
    getUser.prepare("SELECT USERNAME, PASSWORD FROM Users WHERE USER_ID = :userId");
    getUser.bindValue(":userId",userId);
    getUser.exec();
    getUser.next();
    string username = getPost.value(0).toString().toStdString();
    string password = getPost.value(1).toString().toStdString();
    user returnUser(userId, username, password);
    return returnUser;
}

void Database::createPost(int postId,const char* text){
    QSqlQuery insertComment(db);
    insertComment.prepare("INSERT INTO Comments (POST_ID, TEXT) VALUES (:postId, :text)");
    insertComment.bindValue(":postId", postId);
    insertComment.bindValue(":text",  text);
    insertComment.exec();
}
