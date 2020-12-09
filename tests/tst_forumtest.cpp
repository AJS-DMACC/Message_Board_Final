#include <QtTest>

// add necessary includes here

class forumTest : public QObject
{
    Q_OBJECT

public:
    forumTest();
    ~forumTest();

private slots:
   void test_case1();
};

forumTest::forumTest()
{

}

forumTest::~forumTest()
{

}


void forumTest::test_case1()
{
    QCOMPARE(true, true);
}

QTEST_APPLESS_MAIN(forumTest)

#include "tst_forumtest.moc"
