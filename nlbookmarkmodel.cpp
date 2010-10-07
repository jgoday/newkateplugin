#include "nlbookmarkmodel.h"

#include <QDebug>
#include <QStandardItem>

#include <KBookmarkGroup>
#include <KBookmarkManager>
#include <KIcon>
#include <KStandardDirs>
#include <KUrl>
#include <kio/global.h>

NLBookmarkModel::NLBookmarkModel(QObject *parent) : QStandardItemModel(parent)
{
    const QString &bookmarksFile = KStandardDirs::locateLocal("appdata", "kate.bookmarks");
    m_manager = KBookmarkManager::managerForExternalFile(bookmarksFile);

    initBookmarks();
}

NLBookmarkModel::~NLBookmarkModel()
{
    KBookmarkGroup root = m_manager->root();

    for(int i=0; i<rowCount(); i++) {
        QStandardItem *b = item(i);
        QString url = b->data(Qt::UserRole).toString();

        if (m_bookmarks.contains(url)) {
            KBookmark bookmark = m_manager->findByAddress(m_bookmarks.value(url));
            bookmark.setFullText(b->text());
        }
        else {
            root.addBookmark(b->text(), KUrl(url));
        }
    }

    m_manager->emitChanged(root);
}

void NLBookmarkModel::removeBookmark(const QModelIndex &index)
{
    KBookmarkGroup root = m_manager->root();
    QStandardItem *b = itemFromIndex(index);
    QString url = b->data(Qt::UserRole).toString();

    if (m_bookmarks.contains(url)) {
        KBookmark bookmark = m_manager->findByAddress(m_bookmarks.value(url));
        root.deleteBookmark(bookmark);
        m_manager->emitChanged(root);
    }

    removeRow(index.row());
}

void NLBookmarkModel::addBookmark(const QString &title, const QString &url)
{
    QStandardItem *item = new QStandardItem(title);
    item->setSizeHint(QSize(1, 30));
    item->setData(url, Qt::UserRole);

    KIcon icon(KIO::pixmapForUrl(url, 0, KIconLoader::Panel));

    item->setIcon(icon);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

    appendRow(item);
}

void NLBookmarkModel::initBookmarks()
{
    KBookmarkGroup root = m_manager->root();
    KBookmark bookmark  = root.first();

    while(!bookmark.isNull()) {
        QString url = bookmark.url().pathOrUrl();

        addBookmark(bookmark.text(), url);

        m_bookmarks[url] = bookmark.address();

        bookmark = root.next(bookmark);
    }
}

QStringList NLBookmarkModel::mimeTypes() const
{
    QStringList types;
    types << "text/uri-list";

    return types;
}

