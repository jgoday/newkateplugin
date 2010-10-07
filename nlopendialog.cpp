#include "nlopendialog.h"
#include "ui_nlopendialog.h"
#include "nlbookmarkmodel.h"
#include "nlbookmarkview.h"

// Qt headers
#include <QDebug>
#include <QDir>
#include <QDropEvent>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>

// KDE headers
#include <kio/global.h>
#include <KIcon>
#include <KIconLoader>
#include <KLocale>
#include <KUrlNavigator>
#include <KFilePlacesView>
// Kate headers
#include <kate/application.h>
#include <kate/documentmanager.h>
#include <kate/mainwindow.h>

class NLColumnModel : public QStandardItemModel
{
public:
    NLColumnModel(QObject *parent) : QStandardItemModel(parent)
    {
    }

    ~NLColumnModel()
    {
    }


protected:

    QStringList mimeTypes() const
    {
        QStringList types;

        types << "text/uri-list";

        return types;
    }

    QMimeData *mimeData(const QModelIndexList &indexes) const
    {
        qDebug() << "mimeData: " << QStandardItemModel::mimeData(indexes)->formats();

        QMimeData *mimeData = new QMimeData();

        QStringList urls;
        foreach (QModelIndex index, indexes) {
            qDebug() << "mimeData: index = " << index;

            if (index.isValid()) {
                QUrl url(data(index, Qt::UserRole).toString());
                urls << url.toString();
            }
        }

        mimeData->setData("text/uri-list", urls.join(",").toUtf8());

        return mimeData;
    }
};

class NLOpenDialog::Private {
public:
    Private()
    {
    }

private:
};

NLOpenDialog::NLOpenDialog(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::NLOpenDialog),
                                              d(new NLOpenDialog::Private())
{
    ui->setupUi(this);

    setWindowTitle(i18n("Open ..."));
    setGeometry(parent->geometry());

    initView();
}

NLOpenDialog::~NLOpenDialog()
{
    delete ui;
    delete d;
}

void NLOpenDialog::slotLoadFiles(const QModelIndex &index)
{
    const QFileInfo &info = QFileInfo(m_model->data(index, Qt::UserRole).toString());
    if (info.isDir()) {
        loadFiles(info.absoluteFilePath(), index);
    }
    else {
        KTextEditor::Document *doc = Kate::application()->documentManager()->
            openUrl(info.absoluteFilePath());
        Kate::application()->activeMainWindow()->activateView(doc);

        KConfigGroup config(KGlobal::config(), "new_kate_plugin");
        config.writeEntry("last_directory", info.absolutePath());

        accept();
    }
}

void NLOpenDialog::loadParentDirectories(const QString &dirName)
{
    m_model->clear();
    m_items.clear();

    ui->columnView->reset();

    //    QDir dir(dirName);
    const QStringList &paths = dirName.split(QDir::separator());
    QString current = QDir::separator() + paths.at(0);

    if (!m_items.contains(current)) {
        loadFiles(current, m_model->index(0, 0));
    }

    foreach(const QString &p, paths.mid(1)) {
        current = QDir(current).absoluteFilePath(p);

        if (m_items.contains(current)) {
            QModelIndex index = m_items[current]->index();
            loadFiles(current, index);
        }
        current = current;
    }

    if (m_items.contains(current)) {
        QModelIndex index = m_items[current]->index();
        ui->columnView->setCurrentIndex(index);
    }
}

void NLOpenDialog::initView()
{
    m_navigator = new KUrlNavigator(this);
    m_navigator->setShowFullPath(true);
    ui->gridLayout->addWidget(m_navigator, 0, 0, 1, 2);

    NLBookmarkModel *placesModel = new NLBookmarkModel(this);
    NLBookmarkView *bookmarks = new NLBookmarkView(placesModel, this);
    bookmarks->setModel(placesModel);
    bookmarks->setAcceptDrops(true);

    ui->gridLayout->addWidget(bookmarks, 1, 0, 1, 1);

    connect(ui->columnView, SIGNAL(updatePreviewWidget(const QModelIndex &)),
            SLOT(slotLoadFiles(const QModelIndex &)));

    m_model = new NLColumnModel(this);

    ui->columnView->setModel(m_model);
    ui->columnView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->columnView->setSelectionMode(QAbstractItemView::SingleSelection);

    KConfigGroup config(KGlobal::config(), "new_kate_plugin");
    if (config.exists()) {
        loadParentDirectories(config.readEntry("last_directory"));
    }
    else {
        loadFiles(QDir::homePath(), QModelIndex());
    }

    connect(m_navigator, SIGNAL(urlChanged(const KUrl &)),
            SLOT(slotUrlClicked(const KUrl &)));
    connect(bookmarks, SIGNAL(urlClicked(const KUrl &)),
            SLOT(slotUrlClicked(const KUrl &)));
}

void NLOpenDialog::loadFiles(const QString &parent, const QModelIndex &index)
{
    QDir dir(parent);
    m_navigator->setUrl(KUrl(parent));

    foreach(const QString &file,
            dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name)) {

        if (!m_items.contains(dir.absoluteFilePath(file))) {
            QStandardItem *item = new QStandardItem(file);
            item->setData(file, Qt::DisplayRole);
            item->setData(dir.absoluteFilePath(file), Qt::UserRole);

            KIcon icon(KIO::pixmapForUrl(dir.absoluteFilePath(file), 0, KIconLoader::Panel));

            item->setIcon(icon);
            if (index.isValid()) {
                m_model->itemFromIndex(index)->appendRow(item);
            }
            else {
                m_model->appendRow(item);
            }

            m_items[dir.absoluteFilePath(file)] = item;
        }
    }
}

void NLOpenDialog::slotUrlClicked(const KUrl &url)
{
    m_navigator->disconnect();

    loadParentDirectories(url.pathOrUrl());

    if (m_items.contains(url.pathOrUrl())) {
        QModelIndex index = m_items[url.pathOrUrl()]->index();
        ui->columnView->setCurrentIndex(index);
    }

    connect(m_navigator, SIGNAL(urlChanged(const KUrl &)),
            SLOT(slotUrlClicked(const KUrl &)));
}

