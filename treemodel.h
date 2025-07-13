// Copyright (C) 2025 Pedro López-Cabanillas
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QModelIndex>
#include <QVariant>
#include <memory>

#include "riff.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(TreeModel)

    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    bool loadData(QFile *riffFile);

private:
    void traverseRiff(const riff::RiffList<>::Chunk *listChunk, TreeItem *parent);
    uint8_t *m_buffer{nullptr};

    std::unique_ptr<TreeItem> rootItem;
};

#endif // TREEMODEL_H
