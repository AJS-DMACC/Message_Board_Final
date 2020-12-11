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
    QSqlQuery query(db);
    cout << query.exec("CREATE TABLE IF NOT EXISTS Users(USER_ID int auto_increment primary key, USERNAME varchar(30) not null, PASSWORD varchar(30) not null);") << endl;//query.exec("CREATE TABLE IF NOT EXISTS Users(USER_ID int NOT NULL AUTO_INCREMENT, USERNAME varchar(30) not null, PASSWORD varchar(30) not null, primary key (USER_ID))") << endl;
    /*QSqlQuery insertUser(db);
    insertUser.prepare("INSERT INTO Users (USERNAME, PASSWORD) VALUES (?,?)");
    insertUser.addBindValue( "test2");
    insertUser.addBindValue(  "asdf");
      cout<< insertUser.exec();*/
}

void Database::createUser(const char* username,const char* password){

      QSqlQuery insertUser(db);
    insertUser.prepare("INSERT INTO Users (USERNAME, PASSWORD) VALUES (:username, :password)");
    insertUser.bindValue(":username", username);
    insertUser.bindValue(":password",  password);
    cout<< insertUser.exec();

    cout<<"Creating User and Query"<<endl;
   QSqlQuery get(db);
   cout << get.exec("SELECT USERNAME, PASSWORD FROM users;") << endl;
    while (get.next()) {
          // QString country = get.value(0).toString();
           cout << "Person:" <<get.value(0).toString().toStdString()<< endl;
           cout << "Person:" <<get.value(1).toString().toStdString()<< endl;
    }
}
