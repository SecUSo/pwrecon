/*
 * Copyright (c) 2018 Christoph Wütschner <christoph-wuetschner@gmx.de>
 *
 * See LICENSE dist-file for details.
 */

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "pwRecon.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // This snipptet is needed to set the current path of the application which is important especially for mac
    QString tmp = QCoreApplication::applicationFilePath();
            tmp = tmp.mid(0, tmp.lastIndexOf(QDir::separator()));
            qDebug() << "Current Path: " << tmp << endl;
    QDir::setCurrent(tmp);

    pwRecon wizard;
    wizard.show();
    return app.exec();
}
