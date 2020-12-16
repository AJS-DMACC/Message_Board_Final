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
#include "user.h"
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
            body << "<div class=\"header\"><h1>Welcome to Culinary Discussion Board</h1></div><form method = \"POST\"><p>Username</p>";
            body << "<input type=\"text\" name=\"username\"><p>Password </p><input type=\"text\" name=\"password\" </br> ";
            body << "<input type=\"submit\">";
            body << "</form><a href=\"signup\">Not a Member? Sign Up</a></html>";
            res.set_content(body.str(), "text/html");
        });        

        svr.Post("/", [this](const Request &req, Response &res) {
            string username = req.get_param_value("username");
            string password = req.get_param_value("password");
            if(username != "" && password != ""){
            cout << "Username: " << username << " Passord: " << password << endl;
            Database database; database.open();
            if(database.validateCredentials(username, password) != -1){
                userId = database.validateCredentials(username, password);
                res.set_redirect("/home");
            }
            else{
                res.set_redirect("/");
            }
            database.close();
            }else{
                res.set_redirect("/");
            }
          });


        svr.Get("/signup", [this](const Request&, Response& res) {
            stringstream body;
            body << formatPage();
            body << "<div class=\"header\"><h1>Sign Up for the Discussion Board</h1></div><form method = \"POST\"><p>Create a Username</p>";
            body << "<input type=\"text\" name=\"username\"><p>Create a Password </p><input type=\"text\" name=\"password\" >";
            body << "<input type=\"submit\">";
            body <<"</form></html>";
            res.set_content(body.str(), "text/html");
        });

        svr.Post("/signup", [](const Request &req, Response &res) {
            string username = req.get_param_value("username");
            string password = req.get_param_value("password");
            if(username != "" && password != ""){
            Database database;
            database.open();
            database.createUser(username.c_str(), password.c_str());
            database.close();
            res.set_redirect("/");}
            else{
                res.set_redirect("/signup");
            }
          });

        svr.Get("/home", [this](const Request&, Response& res) {
            const char *html = R"(<div class="header">
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
            const char *html = R"(<div class="header">
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

            if(title != "" && body != ""){
            Database database;
            database.open();
            database.createPost(userId, title.c_str(), body.c_str());
            database.close();
            }
            res.set_redirect("/home");

        });

        svr.Get(R"(/viewpost/(\d+))", [&](const Request& req, Response& res) {
            Database database;
            database.open();
            const char *html = R"(</h3>
                                    <p>
                                      Comments:
                                    </p>
                                    <table border=1>
                                      <tr><td>
                                        <form method=POST>
                                          <input type="text" placeholder="Make a Comment" name="comment">
                                          <input type="submit"  value="Comment">
                                        </form>
                                        </td></tr>)";
            int postID = stoi(string(req.matches[1]));
            cout << "Post request id " << postID << endl;
            post Post = database.getPostById(postID);//get the Post from the database
            user User = database.getUserByPostId(postID);
            vector<string> comments = database.getCommentsByPostId(postID);

            stringstream body;
            body<< formatPage() << "<div class=\"header\"><h1>" << Post.getTitle();
            body<< "</h1><a href=\"/viewprofile/" << User.getUserId() << "\">post created by:  " << User.getUsername() << "</a><br><a href=\"/home\">back</a></div><h3>";
            body<< Post.getBody();
            body<< html;
            for(int i=0; i < (int)comments.size(); i++){
                body<<" <tr><td><p>";
                      body<< comments[i];
                      body<< "</p></td></tr>";
            }
            body<< "</table>";
            body << "</body></html>";
            res.set_content(body.str(), "text/html");
            database.close();
        });

        svr.Post(R"(/viewpost/(\d+))", [&](const Request& req, Response& res) {
            Database database;
            database.open();
            database.createComment(stoi(string(req.matches[1])), req.get_param_value("comment").c_str() );
            database.close();
            string redirectURL = "/viewpost/"; redirectURL.append(string(req.matches[1]));
            res.set_redirect(redirectURL.c_str());

        });

        svr.Get(R"(/viewprofile/(\d+))", [this](const Request&req, Response& res) {
            const char *html = R"(<div class=\"header\">
                  <a href="/home">Home</a>)";
            stringstream body;
            body << formatPage() << html;//format the start of the page
            Database database; database.open();
            body << "<h2>Profile for " << database.getUsernameById(stoi(string(req.matches[1]))) << " </h2></div>";
            body << "<table border=1><tr><td>User's Posts</td></tr>";
            if(database.hasPosts()){
                map<int,string> posts = database.getUserPosts(stoi(string(req.matches[1])));//get the posts
                map<int, string>::iterator itr; //set an iterator
                for (itr = posts.begin(); itr != posts.end(); ++itr) {//for each post, set a link to that post, where the post id is in the url
                   // cout << "POST " << itr->first << " : " << itr->second << endl;
                    string concat =  "<tr><td><a href=\"/viewpost/";
                    concat.append(to_string(itr->first));
                    body << concat << "\">" << itr->second << "</a></td></tr>";
                }
            }
            //for each post, get its id and title, and put it as a headline
            body << " </table></html>";
            res.set_content(body.str(), "text/html");
        });


        cout << "Starting this server at http://" << domain << ":" << port << endl;
        svr.listen(domain, port);                
    }        

    string formatPage(){
        const char *format = R"(/* W3.CSS 4.15 December 2020 by Jan Egil and Borge Refsnes */
                             html{box-sizing:border-box}*,*:before,*:after{box-sizing:inherit}
                             .header{width:100%; text-align:center; background-color:#db5461;}
                             /* Extract from normalize.css by Nicolas Gallagher and Jonathan Neal git.io/normalize */
                             html{background-color:#8aa29e; -ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin: 30; padding:30; background-color:#fafafa;}
                             article,aside,details,figcaption,figure,footer,header,main,menu,nav,section{display:block}summary{display:list-item}
                             audio,canvas,progress,video{display:inline-block}progress{vertical-align:baseline}
                             audio:not([controls]){display:none;height:0}[hidden],template{display:none}
                             a{background-color:transparent}a:active,a:hover{outline-width:0}
                             abbr[title]{border-bottom:none;text-decoration:underline;text-decoration:underline dotted}
                             b,strong{font-weight:bolder}dfn{font-style:italic}mark{background:#ff0;color:#000}
                             small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}
                             sub{bottom:-0.25em}sup{top:-0.5em}figure{margin:1em 40px}img{border-style:none}
                             code,kbd,pre,samp{font-family:monospace,monospace;font-size:1em}hr{box-sizing:content-box;height:0;overflow:visible}
                             button,input,select,textarea,optgroup{font:inherit;margin:0}optgroup{font-weight:bold}
                             button,input{overflow:visible}button,select{text-transform:none}
                             button,[type=button],[type=reset],[type=submit]{-webkit-appearance:button}
                             button::-moz-focus-inner,[type=button]::-moz-focus-inner,[type=reset]::-moz-focus-inner,[type=submit]::-moz-focus-inner{border-style:none;padding:0}
                             button:-moz-focusring,[type=button]:-moz-focusring,[type=reset]:-moz-focusring,[type=submit]:-moz-focusring{outline:1px dotted ButtonText}
                             fieldset{border:1px solid #c0c0c0;margin:0 2px;padding:.35em .625em .75em}
                             legend{color:inherit;display:table;max-width:100%;padding:0;white-space:normal}textarea{overflow:auto}
                             [type=checkbox],[type=radio]{padding:0}
                             [type=number]::-webkit-inner-spin-button,[type=number]::-webkit-outer-spin-button{height:auto}
                             [type=search]{-webkit-appearance:textfield;outline-offset:-2px}
                             [type=search]::-webkit-search-decoration{-webkit-appearance:none}
                             ::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}
                             /* End extract */
                             html,body{font-family:Verdana,sans-serif;font-size:15px;line-height:1.5}html{overflow-x:hidden}
                             h1{font-size:46px}h2{font-size:38px}h3{font-size:24px}h4{font-size:20px}h5{font-size:18px}h6{font-size:16px}
                             .w3-serif{font-family:serif}.w3-sans-serif{font-family:sans-serif}.w3-cursive{font-family:cursive}.w3-monospace{font-family:monospace}
                             h1,h2,h3,h4,h5,h6{font-family:"Segoe UI",Arial,sans-serif;font-weight:400;margin:10px 0}.w3-wide{letter-spacing:4px}
                             hr{border:0;border-top:1px solid #eee;margin:20px 0}
                             table{width:70%;  margin-left: auto; margin-right: auto;}
                             td{font-size:28px;})";
        stringstream css;
        css << "<html><head><style>";
        css<< format << " </style>";
        return css.str();
    }
};

#endif // HTTPSERVER_H
