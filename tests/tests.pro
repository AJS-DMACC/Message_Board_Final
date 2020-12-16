QT += testlib
QT -= gui
QT += sql
QT += network

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../app

SOURCES += ../app/comment.cpp \
    ../app/database.cpp \
    ../app/post.cpp \
    ../app/user.cpp \
    tst_forumtest.cpp
