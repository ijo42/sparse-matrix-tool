QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    creatematrix.cpp \
    error.cpp \
    explorer.cpp \
    inmatrix.cpp \
    main.cpp \
    mainwindow.cpp \
    warring.cpp

HEADERS += \
    creatematrix.h \
    error.h \
    explorer.h \
    inmatrix.h \
    mainwindow.h \
    operations.h \
    shared.h \
    warring.h

FORMS += \
    creatematrix.ui \
    error.ui \
    explorer.ui \
    inmatrix.ui \
    mainwindow.ui \
    warring.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lSparseMatrix
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lSparseMatrix

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/libSparseMatrix.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/libSparseMatrix.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../release/SparseMatrix.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../debug/SparseMatrix.lib
