#include "nlconfigpage.h"

// QT
#include <QLabel>
#include <QVBoxLayout>

NLConfigPage::NLConfigPage(QWidget *parent) : Kate::PluginConfigPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QLabel *label = new QLabel("Hola cacerola", this);
    layout->addWidget(label);
}

NLConfigPage::~NLConfigPage()
{
}

void NLConfigPage::apply()
{
}

void NLConfigPage::defaults()
{
}

void NLConfigPage::reset()
{
}

