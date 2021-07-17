QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS =AppIcon.ico  # 设置可执行文件的图标。

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Highlighter.cpp \
    LexAnalyzer.cpp \
    Lex_main.cpp \
    Syn_main.cpp \
    SyntaxAnalyzer.cpp \
    main.cpp \
    mainwindow.cpp \
    sub3.cpp \
    sublex.cpp \
    subsyn.cpp

HEADERS += \
    Highlighter.h \
    LexAnalyzer.h \
    Lex_main.h \
    Syn_main.h \
    SyntaxAnalyzer.h \
    mainwindow.h \
    sub3.h \
    sublex.h \
    subsyn.h

FORMS += \
    mainwindow.ui \
    sub3.ui \
    sublex.ui \
    subsyn.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
