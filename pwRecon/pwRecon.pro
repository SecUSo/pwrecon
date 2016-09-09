QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pwRecon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    recovery_worker.cpp \
    type_password_dialog.cpp \
    sam_dialog.cpp \
    question_dialog.cpp

HEADERS  += mainwindow.h \
    recovery_worker.h \
    type_password_dialog.h \
    sam_dialog.h \
    question_dialog.h

FORMS    += mainwindow.ui \
    type_password_dialog.ui \
    sam_dialog.ui \
    question_dialog.ui

extra_files.path = $$OUT_PWD
extra_files.files = external/*
# Requires to invoke "make install" during build process.
# Otherwise, the program will not work.
INSTALLS += \
    extra_files

RESOURCES +=

DISTFILES +=

# OpenCL libs only for Windows included yet
win32: contains(QMAKE_HOST.arch, x86_64) {
LIBS += -L$$PWD/opencl/lib/x86_64/ -lOpenCL

INCLUDEPATH += opencl/include \
             $$PWD/opencl/lib/x86_64
DEPENDPATH += $$PWD/opencl/lib/x86_64

opencl_dll.path = $$OUT_PWD
opencl_dll.files = opencl/dll/x86_64/OpenCL.dll
INSTALLS += \
    opencl_dll

} else {
LIBS += -L$$PWD/opencl/lib/x86/ -lOpenCL

INCLUDEPATH += opencl/include \
             $$PWD/opencl/lib/x86
DEPENDPATH += $$PWD/opencl/lib/x86

opencl_dll.path = $$OUT_PWD
opencl_dll.files = opencl/dll/x86/OpenCL.dll
INSTALLS += \
    opencl_dll
}

win32: contains(QMAKE_HOST.arch, x86_64) {
!win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/OpenCL.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/libOpenCL.a
} else {
!win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86/OpenCL.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86/libOpenCL.a
}
