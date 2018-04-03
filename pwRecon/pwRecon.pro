QT += widgets printsupport

HEADERS       = \
    testpage.h \
    WizardPages\enterpasswordpage.h \
    WizardPages\enterhashpage.h \
    WizardPages\loadpasswordspage.h \
    WizardPages\loadhashespage.h \
    WizardPages\extractcurrentpage.h \
    WizardPages\attackpage.h \
    WizardPages\resultspage.h \
    auxiliary\sam_dialog.h \
    WizardPages\selectmodepage.h \
    pwrecon_global.h \
    pwRecon.h \
    WizardPages/attacksettingspage.h
SOURCES       = \
                main.cpp \
    testpage.cpp \
    WizardPages\enterpasswordpage.cpp \
    WizardPages\enterhashpage.cpp \
    WizardPages\loadpasswordspage.cpp \
    WizardPages\loadhashespage.cpp \
    WizardPages\extractcurrentpage.cpp \
    WizardPages\attackpage.cpp \
    WizardPages\resultspage.cpp \
    auxiliary\sam_dialog.cpp \
    WizardPages\selectmodepage.cpp \
    pwRecon.cpp \
    WizardPages/attacksettingspage.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/dialogs/licensewizard
INSTALLS += target

FORMS += \
    testpage.ui \
    auxiliary\sam_dialog.ui
