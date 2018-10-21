QT += widgets xml

HEADERS       = \
    testpage.h \
    WizardPages/enterpasswordpage.h \
    WizardPages/enterhashpage.h \
    WizardPages/extractcurrentpage.h \
    WizardPages/attackpage.h \
    WizardPages/resultspage.h \
    auxiliary/sam_dialog.h \
    WizardPages/selectmodepage.h \
    pwrecon_global.h \
    pwRecon.h \
    WizardPages/attacksettingspage.h \
    auxiliary/recovery_worker.h \
    auxiliary/sam_dialog.h \
    opencl/include/CL/cl.hpp \
    WizardPages/selectrecoverymodepage.h \
    WizardPages/selecttestmodepage.h \
    WizardPages/evaluationpage.h \
    auxiliary/executeworker.h

SOURCES       = \
                main.cpp \
    testpage.cpp \
    WizardPages/enterpasswordpage.cpp \
    WizardPages/enterhashpage.cpp \
    WizardPages/extractcurrentpage.cpp \
    WizardPages/attackpage.cpp \
    WizardPages/resultspage.cpp \
    auxiliary/sam_dialog.cpp \
    WizardPages/selectmodepage.cpp \
    pwRecon.cpp \
    WizardPages/attacksettingspage.cpp \
    auxiliary/recovery_worker.cpp \
    WizardPages/selectrecoverymodepage.cpp \
    WizardPages/selecttestmodepage.cpp \
    WizardPages/evaluationpage.cpp \
    auxiliary/executeworker.cpp


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/dialogs/licensewizard
INSTALLS += target

FORMS += \
    testpage.ui \
    auxiliary/sam_dialog.ui

TRANSLATIONS += translate/pwRecon_en_US.ts

# OpenCL libs only for Windows included yet
win: contains(QMAKE_HOST.arch, x86_64) {
LIBS += -L$$PWD/opencl/lib/x86_64/ -lOpenCL

INCLUDEPATH = opencl/include \
             $$PWD/opencl/lib/x86_64
DEPENDPATH += $$PWD/opencl/lib/x86_64

opencl_dll.path = $$OUT_PWD
opencl_dll.files = opencl/dll/x86_64/OpenCL.dll
INSTALLS += \
    opencl_dll

}

win{
!win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/OpenCL.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/libOpenCL.a
}
macx{
    QMAKE_LFLAGS += -framework OpenCL
}

linux{

LIBS += -L$$PWD/opencl/lib/x86_64/ -lOpenCL

INCLUDEPATH += opencl/include \
             $$PWD/opencl/lib/x86_64
DEPENDPATH += $$PWD/opencl/lib/x86_64

opencl_dll.path = $$OUT_PWD
opencl_dll.files = opencl/dll/x86_64/OpenCL.dll
INSTALLS += \
    opencl_dll
    PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/OpenCL.lib
    win32-g++: PRE_TARGETDEPS += $$PWD/opencl/lib/x86_64/libOpenCL.a
}
