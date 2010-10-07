#ifndef NLCONFIGPAGE_H
#define NLCONFIGPAGE_H

#include <kate/pluginconfigpageinterface.h>

class NLConfigPage : public Kate::PluginConfigPage
{
    Q_OBJECT
public:
    NLConfigPage(QWidget *parent);
    ~NLConfigPage();

    void apply();
    void defaults();
    void reset();

signals:
    void applySettings();
};

#endif

