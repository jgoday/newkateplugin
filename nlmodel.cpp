#include "nlmodel.h"

//QT
#include <QDebug>
#include <QDir>
#include <QStandardItem>
#include <QUuid>

//KDE
#include <kio/global.h>
#include <KIcon>
#include <KIconLoader>
#include <KTextEditor/Document>
#include <KCategorizedSortFilterProxyModel>

NLModel::NLModel(QObject *parent) : QStandardItemModel(parent),
    m_categoryRole(NLModel::MIME_TYPE)
{
    setColumnCount(1);
}

NLModel::~NLModel()
{
}

void NLModel::documentChanged(KTextEditor::Document *doc)
{
    if (m_iDocuments.contains(doc)) {
        QString uuid = m_iDocuments[doc];
        QStandardItem *item = m_items[uuid];

        qDebug() << "NLModel::addDocument -> old_name = " << item->text() <<
                                               " name = " << doc->documentName() <<
                                           " mimeType = " << doc->mimeType();

        setItemDataFromDoc(uuid, doc, item);
    }
}

void NLModel::addDocument(KTextEditor::Document *doc)
{
    if (!m_iDocuments.contains(doc)) {
        qDebug() << "NLModel::addDocument -> name = " << doc->documentName() <<
                                       " mimeType = " << doc->mimeType();

        QUuid uid = QUuid::createUuid();
        m_iDocuments[doc] = uid.toString();
        m_documents[uid.toString()] = doc;

        // Create qstandardItem
        QStandardItem *item = new QStandardItem(doc->documentName());
        setItemDataFromDoc(uid.toString(), doc, item);

        m_items[uid.toString()] = item;

        appendRow(item);

        connect(doc, SIGNAL(documentNameChanged(KTextEditor::Document *)),
                     SLOT(documentChanged(KTextEditor::Document *)));

        connect(doc, SIGNAL(documentUrlChanged(KTextEditor::Document *)),
                     SLOT(documentChanged(KTextEditor::Document *)));
    }
}

void NLModel::removeDocument(KTextEditor::Document *doc)
{
    if (m_iDocuments.contains(doc)) {
        QString uuid = m_iDocuments[doc];
        QStandardItem *item = m_items[uuid];

        m_documents.remove(uuid);
        m_iDocuments.remove(doc);

        foreach(QStandardItem *i, takeRow(item->row())) {
            delete i;
        }
        item = 0;
    }

    qDebug() << "NLModel::removeDocument -> name = " << doc->documentName() <<
                                      " mimeType = " << doc->mimeType();
}

void NLModel::setCategoryRole(int role)
{
    m_categoryRole = role;
}

void NLModel::setItemDataFromDoc(const QString &uuid, KTextEditor::Document *doc, QStandardItem *item)
{
    KIcon icon(KIO::pixmapForUrl(doc->url(), 0, KIconLoader::Panel));
    item->setIcon(icon);
    item->setData(doc->documentName(), Qt::DisplayRole);
    item->setData(doc->mimeType(), NLModel::MIME_TYPE);
    item->setData(uuid, NLModel::UUID);
    item->setData(doc->documentName().at(0).toUpper(), NLModel::FIRST_LETTER);

    const QString &folder = doc->url().directory();

    if (folder.count(QDir::separator()) > 6) {
        QStringList paths = folder.split(QDir::separator());

        item->setData(QStringList(paths.mid(paths.size() - 6)).join(QDir::separator()), NLModel::FOLDER);
    }
    else {
        item->setData(folder, NLModel::FOLDER);
    }

/**
    item->setData(doc->documentName().at(0).toUpper(), KCategorizedSortFilterProxyModel::CategoryDisplayRole);
    item->setData(doc->documentName().at(0).toUpper(), KCategorizedSortFilterProxyModel::CategorySortRole);
**/
//     const QString &mime = doc->mimeType().toUpper();
// 
//     item->setData(mime, KCategorizedSortFilterProxyModel::CategoryDisplayRole);
//     item->setData(mime, KCategorizedSortFilterProxyModel::CategorySortRole);
}

KUrl NLModel::urlFromIndex(const QModelIndex &index) const
{
    return documentFromIndex(index)->url();
}

KTextEditor::Document *NLModel::documentFromIndex(const QModelIndex &index) const
{
    QStandardItem *item = itemFromIndex(index);
    return m_documents[item->data(NLModel::UUID).toString()];
}

KTextEditor::Document *NLModel::documentFromUUid(const QString &uuid) const
{
    return m_documents[uuid];
}

QVariant NLModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
            return QStandardItemModel::data(index, m_categoryRole);
        case KCategorizedSortFilterProxyModel::CategorySortRole:
            return QStandardItemModel::data(index, m_categoryRole);
        default:
            return QStandardItemModel::data(index, role);
    }
}

