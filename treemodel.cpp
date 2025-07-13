// Copyright (C) 2025 Pedro López-Cabanillas
// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QStringList>
#include <QVariantList>

#include "riff.h"
#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , rootItem(std::make_unique<TreeItem>(QVariantList{tr("Chunk"), tr("Offset"), tr("Size")}))
{}

TreeModel::~TreeModel() = default;

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

bool TreeModel::loadData(QFile *riffFile)
{
    riff::RiffChunk<> *chunk = reinterpret_cast<riff::RiffChunk<> *>(m_buffer);
    if (!chunk->hasTypeRiff()) {
        QMessageBox::warning(qApp->activeWindow(),
                             qApp->applicationName(),
                             tr("%1 is not a valid RIFF file").arg(riffFile->fileName()));
        return false;
    }

    beginResetModel();
    traverseRiff(chunk->castTo<riff::RiffList<> >(), rootItem.get());
    endResetModel();

    return true;
}

void TreeModel::traverseRiff(const riff::RiffList<>::Chunk *listChunk, TreeItem *lastParent)
{
    qintptr lastPos = listChunk->offset(m_buffer);
    lastParent->appendChild(
        std::make_unique<TreeItem>(QVariantList{QString("%1(%2)")
                                                    .arg(listChunk->typeToQString())
                                                    .arg(listChunk->data->listTypeToQString()),
                                                lastPos,
                                                listChunk->size},
                                   lastParent));

    lastParent = lastParent->child(lastParent->childCount() - 1);

    const riff::RiffChunk<> *child = listChunk->data->chunks;
    const void *end = listChunk->dataEnd();
    while (child < end) {
        if (child->hasTypeList() || child->hasTypeRiff()) {
            traverseRiff(child->castTo<riff::RiffList<> >(), lastParent);
        } else {
            lastPos = child->offset(m_buffer);
            lastParent->appendChild(std::make_unique<TreeItem>(QVariantList{child->typeToQString(),
                                                                            lastPos,
                                                                            child->size},
                                                               lastParent));
        }
        child = child->nextChunk();
    }
}

uint8_t *TreeModel::buffer() const
{
    return m_buffer;
}

void TreeModel::setBuffer(uint8_t *newBuffer)
{
    m_buffer = newBuffer;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto *item = static_cast<const TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    return index.isValid()
        ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    return orientation == Qt::Horizontal && role == Qt::DisplayRole
        ? rootItem->data(section) : QVariant{};
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    TreeItem *parentItem = parent.isValid()
        ? static_cast<TreeItem*>(parent.internalPointer())
        : rootItem.get();

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    return parentItem != rootItem.get()
        ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const TreeItem *parentItem = parent.isValid()
        ? static_cast<const TreeItem*>(parent.internalPointer())
        : rootItem.get();

    return parentItem->childCount();
}
