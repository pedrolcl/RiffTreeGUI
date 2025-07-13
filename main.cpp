// Copyright (C) 2025 Pedro López-Cabanillas
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainwin;
    mainwin.show();
    return QCoreApplication::exec();
}
