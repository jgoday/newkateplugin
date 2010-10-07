#ifndef NLBOOKMARKMODEL_H
#define NLBOOKMARKMODEL_H

#include <QMap>
#include <QStandardItemModel>

class KBookmarkManager;

class NLBookmarkModel : public QStandardItemModel
{
    Q_OBJECT
public:
    NLBookmarkModel(QObject *parent);
    ~NLBookmarkModel();

public slots:
    void addBookmark(const QString &title, const QString &url);
    void removeBookmark(const QModelIndex &);

protected:
    virtual QStringList mimeTypes() const;

private:
    void initBookmarks();

private:
    QMap<QString, QString> m_bookmarks;
    KBookmarkManager *m_manager;
};

#endif
