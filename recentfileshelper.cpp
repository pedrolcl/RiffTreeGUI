// Copyright (C) 2025-2026 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QAction>
#include <QFileInfo>
#include <QMenu>
#include <QSettings>

#include "recentfileshelper.h"

RecentFilesHelper::RecentFilesHelper(QObject *parent)
	: QObject(parent)
{
	QMenu* menu = qobject_cast<QMenu *>(parent);
	if (menu) {
		for (int i = 0; i < MaxRecentFiles; ++i) {
			recentFileActs[i] = new QAction(this);
			recentFileActs[i]->setVisible(false);
			menu->addAction(recentFileActs[i]);
            connect(recentFileActs[i], &QAction::triggered, this, &RecentFilesHelper::openRecentFile);
		}
		menu->addSeparator();
		clearAct = new QAction(tr("Clear recent files"), this);
        clearAct->setIcon(QIcon::fromTheme("edit-delete"));
        menu->addAction(clearAct);
        connect(clearAct, &QAction::triggered, this, &RecentFilesHelper::clear);
    }
}

void RecentFilesHelper::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        emit selectedFile(action->data().toString());
    }
}

void RecentFilesHelper::clear()
{
    QSettings settings;
    settings.beginGroup("RecentFiles");
    settings.setValue("recentFileList", QStringList());
    settings.endGroup();
    updateRecentFileActions();
}

void RecentFilesHelper::setCurrentFile(const QString &fileName)
{
    QSettings settings;
    settings.beginGroup("RecentFiles");
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    settings.endGroup();
    updateRecentFileActions();
}

void RecentFilesHelper::updateRecentFileActions()
{
    QSettings settings;
    settings.beginGroup("RecentFiles");
    QStringList files = settings.value("recentFileList").toStringList();
    settings.endGroup();
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        recentFileActs[j]->setVisible(false);
    }
	clearAct->setEnabled(numRecentFiles > 0);
}

QString RecentFilesHelper::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QStringList RecentFilesHelper::files()
{
    QSettings settings;
    settings.beginGroup("RecentFiles");
    QStringList files = settings.value("recentFileList").toStringList();
    settings.endGroup();
    return files;
}

void RecentFilesHelper::retranslateUi()
{
    clearAct->setText(tr("Clear recent files"));
}
