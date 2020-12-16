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

void Database::rebuild(){//rebuilds the database with fresh tables
    QSqlQuery query1(db);
    cout << query1.exec("DROP TABLE IF EXISTS Users") << endl;
    cout << query1.exec("DROP TABLE IF EXISTS Posts") << endl;
    cout << query1.exec("DROP TABLE IF EXISTS Comments") << endl;
    QSqlQuery query(db);
    cout << query.exec("CREATE TABLE IF NOT EXISTS Users(USER_ID integer primary key, USERNAME varchar(30) not null, PASSWORD varchar(30) not null);") << endl;
    cout << query.exec("CREATE TABLE IF NOT EXISTS Posts(POST_ID integer primary key, USER_ID int not null, TITLE varchar(30) not null, BODY varchar(255) not null);") << endl;
    cout << query.exec("CREATE TABLE IF NOT EXISTS Comments(COMMENT_ID integer primary key, POST_ID int not null,TEXT varchar(255) not null);") << endl;

}

void Database::createUser(const char* username,const char* password){//creates a new user
    try{
    QSqlQuery insertUser(db);
    insertUser.prepare("INSERT INTO Users (USERNAME, PASSWORD) VALUES (:username, :password)");
    insertUser.bindValue(":username", username);
    insertUser.bindValue(":password",  password);
    insertUser.exec();
    } catch (const char* exStr) {
        cout << exStr << endl;
    }
}

int Database::validateCredentials(string username, string password){//inputs username/password, selects list of passwords from username, returns userId if match is found, else returns -1
    try{
    QSqlQuery login(db);
    login.prepare("SELECT PASSWORD, USER_ID FROM Users WHERE USERNAME = :username");
    login.bindValue(":username", username.c_str());
    cout << "validating username: " << username << login.exec() << endl;
    while(login.next()){
        string value =  login.value(0).toString().toStdString();
        cout <<"Selected Password: "<< value<< " Entered: " << password << endl;
        if(password == value){
            cout << "Password Matches, ID: " << login.value(1).toString().toStdString() << endl;
            return login.value(1).toInt();
        }
    }
    } catch (const char* exStr) {
        cout << exStr << endl;
    }
    return -1;
}

bool Database::createPost(int userId,const char* title, const char* body){//creates a post based on users id and content
    try{
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
    } catch (const char* exStr) {
        return false;
        cout << exStr << endl;
    }
}

bool Database::hasPosts(){//ensures the database has posts before pulling from database
    QSqlQuery getPosts;
    getPosts.prepare("SELECT POST_ID FROM Posts;");
    return getPosts.exec();//if it selects successfully, then it returns true
}

map<int, string> Database::getPosts(){//returns a map with the poster's id and the title (for home page)
    try{
    map<int, string> posts;
    QSqlQuery getPosts;
    cout << "Get Posts Sucess" << getPosts.exec("SELECT POST_ID, TITLE FROM Posts;") << endl;
    while(getPosts.next()){
        cout << getPosts.value(1).toString().toStdString();
        posts.insert(pair<int,string>(getPosts.value(0).toInt(), getPosts.value(1).toString().toStdString()));
    }
    return posts;
    } catch (const char* exStr) {
            cout << exStr << endl;
            map<int, string> error;
            error.insert(pair<int,string>(-1,"ERROR"));
            return error;
    }
}

map<int, string> Database::getUserPosts(int id){//gets all posts by one user
        try{
    map<int, string> posts;
    QSqlQuery getPosts;
    getPosts.prepare("SELECT POST_ID, TITLE FROM Posts WHERE USER_ID = :userId;");
    getPosts.bindValue(":userId", id);
    cout << "Get Posts Sucess" << getPosts.exec() << endl;
    while(getPosts.next()){
        cout << getPosts.value(1).toString().toStdString();
        posts.insert(pair<int,string>(getPosts.value(0).toInt(), getPosts.value(1).toString().toStdString()));
    }
    return posts;
    } catch (const char* exStr) {
            cout << exStr << endl;
            map<int, string> error;
            error.insert(pair<int,string>(-1,"ERROR"));
            return error;
    }
}

post Database::getPostById(int id){//gets a post by its postID
    try{
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
    } catch (const char* exStr) {
        post er = post(-1,-1,"error","error");
        return er;
        cout << exStr << endl;
    }
}

user Database::getUserByPostId(int id){//gets which user posted a post by the post's id
    try{
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
    string username = getUser.value(0).toString().toStdString();
    string password = getUser.value(1).toString().toStdString();
    user returnUser(userId, username, password);
    return returnUser;
    } catch (const char* exStr) {
        user er = user(-1, "ERROR", "ERROR");
        return er;
        cout << exStr << endl;
    }
}

vector<string> Database::getCommentsByPostId(int id){//gets a vector of comments that were on a post
    vector<string> comments;
    QSqlQuery getComments;
    getComments.prepare("SELECT TEXT FROM Comments WHERE POST_ID = :postId");
    getComments.bindValue(":postId",id);
    getComments.exec();
    while(getComments.next()){
        comments.push_back(getComments.value(0).toString().toStdString());
    }
    return comments;
}

string Database::getUsernameById(int id){//gets the usersname by thier username id
    QSqlQuery getUsername;
    getUsername.prepare("SELECT USERNAME FROM Users WHERE USER_ID = :userId");
    getUsername.bindValue(":userId",id);
    getUsername.exec();
    getUsername.next();
    return getUsername.value(0).toString().toStdString();
}

void Database::createComment(int postId,const char* text){//creates database comments.
    try{
    QSqlQuery insertComment(db);
    insertComment.prepare("INSERT INTO Comments(POST_ID, TEXT) VALUES (:postId, :text)");
    insertComment.bindValue(":postId", postId);
    insertComment.bindValue(":text",  text);
    cout<< "Post ID: " << to_string(postId) << " text: " << text<< endl;
    cout<< "Creating comment sucess: " << insertComment.exec() << endl;
    } catch (const char* exStr) {
        cout << exStr << endl;
    }
}
