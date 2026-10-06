// Copyright (C) 2025-2026 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RECENTFILESHELPER_H
#define RECENTFILESHELPER_H

#include <QObject>
#include <QAction>
#include <QString>

class RecentFilesHelper : public QObject
{
	Q_OBJECT
public:
	RecentFilesHelper(QObject *parent);
	~RecentFilesHelper() {}

	void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    QStringList files();
    void retranslateUi();

Q_SIGNALS:
	void selectedFile(QString fileName);

private Q_SLOTS:
	void openRecentFile();
	void clear();

private:
	static const int MaxRecentFiles = 10;
    QAction *recentFileActs[MaxRecentFiles];
	QAction *clearAct;
};

#endif // RECENTFILESHELPER_H
