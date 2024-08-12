/**

* @file main.cpp
* @brief Entry point for the line follower program.
*/
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/**

* @brief The main function of the program.
* @param argc The number of arguments passed to the program.
* @param argv An array of pointers to the arguments.
* @return The exit code of the program.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "line_follower_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
