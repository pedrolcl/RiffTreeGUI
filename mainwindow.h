// Copyright (C) 2025 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QDragEnterEvent>
#include <QDropEvent>
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
    void openFile(const QString fileName);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void open();
    void about();
    void treeItemClicked(const QModelIndex &index);
    void updateWindowTitle();

private:
    void createActions();
    void createMenus();

    QMenu *editMenu;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *findAct;

    QSplitter *m_splitter;
    QTreeView *m_treeview;
    QHexView *m_hexview;

    TreeModel *m_treemodel{nullptr};
    QHexDocument *m_hexdoc{nullptr};

    QString m_openFileName;
};

#endif // MAINWINDOW_H
