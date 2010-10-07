#include "pluginview.h"

#include <kate/application.h>
#include <ktexteditor/view.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>

NLView::NLView( Kate::MainWindow *mainWin )
    : Kate::PluginView( mainWin ),
    Kate::XMLGUIClient(KAboutData("katehelloworld","katehelloworld",ki18n("Hello World"), "0.1", ki18n("Example kate plugin")))
{
  KAction *a = actionCollection()->addAction( "edit_insert_helloworld" );
  a->setText( i18n("Insert Hello World") );
  connect( a, SIGNAL( triggered(bool) ), this, SLOT( slotInsertHello() ) );

  mainWindow()->guiFactory()->addClient( this );
}

NLView::~NLView()
{
  mainWindow()->guiFactory()->removeClient( this );
}

void NLView::slotInsertHello()
{
  if (!mainWindow()) {
    return;
  }

  KTextEditor::View *kv = mainWindow()->activeView();

  if (kv) {
    kv->insertText( "Hello World" );
  }
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

