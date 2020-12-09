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
    db.close();
    QSqlDatabase::removeDatabase(DRIVER);
}
