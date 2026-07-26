// Copyright (C) 2025-2026 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class AboutDialog : public QDialog {
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
};

#endif // ABOUTDIALOG_H
