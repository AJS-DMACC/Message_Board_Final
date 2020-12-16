#include <QtTest>
#include "database.h"
#include "user.h"

// add necessary includes here

class forumTest : public QObject
{
    Q_OBJECT

public:
    Database database;
    forumTest();
    ~forumTest();

private slots:
   void testGetComments();
   void testGetUsername();
   void testCredentials();
   void testMakeUser();
   void testGetPosts();
};

forumTest::forumTest()
{
    database.open();
}

forumTest::~forumTest()
{
    database.close();
}

void forumTest::testGetComments()
{
    database.rebuild();
    user test_user = user(1, "test_user", "test_password");
    database.createUser(test_user.getUsername().c_str(), test_user.getPassword().c_str());
    database.createPost(1,"testPost","testBody");
    vector<string>comments = {"comment1", "comment2", "comment3"};
    database.createComment(1, comments.at(0).c_str());
    database.createComment(1, comments.at(1).c_str());
    database.createComment(1, comments.at(2).c_str());
    vector<string> responce = database.getCommentsByPostId(1);
    QCOMPARE(comments, responce);
}

void forumTest::testGetUsername()
{
    database.rebuild();
    user test_user = user(1, "test_user", "test_password");
    database.createUser(test_user.getUsername().c_str(), test_user.getPassword().c_str());
    database.createPost(1,"testPost","testBody");
    string compare_user = database.getUsernameById(1);
    QCOMPARE(test_user.getUsername(), compare_user);
}


void forumTest::testGetPosts()
{
    database.rebuild();
    user test_user = user(1, "test_user", "test_password");
    database.createUser(test_user.getUsername().c_str(), test_user.getPassword().c_str());
    map<int,string> posts;
    posts.insert(pair<int,string>(1,"Title1"));
    posts.insert(pair<int,string>(2,"Title2"));
    posts.insert(pair<int,string>(3,"Title3"));
    database.createPost(1,posts.find(1)->second.c_str(),"testBody");
    database.createPost(1,posts.find(2)->second.c_str(),"testBody");
    database.createPost(1,posts.find(3)->second.c_str(),"testBody");
    map<int,string> responce = database.getPosts();
    QCOMPARE(posts, responce);
}


void forumTest::testMakeUser()
{
    database.rebuild();
    user test_user = user(1, "test_user", "test_password");
    database.createUser(test_user.getUsername().c_str(), test_user.getPassword().c_str());
    database.createPost(1,"testPost","testBody");
    user compare_user = database.getUserByPostId(1);
    QCOMPARE(test_user.getUsername(), compare_user.getUsername());
    QCOMPARE(test_user.getPassword(), compare_user.getPassword());
    QCOMPARE(test_user.getUserId(), compare_user.getUserId());
}

void forumTest::testCredentials()
{
    database.rebuild();
    database.createUser("username", "password");
    QCOMPARE(database.validateCredentials("username","password"), 1);
    QCOMPARE(database.validateCredentials("invalid","credentials"), -1);
}

QTEST_APPLESS_MAIN(forumTest)

#include "tst_forumtest.moc"
