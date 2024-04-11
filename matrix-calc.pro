QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/details.cpp \
    src/preview.cpp \
    src/creatematrix.cpp \
    src/error.cpp \
    src/exporer.cpp \
    src/inmatrix.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/sparsematrixmodel.cpp \
    src/matrixitem.cpp

HEADERS += \
    headers/details.h \
    headers/creatematrix.h \
    headers/error.h \
    headers/explorer.h \
    headers/inmatrix.h \
    headers/mainwindow.h \
    headers/preview.h \
    headers/sparsematrixmodel.h \
    headers/matrixitem.h

FORMS += \
    ui/details.ui \
    ui/preview.ui \
    ui/creatematrix.ui \
    ui/error.ui \
    ui/explorer.ui \
    ui/inmatrix.ui \
    ui/mainwindow.ui \
    ui/matrixitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/library/cmake-build-release/ -llibSparseMatrix.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/library/cmake-build-debug/ -llibSparseMatrix.dll

INCLUDEPATH += $$PWD/library/headers
DEPENDPATH += $$PWD/library/headers
