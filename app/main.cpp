#include<iostream>
#include "httpserver.h"
#include "database.h"
using namespace std;


int main()
{
    Database database;
    database.open();
    database.close();
    HttpServer server;

    server.startServer();

    return 0;
}

