#include "nldialog.h"
#include "nlmodel.h"
#include "ui_nldialog.h"

// QT
#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>
// KDE
#include <KCategorizedView>
#include <KCategoryDrawer>
#include <KCategorizedSortFilterProxyModel>
#include <KLocale>
// kate headers
#include <kate/application.h>
#include <kate/mainwindow.h>

class MyCategoryDrawer : public KCategoryDrawerV3
{
public:
    MyCategoryDrawer(KCategorizedView *view) : KCategoryDrawerV3(view)
    {
    }

    virtual int categoryHeight(const QModelIndex &index, const QStyleOption &option) const
    {
        return 40 ;
    }

    virtual void drawCategory(const QModelIndex &index,
                      int sortRole,
                      const QStyleOption &option,
                      QPainter *painter) const
    {
        if (index.isValid())
            KCategoryDrawerV3::drawCategory(index, sortRole, option, painter);
    }

private :
    KCategorizedSortFilterProxyModel *m_model;
};

NLDialog::NLDialog(NLModel *model, QWidget *parent) : QDialog(parent),
    ui(new Ui::NLDialog),
    m_model(model)
{
    ui->setupUi(this);

    //    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setGeometry(parent->geometry());
    setWindowTitle(i18n("Go ..."));

    ui->m_sortBox->addItem("Type", NLModel::MIME_TYPE);
    ui->m_sortBox->addItem("Name", NLModel::FIRST_LETTER);
    ui->m_sortBox->addItem("Folder", NLModel::FOLDER);

    connect(ui->m_sortBox, SIGNAL(activated(int)),
                           SLOT(slotSortBy(int)));
    initView();
}

NLDialog::~NLDialog()
{
    delete ui;
}

void NLDialog::initView()
{
    m_view = new KCategorizedView(this);

    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setViewMode(QListView::IconMode);
    m_view->setSpacing(10);

    // m_view->setCollapsibleBlocks(true);
    ui->gridLayout_2->addWidget(m_view, 1, 0, 1, 3);

    connect(m_view, SIGNAL(clicked(const QModelIndex &)),
                    SLOT(slotDocumentClicked(const QModelIndex &)));
    connect(m_view, SIGNAL(activated(const QModelIndex &)),
                    SLOT(slotDocumentClicked(const QModelIndex &)));


//     connect(m_view, SIGNAL(clicked(const QModelIndex &)),
//                 SLOT(slotItemClicked(const QModelIndex &)));

    KCategorizedSortFilterProxyModel *proxyModel = new KCategorizedSortFilterProxyModel(this);
    proxyModel->setCategorizedModel(true);
    proxyModel->setSourceModel(m_model);
    proxyModel->setSortCategoriesByNaturalComparison(true);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(ui->filter, SIGNAL(textChanged(const QString &)),
            proxyModel, SLOT(setFilterRegExp(const QString &)));

    ui->filter->setFocus();

//     proxyModel->setSortRole(KCategorizedSortFilterProxyModel::CategorySortRole);
//    proxyModel->setSortRole(NLModel::FIRST_LETTER);
    KConfigGroup config(KGlobal::config(), "new_kate_plugin");
    if (config.exists()) {
        int sortBy = config.readEntry("sort_by").toInt();
        m_model->setCategoryRole(sortBy);
        ui->m_sortBox->setCurrentIndex(ui->m_sortBox->findData(sortBy));
    }
    else {
        m_model->setCategoryRole(NLModel::MIME_TYPE);
    }

    m_view->setModel(proxyModel);

    KCategoryDrawer *drawer = new MyCategoryDrawer(m_view);
    m_view->setCategoryDrawer(drawer);

/**
    for(int i=0; i<m_model->rowCount(); i++) {
        QStandardItem *item = m_model->item(0);
        qDebug() << " -> Item = " << item->data(KCategorizedSortFilterProxyModel::CategoryDisplayRole);
    }

    qDebug() << "Sort filter model has " << proxyModel->rowCount();
    qDebug() << "Sort filter model, sort role = " << proxyModel->sortRole();
**/
    proxyModel->sort(0);

//    m_view->reset();
}

void NLDialog::slotDocumentClicked(const QModelIndex &index)
{
    QString uuid = m_view->model()->data(index, NLModel::UUID).toString();

    Kate::application()->activeMainWindow()->activateView(
        m_model->documentFromUUid(uuid)
    );

    accept();
}

void NLDialog::slotSortBy(int index)
{
    int sortBy = ui->m_sortBox->itemData(index).toInt();

    KConfigGroup config(KGlobal::config(), "new_kate_plugin");
    config.writeEntry("sort_by", sortBy);

    m_model->setCategoryRole(sortBy);
    KCategorizedSortFilterProxyModel *proxyModel =
        dynamic_cast <KCategorizedSortFilterProxyModel *>(m_view->model());
    proxyModel->sort(0);
}

