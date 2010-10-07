#ifndef NLMODEL_H
#define NLMODEL_H

// QT
#include <QMap>
#include <QStandardItemModel>

// KDE
#include <KUrl>

namespace KTextEditor {
    class Document;
}


class NLModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum Roles {
        UUID      = 0x128,
        MIME_TYPE = 0x130,
        FIRST_LETTER = 0x132,
        FOLDER       = 0x134
    };

    NLModel(QObject *parent);
    ~NLModel();

    KUrl urlFromIndex(const QModelIndex &index) const;
    KTextEditor::Document *documentFromIndex(const QModelIndex &index) const;
    KTextEditor::Document *documentFromUUid(const QString &uuid) const;

    virtual QVariant data(const QModelIndex &index, int role) const;;

public slots:
    void addDocument(KTextEditor::Document *);
    void documentChanged(KTextEditor::Document *);
    void removeDocument(KTextEditor::Document *);
    void setCategoryRole(int);

private:
    void setItemDataFromDoc(const QString &, KTextEditor::Document *, QStandardItem *);

private:
    QMap<QString, KTextEditor::Document *> m_documents;
    QMap<QString, QStandardItem *> m_items;
    QMap<KTextEditor::Document *, QString> m_iDocuments;

    int m_categoryRole;
};
#endif

