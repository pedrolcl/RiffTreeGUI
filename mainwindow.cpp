// Copyright (C) 2025 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLocale>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QScreen>
#include <QSettings>
#include <algorithm>

#include <qhexview/model/buffer/qmemorybuffer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_treeview{new QTreeView(this)}
    , m_hexview{new QHexView(this)}
{
    m_treeview->setModel(m_treemodel);
    m_hexview->setDocument(m_hexdoc);
    m_hexview->setReadOnly(true);

    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_treeview);
    m_splitter->addWidget(m_hexview);
    m_splitter->setSizes({333, 666});
    setCentralWidget(m_splitter);

    createActions();
    createMenus();

    setWindowIcon(QIcon(":/RiffTree.png"));
    setMinimumSize(666, 666);
    const auto screenSize = screen()->availableSize();
    resize({screenSize.width() / 2, screenSize.height() * 2 / 3});

    connect(m_treeview, &QTreeView::clicked, this, &MainWindow::treeItemClicked);
    updateWindowTitle();
    readSettings();
}

void MainWindow::openFile(const QString fileName)
{
    QFile file(fileName);
    bool Ok = file.open(QIODevice::ReadOnly);
    if (Ok) {
        // QFile::map doesn't allow options like MAP_HUGETLB, MAP_PRIVATE or MAP_LOCKED
        // but it is more portable between different operating systems than mmap().
        // Previously, we tried to use MAP_HUGETLB with mmap() syscall but it is only
        // valid for anonymous memory.
        uint8_t *buffer = file.map(0, file.size());
        if (buffer == nullptr) {
            QMessageBox::warning(this, qApp->applicationName(), file.errorString());
            file.close();
            return;
        }

        delete m_treemodel;
        m_treemodel = new TreeModel(this);

        m_treeview->setModel(m_treemodel);
        m_treeview->setSelectionMode(QAbstractItemView::SingleSelection);
        m_treeview->setColumnWidth(0, 100);
        m_treeview->setColumnWidth(1, 66);
        m_treeview->setColumnWidth(2, 66);
        m_hexview->setDocument(nullptr);

        if (m_treemodel->loadData(buffer)) {
            m_hexdoc = QHexDocument::fromMemory<QMemoryBuffer>(reinterpret_cast<char *>(buffer),
                                                               file.size());
            m_hexview->setDocument(m_hexdoc);
            m_treeview->expandAll();
            m_treeview->resizeColumnToContents(0);

            m_openFileName = QFileInfo(fileName).fileName();
            updateWindowTitle();
        } else {
            QMessageBox::warning(this,
                                 qApp->applicationName(),
                                 tr("%1 is not a valid RIFF file").arg(fileName));
        }

        file.unmap(buffer);
        file.close();
    }
}

void MainWindow::open()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        m_openFileName,
        tr("All Files (*);;Riff Files (*.dls *.sf2 *.sf3 *.avi *.wav "
           "*.rmi *.cdr *.ani *.pal *.webp)"),
        &selectedFilter,
        QFileDialog::ReadOnly);
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(tr("%1 [%2]").arg(qApp->applicationName()).arg(m_openFileName));
}

void MainWindow::retranslate()
{
    qDebug() << Q_FUNC_INFO << m_currentLang;
    QLocale locale(m_currentLang);
    qDebug() << "load Qt translator:" << locale.name()
             << qtTranslator.load(locale, "qt", "_", ":/");
    QCoreApplication::installTranslator(&qtTranslator);

    qDebug() << "load app translator:" << locale.name()
             << appTranslator.load(locale, qApp->applicationName(), "_", ":/");
    QCoreApplication::installTranslator(&appTranslator);
    // retranslate the menus
    fileMenu->setTitle(tr("&File"));
    editMenu->setTitle(tr("&Edit"));
    helpMenu->setTitle(tr("&Help"));
    languageMenu->setTitle(tr("&Language"));
    openAct->setText(tr("&Open..."));
    openAct->setStatusTip(tr("Open an existing file"));
    exitAct->setText(tr("E&xit"));
    exitAct->setStatusTip(tr("Exit the application"));
    aboutAct->setText(tr("&About"));
    aboutAct->setStatusTip(tr("Show the application's About box"));
    aboutQtAct->setText(tr("About &Qt"));
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    findAct->setText(tr("Find..."));
    findAct->setStatusTip(tr("Show the Find dialog"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    QByteArray geometry = settings.value("geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    m_currentLang = settings.value("language").toString();
    auto allActions = languageMenu->actions();
    auto it = std::find_if(allActions.begin(), allActions.end(), [=](QAction *action) {
        return action->data().toString() == m_currentLang;
    });
    if (it != actions().end()) {
        (*it)->setChecked(true);
    }
    retranslate();
}

void MainWindow::changeLanguage()
{
    auto action = qobject_cast<QAction *>(sender());
    if (action) {
        m_currentLang = action->data().toString();
        retranslate();
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
    // QString title = m_treemodel->data(index.sibling(index.row(), 0), Qt::DisplayRole).toString();
    qint64 offs = m_treemodel->data(index.sibling(index.row(), 1), Qt::DisplayRole).toLongLong();
    qint64 size = m_treemodel->data(index.sibling(index.row(), 2), Qt::DisplayRole).toLongLong()
                  + 2 * sizeof(uint32_t);

    // qDebug() << Q_FUNC_INFO << title << "offset:" << offs << "size:" << size;
    // m_hexview->clearMetadata();
    // m_hexview->hexCursor()->move(offs);
    // m_hexview->setMetadata(offs, offs + size, Qt::black, Qt::yellow, title);

    m_hexview->hexCursor()->clearSelection();
    m_hexview->hexCursor()->move(offs);
    m_hexview->hexCursor()->select(offs);
    m_hexview->hexCursor()->selectSize(size);
    m_hexview->update();
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QIcon findIcon = QIcon::fromTheme("edit-find");
#else
    QIcon findIcon = QIcon::fromTheme(QIcon::ThemeIcon::EditFind);
#endif
    findAct = new QAction(findIcon, tr("Find..."), this);
    findAct->setStatusTip(tr("Show the Find dialog"));
    connect(findAct, &QAction::triggered, m_hexview, &QHexView::showFind);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(findAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    languageMenu = helpMenu->addMenu(tr("&Language"));
    auto langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);
    auto enAct = languageMenu->addAction("English"); // Native name, do not translate
    enAct->setData("en_US");
    enAct->setCheckable(true);
    enAct->setActionGroup(langGroup);
    auto esAct = languageMenu->addAction("español"); // Native name, do not translate
    esAct->setData("es_ES");
    esAct->setCheckable(true);
    esAct->setActionGroup(langGroup);

    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    connect(enAct, &QAction::triggered, this, &MainWindow::changeLanguage);
    connect(esAct, &QAction::triggered, this, &MainWindow::changeLanguage);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList types{"dls", "sf2", "sf3", "avi", "wav", "rmi", "cdr", "ani", "pal", "webp"};
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fname = url.toLocalFile();
        QFileInfo info(fname);
        if (info.exists() && types.contains(info.suffix().trimmed(), Qt::CaseInsensitive)) {
            openFile(fname);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("language", m_currentLang);
    QMainWindow::closeEvent(event);
}
