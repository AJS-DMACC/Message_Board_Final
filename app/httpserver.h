#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <sstream>
#include <QCoreApplication>
#include <QDir>
#include <sstream>
#include "httplib.h"
#include "database.h"
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
            body << "<h1>Welcome to Culinary Discussion Board</h1><form method = \"POST\"><p>Username</p>";
            body << "<input type=\"text\" name=\"username\"><p>Password </p><input type=\"text\" name=\"password\" </br> >";
            body << "<input type=\"submit\">";
            body << "</form><a href=\"signup\">Not a Member? Sign Up</a></html>";
            res.set_content(body.str(), "text/html");
        });        

        svr.Post("/", [](const Request &req, Response &res) {
            string formValue = req.get_param_value("username");
            string formValue = req.get_param_value("username");
            cout << "Username: " << formValue;
            /*auto image_file = req.get_file_value("image_file");
            auto text_file = req.get_file_value("text_file");

            cout << "image file length: " << image_file.content.length() << endl
                 << "image file name: " << image_file.filename << endl
                 << "text file length: " << text_file.content.length() << endl
                 << "text file name: " << text_file.filename << endl;*/

            res.set_content("GOT To Post mapping", "text/plain");
          });


        svr.Get("/signup", [this](const Request&, Response& res) {
            stringstream body;
            body << formatPage();
            body << "<h1>Sign Up for the Discussion Board</h1><form method = \"POST\"><p>Create a Username</p>";
            body << "<input type=\"text\" name=\"username\"><p>Create a Password </p><input type=\"text\" name=\"password\" >";
            body << "<input type=\"submit\">";
            body <<"</form></html>";
            res.set_content(body.str(), "text/html");
        });

        svr.Post("/signup", [](const Request &req, Response &res) {
            string username = req.get_param_value("username");
            string password = req.get_param_value("password");

            Database database;
            database.open();
            database.createUser(username.c_str(), password.c_str());
            database.close();
            res.set_redirect("/");
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
