#pragma once
#include <QObject>
#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "lc_plugininterface.h"
#include "rs_entity.h"
#include "rs_document.h"
#include "rs_graphic.h"

class PropertiesSidebarPlugin : public QObject, public LC_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.librecad.LC_PluginInterface" FILE "properties_sidebar_plugin.json")
    // Q_INTERFACES DIBUANG, INI YANG BIKIN ERROR TADI

public:
    PropertiesSidebarPlugin() = default;
    ~PropertiesSidebarPlugin() override = default;

    // Kita sediain 2 versi nama, biar nggak salah lagi
    QString getName() const { return QString("PropertiesSidebar"); }
    QString getVersion() const { return QString("1.0"); }
    QString getDescription() const { return QString("Properties Sidebar - Luas, Keliling, Centroid"); }

    QString getPluginName() const { return QString("PropertiesSidebar"); }
    QString getPluginVersion() const { return QString("1.0"); }
    QString getPluginDescription() const { return QString("Properties Sidebar - Luas, Keliling, Centroid"); }

    void init(QMainWindow* mainWindow) override;
    void unload() override;

private:
    QMainWindow* m_mainWindow = nullptr;
    QDockWidget* m_dock = nullptr;
    QLabel* m_label = nullptr;

    void updateForEntity(RS_Entity* e);
};
