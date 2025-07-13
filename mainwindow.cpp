// Copyright (C) 2025 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>

//using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_treeview{new QTreeView(this)}
    , m_hexview{new QHexView(this)}
{
    m_treeview->setModel(m_treemodel);
    m_hexview->setReadOnly(true);

    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_treeview);
    m_splitter->addWidget(m_hexview);
    m_splitter->setSizes({333, 666});
    setCentralWidget(m_splitter);

    createActions();
    createMenus();

    setWindowTitle(tr("Riff Tree"));
    setWindowIcon(QIcon(":/RiffTree.png"));
    setMinimumSize(666, 666);
    const auto screenSize = screen()->availableSize();
    resize({screenSize.width() / 2, screenSize.height() * 2 / 3});

    connect(m_treeview, &QTreeView::clicked, this, &MainWindow::treeItemClicked);
}

void MainWindow::open()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        openFileName,
        tr("All Files (*);;Riff Files (*.dls *.sf2 *.avi *.wav *.rmi)"),
        &selectedFilter,
        QFileDialog::ReadOnly);
    if (!fileName.isEmpty()) {
        openFileName = fileName;
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        delete m_treemodel;
        m_treemodel = new TreeModel(this);
        m_treeview->setModel(m_treemodel);
        m_treeview->setSelectionMode(QAbstractItemView::SingleSelection);
        m_treeview->setColumnWidth(0, 100);
        m_treeview->setColumnWidth(1, 66);
        m_treeview->setColumnWidth(2, 66);
        bool ok = m_treemodel->loadData(&file);
        file.close();

        if (ok) {
            if (m_hexdoc) {
                m_hexdoc->deleteLater();
            }
            m_hexdoc = QHexDocument::fromMappedFile(fileName, this);
            m_hexview->setDocument(m_hexdoc);

            m_treeview->expandAll();
            m_treeview->resizeColumnToContents(0);
        }
    }
}

void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About RiffTree"),
                       tr("RiffTree is a RIFF file structure viewer\n"
                          "with hex content view"));
}

void MainWindow::treeItemClicked(const QModelIndex &index)
{
    QString title = m_treemodel->data(index.sibling(index.row(), 0), Qt::DisplayRole).toString();
    qint64 offs = m_treemodel->data(index.sibling(index.row(), 1), Qt::DisplayRole).toLongLong();
    qint64 size = m_treemodel->data(index.sibling(index.row(), 2), Qt::DisplayRole).toLongLong()
                  + 2 * sizeof(uint32_t);
    //qDebug() << Q_FUNC_INFO << title << "offset:" << offs << "size:" << size;

    // m_hexview->clearMetadata();
    // m_hexview->hexCursor()->move(offs);
    // m_hexview->setMetadata(offs, offs + size, Qt::black, Qt::yellow, title);

    m_hexview->hexCursor()->clearSelection();
    m_hexview->hexCursor()->move(offs);
    m_hexview->hexCursor()->select(offs);
    m_hexview->hexCursor()->selectSize(size);
}

void MainWindow::createActions()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QIcon openIcon = QIcon::fromTheme("document-open");
#else
    QIcon openIcon = QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen);
#endif
    openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QIcon exitIcon = QIcon::fromTheme("application-exit");
#else
    QIcon exitIcon = QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit);
#endif
    exitAct = new QAction(exitIcon, tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QIcon helpIcon = QIcon::fromTheme("help-about");
#else
    QIcon helpIcon = QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout);
#endif
    aboutAct = new QAction(helpIcon, tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    QIcon aboutQtIcon = qApp->style()->standardIcon(QStyle::SP_TitleBarMenuButton);
    aboutQtAct = new QAction(aboutQtIcon, tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
