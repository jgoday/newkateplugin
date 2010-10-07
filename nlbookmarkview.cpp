#include "nlbookmarkview.h"
#include "nlbookmarkmodel.h"

#include <QDropEvent>
#include <QStandardItem>
#include <QUrl>

#include <KIcon>
#include <kio/global.h>
#include <KLocale>
#include <KMenu>
#include <KUrl>

NLBookmarkView::NLBookmarkView(NLBookmarkModel *model, QWidget *parent) : QListView(parent),
                                                                          m_model(model),
                                                                          m_popup(0)
{
    viewport()->setAutoFillBackground(false);
    setFrameStyle(QFrame::NoFrame);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(clicked(const QModelIndex &)),
            SLOT(slotUrlClicked(const QModelIndex &)));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(contextMenuRequested(const QPoint &)));

}

NLBookmarkView::~NLBookmarkView()
{
}

void NLBookmarkView::dropEvent(QDropEvent *event)
{
    KUrl url(event->mimeData()->urls().at(0).toString());
    m_model->addBookmark(url.fileName(), url.pathOrUrl());
}

void NLBookmarkView::contextMenuRequested(const QPoint &p)
{
    if (m_popup) return;

    QModelIndex index = indexAt(p);

    if (!index.isValid()) {
        QList <QModelIndex> selected = selectionModel()->selectedIndexes();
        if (selected.size() > 0) index = selected.at(0);
        else return;
    }

    m_popup = new KMenu(0);
    m_popup->addTitle(model()->data(index, Qt::DisplayRole).toString());
    QAction *deleteAction = m_popup->addAction(KIcon("edit-delete"), i18n("Delete Bookmark"));
    QAction *action = m_popup->exec(mapToGlobal(p));

    if (action == deleteAction) {
        m_model->removeBookmark(index);
    }

    m_popup = 0;
}

void NLBookmarkView::slotUrlClicked(const QModelIndex &index)
{
    QStandardItem *item = m_model->itemFromIndex(index);
    emit urlClicked(KUrl(item->data(Qt::UserRole).toString()));
}


