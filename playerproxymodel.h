#ifndef PLAYERPROXYMODEL_H
#define PLAYERPROXYMODEL_H

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>

class playerproxymodel : public QSortFilterProxyModel
{
public:
    playerproxymodel(QObject *parent = nullptr);
};

#endif // PLAYERPROXYMODEL_H
