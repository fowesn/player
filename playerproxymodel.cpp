#include "playerproxymodel.h"

#include "playercontrols.h"
#include "playlistmodel.h"
#include "histogramwidget.h"
#include "videowidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QList>
#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QStringListModel>
#include <QMimeData>
#include <QDataStream>

playerproxymodel::playerproxymodel(QObject* parent) : QSortFilterProxyModel (parent)
{

}
