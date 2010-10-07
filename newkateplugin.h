/* This file is part of the KDE project

   Copyright (C) 2010 Javier Goday <jgoday @ gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef NEWKATEPLUGIN_H
#define NEWKATEPLUGIN_H

#include <QObject>

#include <kxmlguiclient.h>

#include <kate/documentmanager.h>
#include <kate/mainwindow.h>
#include <kate/plugin.h>
#include <kate/pluginconfigpageinterface.h>

namespace KTextEditor {
    class Document;
};
class QLabel;
class KateTabBar;
class NLModel;

class NewKatePlugin : public Kate::Plugin, public Kate::PluginConfigPageInterface
{
    Q_OBJECT
    Q_INTERFACES(Kate::PluginConfigPageInterface)
public:
    explicit NewKatePlugin(QObject *parent = 0, const QList<QVariant> &args= QList<QVariant>());
    virtual ~NewKatePlugin();

    Kate::PluginView *createView(Kate::MainWindow *window);

    uint configPages() const;
    Kate::PluginConfigPage *configPage(uint number = 0, QWidget *parent = 0, const char *name = 0);
    QString configPageName(uint number = 0) const;
    QString configPageFullName(uint number = 0) const;
    KIcon configPageIcon(uint number = 0) const;

    void loadConfig();

private:
    Kate::PluginView *m_view;
};


class NLView : public Kate::PluginView, public Kate::XMLGUIClient
{
    Q_OBJECT
public:
    NLView(Kate::MainWindow *mainWindow);
    ~NLView();

    virtual void readSessionConfig( KConfigBase* config, const QString& groupPrefix );
    virtual void writeSessionConfig( KConfigBase* config, const QString& groupPrefix );

public slots:
    void slotShowBrowseDialog();
    void slotShowOpenDialog();

private:
    bool m_isDialogShowed;

    NLModel *m_model;
};


#endif
