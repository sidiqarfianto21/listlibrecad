
// LibreCAD Plugin: Properties Sidebar + Raster Adjust
#ifndef PROPERTIES_SIDEBAR_PLUGIN_H
#define PROPERTIES_SIDEBAR_PLUGIN_H
#include <QObject>
#include <QDockWidget>
#include <QLabel>
#include <QSlider>
#include "qc_plugininterface.h"
class PropertiesSidebarPlugin : public QObject, public QC_PluginInterface {
    Q_OBJECT
    Q_INTERFACES(QC_PluginInterface)
public:
    PropertiesSidebarPlugin();
    virtual ~PropertiesSidebarPlugin();
    virtual PluginCapabilities getCapabilities() const;
    virtual QString name() const;
    virtual void execCommmandLine(int cmd);
    virtual void execAction(void* doc, void* view, int action);
public slots:
    void onSelectionChanged();
    void onTransparencyChanged(int value);
    void onFadeChanged(int value);
private:
    QDockWidget* dockProps = nullptr;
    QDockWidget* dockRaster = nullptr;
    QLabel* lblLuas = nullptr;
    QLabel* lblKeliling = nullptr;
    QLabel* lblCentroid = nullptr;
    QLabel* lblTitik = nullptr;
    QLabel* lblLayer = nullptr;
    QLabel* lblInfo = nullptr;
    QSlider* slTransparency = nullptr;
    QSlider* slFade = nullptr;
    void* currentDoc = nullptr;
    void* currentView = nullptr;
    void createPropsDock(QWidget* mainWin);
    void createRasterDock(QWidget* mainWin);
    void updatePropsForSelected();
    void applyRasterToSelected();
};
#endif
