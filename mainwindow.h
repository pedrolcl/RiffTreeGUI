// Copyright (C) 2025 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QMenu>
#include <QSplitter>
#include <QTreeView>
#include <qhexview/qhexview.h>

#include "treemodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void open();
    void about();
    void treeItemClicked(const QModelIndex &index);

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QSplitter *m_splitter;
    QTreeView *m_treeview;
    QHexView *m_hexview;

    uint8_t *m_buffer{nullptr};

    TreeModel *m_treemodel{nullptr};
    QHexDocument *m_hexdoc{nullptr};

    QString openFileName;
};

#endif // MAINWINDOW_H
