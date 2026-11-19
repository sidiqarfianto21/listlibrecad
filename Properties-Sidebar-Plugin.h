#pragma once
#include <QObject>
#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>

// INI YANG BARU, BUKAN QC_PluginInterface LAGI
#include "lc_plugininterface.h"
#include "rs_entity.h"
#include "rs_document.h"
#include "rs_graphic.h"

class PropertiesSidebarPlugin : public QObject, public LC_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID LC_PluginInterface_iid FILE "properties_sidebar_plugin.json")
    Q_INTERFACES(LC_PluginInterface)

public:
    PropertiesSidebarPlugin() = default;
    ~PropertiesSidebarPlugin() override = default;

    // Wajib ada di LC_PluginInterface baru
    QString getName() const override { return QString("PropertiesSidebar"); }
    QString getVersion() const override { return QString("1.0"); }
    QString getDescription() const override { return QString("Properties Sidebar - Luas, Keliling, Centroid"); }

    void init(QMainWindow* mainWindow) override;
    void unload() override;

private:
    QMainWindow* m_mainWindow = nullptr;
    QDockWidget* m_dock = nullptr;
    QLabel* m_label = nullptr;

    void updateForEntity(RS_Entity* e);
};
