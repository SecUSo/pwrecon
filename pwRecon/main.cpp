#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "pwRecon.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += "en";
//    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    qDebug() << QLibraryInfo::location(QLibraryInfo::TranslationsPath) << endl;
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

#endif

    pwRecon wizard;
    wizard.show();
    return app.exec();
}
