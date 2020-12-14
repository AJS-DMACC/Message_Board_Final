#include<iostream>
#include "httpserver.h"
#include "database.h"
using namespace std;

bool rebuild=false;
int main()
{
    if(rebuild){Database database; database.open(); database.rebuild(); database.close();}
    /*Database db;
    db.open();
    db.rebuild();
    db.createUser("test", "password");
    db.createUser("asdf", "password");
    db.close();*/
    HttpServer server;

   server.startServer();

    return 0;
}

