QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MatrixManager.cpp \
    creatematrix.cpp \
    error.cpp \
    inmatrix.cpp \
    main.cpp \
    mainwindow.cpp \
    preview.cpp \
    sparsematrixmodel.cpp \
    warring.cpp

HEADERS += \
    creatematrix.h \
    error.h \
    explorer.h \
    inmatrix.h \
    mainwindow.h \
    preview.h \
    sparsematrixmodel.h \
    warring.h

FORMS += \
    creatematrix.ui \
    error.ui \
    explorer.ui \
    inmatrix.ui \
    mainwindow.ui \
    preview.ui \
    warring.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../sparse-matrix-consoleline/cmake-build-release/ -llibSparseMatrix.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../sparse-matrix-consoleline/cmake-build-debug/ -llibSparseMatrix.dll

INCLUDEPATH += $$PWD/../../sparse-matrix-consoleline
DEPENDPATH += $$PWD/../../sparse-matrix-consoleline

DISTFILES +=
