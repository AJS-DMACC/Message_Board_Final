#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <sstream>
#include <QCoreApplication>
#include <QDir>
#include <sstream>
#include <map>
#include "httplib.h"
#include "database.h"
using namespace std;
using namespace httplib;

class HttpServer {
private:
    char* domain = "localhost";
    int port = 8080;
    int userId = -1;
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

        svr.Post("/", [this](const Request &req, Response &res) {
            string username = req.get_param_value("username");
            string password = req.get_param_value("username");
            cout << "Username: " << username << " Passord: " << password << endl;
            Database database; database.open();
            if(database.validateCredentials(username, password) != -1){
                userId = database.validateCredentials(username, password);
                res.set_redirect("/home");
            }

            database.close();
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

        svr.Get("/home", [this](const Request&, Response& res) {
            const char *html = R"(<div id=\"header\">
                    <h2>Welcome to the Discussion Board</h2>
                  <a href="logout">Logout</a>
                  </div>
                  <a href="newPost">Make A New Post</a>
                  <table border=1>
                    <tr><td>Most Recent Posts</td></tr>)";
            stringstream body;
            body << formatPage() << html;//format the start of the page
            Database database; database.open();
            if(database.hasPosts()){
                map<int,string> posts = database.getPosts();//get the posts
                map<int, string>::iterator itr; //set an iterator
                for (itr = posts.begin(); itr != posts.end(); ++itr) {//for each post, set a link to that post, where the post id is in the url
                   // cout << "POST " << itr->first << " : " << itr->second << endl;
                    string concat =  "<tr><td><a href=\"viewpost/";
                    concat.append(to_string(itr->first));
                    body << concat << "\">" << itr->second << "</a></td></tr>";
                }
            }
            //for each post, get its id and title, and put it as a headline
            body << " </table></html>";
            res.set_content(body.str(), "text/html");
        });

        svr.Get("/logout", [this](const Request&, Response& res) {
            userId = -1;
            res.set_redirect("/");
        });

        svr.Get("/newPost", [this](const Request&, Response& res) {
            const char *html = R"(<div id="header">
                               <h1>Make A New Post</h1>
                               </div>
                               <form method="POST">
                               <p>Title</p>
                               <input name="title" type="text" />
                               <p>Body</p>
                               <textarea cols="40" name="body" rows="6"></textarea>
                                <input type="submit" value="Post" style="width:50%;">
                               </form>)";
            stringstream body;
            body << formatPage() << html;
            res.set_content(body.str(), "text/html");
        });

        svr.Post("/newPost", [this](const Request &req, Response& res) {
            string title = req.get_param_value("title");
            string body = req.get_param_value("body");

            Database database;
            database.open();
            database.createPost(userId, title.c_str(), body.c_str());
            database.close();

            res.set_redirect("/home");
        });

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
