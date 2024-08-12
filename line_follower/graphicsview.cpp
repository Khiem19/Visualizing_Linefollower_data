
#include "graphicsview.h"


graphicsview::graphicsview(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant graphicsview::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex graphicsview::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex graphicsview::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int graphicsview::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int graphicsview::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant graphicsview::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

