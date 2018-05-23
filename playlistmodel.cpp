/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "playlistmodel.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QStringListModel>
#include <QMimeData>
#include <QDataStream>
#include <QtDebug>
#include <algorithm>
#include <QMessageLogger>
#include <QList>
#include <QVector>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

PlaylistModel::~PlaylistModel()
{
}

/*Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return Qt::MoveAction;
}*/

/*Qt::DropActions PlaylistModel::supportedDragActions() const
{
    return Qt::MoveAction;
}*/

/*Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    QStringListModel *qstrmodel = new QStringListModel();

    Qt::ItemFlags defaultFlags = qstrmodel->flags(index);

     if (index.isValid())
         return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
     else
         return Qt::ItemIsDropEnabled | defaultFlags;
 }*/

/*bool PlaylistModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount() || parent.isValid())
        return false;
    beginInsertRows(parent, row, row + count - 1);
    for(int index = row, end = row + count; index < end; ++index)
        //m_data.insert(index, p(KnownField::Invalid, false));
    endInsertRows();
    return true;
}*/

/*bool PlaylistModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || (row + count) > rowCount() || parent.isValid())
        return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int index = row, end = row + count; index < end; ++index)
        //m_data.removeAt(index);
    endRemoveRows();
    return true;
}*/


/*Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}*/

/*bool PlaylistModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         m_playlist.insert(position, "");
     }

     endInsertRows();
     return true;
 }

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         m_playlist.removeAt(position);
     }

     endRemoveRows();
     return true;
 }*/

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList PlaylistModel::mimeTypes() const
{
    QStringList types;
    types << "application/x-qabstractitemmodeldatalist";
    return types;
}

Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

/*QMimeData *PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            //QString text = data(index, Qt::DisplayRole).toString();
            //QString text = index.row();
            stream << index.row();
        }
    }

    mimeData->setData("application/x-qabstractitemmodeldatalist", encodedData);
    return mimeData;
}*/

bool PlaylistModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    int beginRow;

      if (row != -1)
          beginRow = row;

      else if (parent.isValid())
            beginRow = parent.row();

      else
            beginRow = rowCount(QModelIndex());

      QByteArray encodedData = data->data("application/x-qabstractitemmodeldatalist");
      QDataStream stream(&encodedData, QIODevice::ReadOnly);
          /*QStringList newItems;
          int rows = 0;

          while (!stream.atEnd()) {
              QString text;
              stream >> text;
              newItems << text;
              ++rows;
          }*/
          int oneId;
          QMap<int,  QVariant> roleDataMap;
          QList <int> id;
          int col;
          while (!stream.atEnd()) {
          stream >> oneId >> col >> roleDataMap;
          id.append(oneId);
          }

          qSort(id.begin(), id.end(), qGreater<int>());
          int j = 0;
          for (auto &k: id ) {
              if (k <= beginRow) {
                beginRow--;
                QMediaContent m = m_playlist->media(k);
                m_playlist->removeMedia(k);
                m_playlist->insertMedia(beginRow, m);
              } else {
                QMediaContent m = m_playlist->media(k+j);
                m_playlist->removeMedia(k+j);
                m_playlist->insertMedia(beginRow, m);
              }
          if (k > beginRow) {j++;}
          }
          //++beginRow;




           /*foreach (const QString &text, newItems) {
               QModelIndex idx = index(beginRow, 0, QModelIndex());
               setData(idx, text);
               beginRow++;
           }*/

           return true;
}

bool PlaylistModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    m_playlist->moveMedia(position, rows);
    endInsertRows();
    return true;
}


int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid()
            && row >= 0 && row < m_playlist->mediaCount()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant value = m_data[index];
        if (!value.isValid() && index.column() == Title) {
            QUrl location = m_playlist->media(index.row()).canonicalUrl();
            return QFileInfo(location.path()).fileName();
        }

        return value;
    }
    return QVariant();
}

QMediaPlaylist *PlaylistModel::playlist() const
{
    return m_playlist.data();
}

void PlaylistModel::setPlaylist(QMediaPlaylist *playlist)
{
    if (m_playlist) {
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);
    }

    beginResetModel();
    m_playlist.reset(playlist);

    if (m_playlist) {
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);
    }

    endResetModel();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::beginInsertItems(int start, int end)
{
    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void PlaylistModel::endInsertItems()
{
    endInsertRows();
}

void PlaylistModel::beginRemoveItems(int start, int end)
{
    m_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistModel::endRemoveItems()
{
    endRemoveRows();
}

void PlaylistModel::changeItems(int start, int end)
{
    m_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}
