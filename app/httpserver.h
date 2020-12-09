#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <sstream>
#include <QCoreApplication>
#include <QDir>
#include <sstream>
#include "httplib.h"
using namespace std;
using namespace httplib;

class HttpServer {
private:
    char* domain = "localhost";
    int port = 8080;
public:
    void startServer() {
        Server svr;

        svr.Get("/", [this](const Request&, Response& res) {            
            stringstream body;
            body << formatPage();
            body << "<h1>Welcome to Culinary Discussion Board</h1>";
            body << "<p>To view an account balance visit http://localhost:" << this->port;
            body << "/accounts/x where x is the account number.</p></html>";
            res.set_content(body.str(), "text/html");
        });

        /*
        svr.Get(R"(/accounts/(\d+))", [&](const Request& req, Response& res) {
          Database database;
            database.open();

            int accountId = stoi(string(req.matches[1]));
            Account account = Account(accountId);//This is assignment is redundant, but in there are no 0-args constructors for the account
            bank.getDatabase().getAccount(accountId, account);
            stringstream body;
            body << "<html> <body style=\"background-color:grey;\">";

            body << "<h1 style=\"background-color:blue; color:white;\"> Account #" << accountId << "</h1>";
            body << "</hr><p>Balance: $" << account.getBalance() << "</p>";
             TODO: Respond with a message containing the account id and balance.
             * hint: Use bank.getDatabase().getAccount(accountId, account)

            body << "</body></html>";
            res.set_content(body.str(), "text/html");
            database.close();

        });*/

        cout << "Starting this server at http://" << domain << ":" << port << endl;
        svr.listen(domain, port);
    }

    string formatPage(){
        stringstream css;
        css << "<html><head><style>";
        css<<"h1{ color:#ff0000; font-size: 1em; line-height: 1.4;} </style>";
        return css.str();
    }
};

#endif // HTTPSERVER_H
