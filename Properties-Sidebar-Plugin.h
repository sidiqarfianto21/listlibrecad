#pragma once
#include <QObject>
#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>

// NGGAK ADA LAGI #include "lc_plugininterface.h"
// KITA BIKIN SENDIRI INTERFACE-NYA DI SINI, BIAR NGGAK NEBAK-NEBAK FILE LAGI
class LC_PluginInterface
{
public:
    virtual ~LC_PluginInterface() {}
    virtual void init(QMainWindow* mainWindow) = 0;
    virtual void unload() = 0;
};
Q_DECLARE_INTERFACE(LC_PluginInterface, "org.librecad.LC_PluginInterface")

#include "rs_entity.h"
#include "rs_document.h"
#include "rs_graphic.h"

class PropertiesSidebarPlugin : public QObject, public LC_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.librecad.LC_PluginInterface" FILE "properties_sidebar_plugin.json")
    Q_INTERFACES(LC_PluginInterface)

public:
    PropertiesSidebarPlugin() = default;
    ~PropertiesSidebarPlugin() override = default;

    QString getName() const { return QString("PropertiesSidebar"); }
    QString getVersion() const { return QString("1.0"); }
    QString getDescription() const { return QString("Properties Sidebar - Luas, Keliling, Centroid"); }

    void init(QMainWindow* mainWindow) override;
    void unload() override;

private:
    QMainWindow* m_mainWindow = nullptr;
    QDockWidget* m_dock = nullptr;
    QLabel* m_label = nullptr;

    void updateForEntity(RS_Entity* e);
};
