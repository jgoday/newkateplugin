/* This file is part of the KDE project

   Copyright (C) 2010 Javier Goday <jgoday @ gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "newkateplugin.h"
#include "nlconfigpage.h"
#include "nldialog.h"
#include "nlopendialog.h"
#include "nlmodel.h"

// qt headers
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>

// kde headers
#include <KConfigGroup>
#include <KDebug>
#include <KDialog>
#include <KGenericFactory>
#include <KVBox>

// kate headers
#include <kate/application.h>
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>

K_PLUGIN_FACTORY(NewKatePluginFactory, registerPlugin<NewKatePlugin>();)
K_EXPORT_PLUGIN(NewKatePluginFactory(
        KAboutData("newkateplugin","newkateplugin",
                   ki18n("Hello World"), "0.1", ki18n("Example kate plugin"))))


NewKatePlugin::NewKatePlugin(QObject *parent, const QList<QVariant> &args) :
    Kate::Plugin(qobject_cast<Kate::Application *> (parent), "newkateplugin"),
    m_view(0)
{
    Q_UNUSED (args)
    qDebug() << "NEWKATEPLUGIN: Creando plugin";
}

NewKatePlugin::~NewKatePlugin()
{
    qDebug() << "NEWKATEPLUGIN: Borrando plugin";
}


Kate::PluginView *NewKatePlugin::createView(Kate::MainWindow *window)
{
    if (m_view == 0) {
        m_view = new NLView(window);
    }

    qDebug() << "NEWKATEPLUGIN: Creando view ???";

    return m_view;
}

uint NewKatePlugin::configPages() const
{
    return 1;
}

Kate::PluginConfigPage *NewKatePlugin::configPage(uint number, QWidget *parent , const char *name)
{
    Q_UNUSED(number)
    Q_UNUSED(name)

    NLConfigPage *page = new NLConfigPage(parent);

    connect(page, SIGNAL(applySettings()), m_view, SLOT(loadConfig()));

    return page;
}

QString NewKatePlugin::configPageName(uint number) const
{
    Q_UNUSED(number)
    return i18n("New Kate Look");
}

QString NewKatePlugin::configPageFullName(uint number) const
{
    Q_UNUSED(number)
    return i18n("Configure new kate look");
}

KIcon NewKatePlugin::configPageIcon(uint number) const
{
    Q_UNUSED(number)
    return KIcon("tab-duplicate");
}

void NewKatePlugin::loadConfig()
{
    qDebug() << "NEWKATEPLUGIN: Cargando configuracion";
/**
    KateTabsPluginView *view = qobject_cast <KateTabsPluginView *> (m_view);
    view->loadConfig();
**/
}


NLView::NLView(Kate::MainWindow *mainWin) : Kate::PluginView(mainWin),
    Kate::XMLGUIClient(NewKatePluginFactory::componentData()),
    m_isDialogShowed(0)
{
    KAction *a = actionCollection()->addAction("show_nlview");
    a->setText(i18n("Show new browse plugin"));
    a->setShortcut(Qt::META + Qt::Key_B);
    connect(a, SIGNAL(triggered(bool)), this, SLOT(slotShowBrowseDialog()));

    KAction *b = actionCollection()->addAction("show_nlopenview");
    b->setText(i18n("Show new open plugin"));
    b->setShortcut(Qt::META + Qt::Key_O);
    connect(b, SIGNAL(triggered(bool)), this, SLOT(slotShowOpenDialog()));
    mainWindow()->guiFactory()->addClient(this);

    m_model = new NLModel(this);

    foreach(KTextEditor::Document* doc, Kate::application()->documentManager()->documents()) {
        m_model->addDocument(doc);
    }

    connect(Kate::application()->documentManager(), SIGNAL(documentCreated(KTextEditor::Document*)),
            m_model, SLOT(addDocument(KTextEditor::Document*)));
    connect(Kate::application()->documentManager(), SIGNAL(documentWillBeDeleted(KTextEditor::Document*)),
            m_model, SLOT(removeDocument(KTextEditor::Document*)));
}

NLView::~NLView()
{
    delete m_model;
}

void NLView::readSessionConfig( KConfigBase* config, const QString& groupPrefix )
{
  // If you have session-dependant settings, load them here.
  // If you have application wide settings, you have to read your own KConfig,
  // see the Kate::Plugin docs for more information.
  Q_UNUSED( config );
  Q_UNUSED( groupPrefix );
}

void NLView::writeSessionConfig( KConfigBase* config, const QString& groupPrefix )
{
  // If you have session-dependant settings, save them here.
  // If you have application wide settings, you have to create your own KConfig,
  // see the Kate::Plugin docs for more information.
  Q_UNUSED( config );
  Q_UNUSED( groupPrefix );
}

void NLView::slotShowBrowseDialog()
{
    if (!m_isDialogShowed) {
        m_isDialogShowed = true;
        NLDialog dialog(m_model, mainWindow()->window());
        dialog.exec();
        m_isDialogShowed = false;
    }
}

void NLView::slotShowOpenDialog()
{
    if (!m_isDialogShowed) {
        m_isDialogShowed = true;
        NLOpenDialog dialog(mainWindow()->window());
        dialog.exec();
        m_isDialogShowed = false;
    }
}
