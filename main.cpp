// Copyright (C) 2025 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QT_STRINGIFY(PROGRAM));
    QCoreApplication::setApplicationVersion(QT_STRINGIFY(VERSION));

    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "RIFF file");
    parser.process(app);
    // Retrieve command line arguments from Qt and parse options
    QStringList args = parser.positionalArguments();

    MainWindow mainwin;
    mainwin.show();
    if (args.size() > 0) {
        mainwin.openFile(args.first());
    }
    return QCoreApplication::exec();
}
