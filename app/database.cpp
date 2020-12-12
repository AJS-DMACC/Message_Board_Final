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
    cout << query.exec("CREATE TABLE IF NOT EXISTS Users(USER_ID integer primary key, USERNAME varchar(30) not null, PASSWORD varchar(30) not null);") << endl;
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
   cout << get.exec("SELECT USERNAME, PASSWORD, USER_ID FROM users;") << endl;
    while (get.next()) {
          // QString country = get.value(0).toString();
           cout << "Person:" <<get.value(0).toString().toStdString()<< endl;
           cout << "pass:" <<get.value(1).toString().toStdString()<< endl;
           cout << "userid:" <<get.value(2).toInt()<< endl;
    }
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
