#ifndef NLBOOKMARKVIEW_H
#define NLBOOKMARKVIEW_H

#include <QListView>
#include <KUrl>

class NLBookmarkModel;
class KMenu;

class NLBookmarkView : public QListView
{
    Q_OBJECT
public:
    NLBookmarkView(NLBookmarkModel *model, QWidget *parent);
    ~NLBookmarkView();

signals:
    void urlClicked(const KUrl &);

protected:
    virtual void dropEvent(QDropEvent *event);

private slots:
    void contextMenuRequested(const QPoint &p);
    void slotUrlClicked(const QModelIndex &);

private:
    NLBookmarkModel *m_model;
    KMenu *m_popup;
};

#endif
