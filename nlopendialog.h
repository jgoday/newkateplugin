#ifndef NLOPENDIALOG_H
#define NLOPENDIALOG_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>

#include <KUrl>

namespace Ui {
    class NLOpenDialog;
}
class QStandardItem;
class QStandardItemModel;
class KUrlNavigator;

class NLOpenDialog : public QDialog
{
    Q_OBJECT
public:
    NLOpenDialog(QWidget *parent);
    ~NLOpenDialog();

private slots:
    void slotLoadFiles(const QModelIndex &);
    void slotUrlClicked(const KUrl &);

private:
    void loadParentDirectories(const QString &dir);

    void initView();
    void loadFiles(const QString &parent, const QModelIndex &index);

private:
    Ui::NLOpenDialog *ui;

    QMap <QString, QStandardItem *> m_items;
    QStandardItemModel *m_model;
    KUrlNavigator *m_navigator;

    class Private;
    Private *d;
};
#endif
