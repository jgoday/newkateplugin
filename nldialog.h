#ifndef NLDIALOG_H
#define NLDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
    class NLDialog;
}

class NLModel;
class KCategorizedSortFilterProxyModel;
class KCategorizedView;

class NLDialog : public QDialog
{
    Q_OBJECT
public:
    NLDialog(NLModel *model, QWidget *parent);
    ~NLDialog();

private slots:
    void slotDocumentClicked(const QModelIndex &index);
    void slotSortBy(int);

private:
    void initView();

private:
    Ui::NLDialog *ui;

    NLModel *m_model;

    KCategorizedView *m_view;
};
#endif
