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
    QSqlQuery query;
    query.exec("create table if not exists users(USER_ID int NOT NULL AUTO_INCREMENT, USERNAME varchar(30) not null, PASSWORD varchar(30) not null, primary key (USER_ID));");
}

void Database::createUser(const char* username,const char* password){
    QSqlQuery insertUser;
    insertUser.prepare("INSERT INTO users (USERNAME, PASSWORD) VALUES (:username, :password)");
    insertUser.bindValue(":username", username);
    insertUser.bindValue(":password",  password);
    insertUser.exec();
}
