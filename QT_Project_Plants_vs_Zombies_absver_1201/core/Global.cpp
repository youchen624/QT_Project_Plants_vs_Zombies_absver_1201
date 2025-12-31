#include "Global.h"
#include <QPixmapCache>
using namespace PVZ::Core;

QPixmap img(const QString &path) {
    QPixmap pix;
    if (!QPixmapCache::find(path, &pix)) {
        if (pix.load(path)) {
            QPixmapCache::insert(path, pix);
        }
    }
    return pix;
};
