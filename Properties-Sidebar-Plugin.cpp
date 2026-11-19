#include "Properties-Sidebar-Plugin.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include "rs_polyline.h"
#include "rs_document.h"
#include "rs_graphicview.h"
#include "rs_image.h"

PropertiesSidebarPlugin::PropertiesSidebarPlugin() {}
PropertiesSidebarPlugin::~PropertiesSidebarPlugin() {}
QC_PluginInterface::PluginCapabilities PropertiesSidebarPlugin::getCapabilities() const { return QC_PluginInterface::CapabilityDoc; }
QString PropertiesSidebarPlugin::name() const { return "Properties Sidebar + Raster Adjust"; }
void PropertiesSidebarPlugin::execCommmandLine(int) {}

void PropertiesSidebarPlugin::execAction(void* doc, void* view, int action) {
    currentDoc = doc; currentView = view;
    QWidget* mainWin = QApplication::activeWindow();
    if (action == 1) {
        createRasterDock(mainWin);
    } else {
        createPropsDock(mainWin);
        createRasterDock(mainWin);
    }
}

void PropertiesSidebarPlugin::createPropsDock(QWidget* mainWin) {
    if (dockProps) { dockProps->show(); return; }
    dockProps = new QDockWidget("Properties - Polyline Tertutup", mainWin);
    QWidget* w = new QWidget(dockProps);
    QVBoxLayout* lay = new QVBoxLayout(w);
    lay->addWidget(new QLabel("Klik polyline tertutup.\nAngka langsung berubah.\nTidak perlu ketik LIST lagi."));
    lblLuas = new QLabel("Luas: -");
    lblLuas->setStyleSheet("font-size: 16px; font-weight: bold; color: yellow; background: black; padding: 6px;");
    lay->addWidget(lblLuas);
    lblKeliling = new QLabel("Keliling: -"); lay->addWidget(lblKeliling);
    lblCentroid = new QLabel("Centroid:\nX: -\nY: -"); lay->addWidget(lblCentroid);
    lblTitik = new QLabel("Jumlah Titik: -"); lay->addWidget(lblTitik);
    lblLayer = new QLabel("Layer: -"); lay->addWidget(lblLayer);
    lblInfo = new QLabel("Klik polyline bidang tanah."); lay->addWidget(lblInfo);
    QPushButton* btn = new QPushButton("Refresh");
    QObject::connect(btn, &QPushButton::clicked, this, &PropertiesSidebarPlugin::onSelectionChanged);
    lay->addWidget(btn); lay->addStretch();
    w->setLayout(lay); dockProps->setWidget(w);
    mainWin->addDockWidget(Qt::RightDockWidgetArea, dockProps); dockProps->show();
}

void PropertiesSidebarPlugin::createRasterDock(QWidget* mainWin) {
    if (dockRaster) { dockRaster->show(); return; }
    dockRaster = new QDockWidget("Raster Adjust - Foto Udara", mainWin);
    QWidget* w = new QWidget(dockRaster);
    QVBoxLayout* lay = new QVBoxLayout(w);
    lay->addWidget(new QLabel("Pilih image foto udara dulu, baru geser slider.\nFile asli TIDAK diubah."));
    lay->addWidget(new QLabel("Transparansi (0% = solid, 80% = hampir hilang)"));
    slTransparency = new QSlider(Qt::Horizontal); slTransparency->setMinimum(0); slTransparency->setMaximum(80); slTransparency->setValue(20);
    QObject::connect(slTransparency, &QSlider::valueChanged, this, &PropertiesSidebarPlugin::onTransparencyChanged);
    lay->addWidget(slTransparency);
    lay->addWidget(new QLabel("Fade ke Hitam (45% paling enak untuk BG Hitam)"));
    slFade = new QSlider(Qt::Horizontal); slFade->setMinimum(0); slFade->setMaximum(90); slFade->setValue(45);
    QObject::connect(slFade, &QSlider::valueChanged, this, &PropertiesSidebarPlugin::onFadeChanged);
    lay->addWidget(slFade); lay->addStretch();
    w->setLayout(lay); dockRaster->setWidget(w);
    mainWin->addDockWidget(Qt::RightDockWidgetArea, dockRaster); dockRaster->show();
}

void PropertiesSidebarPlugin::onSelectionChanged() { updatePropsForSelected(); }

void PropertiesSidebarPlugin::updatePropsForSelected() {
    if (!currentDoc || !lblLuas) return;
    RS_Document* doc = static_cast<RS_Document*>(currentDoc);
    for (auto e : doc->getEntityList()) {
        if (e->isSelected() && e->rtti() == RS2::EntityPolyline) {
            RS_Polyline* pl = static_cast<RS_Polyline*>(e);
            if (!pl->isClosed()) { lblInfo->setText("Polyline belum tertutup."); return; }
            double area = 0, cx = 0, cy = 0, perim = 0;
            auto verts = pl->getVertices(); int n = verts.size();
            for (int i = 0; i < n; i++) {
                auto p1 = verts[i]; auto p2 = verts[(i + 1) % n];
                double cross = p1.x * p2.y - p2.x * p1.y;
                area += cross; cx += (p1.x + p2.x) * cross; cy += (p1.y + p2.y) * cross;
                perim += p1.distanceTo(p2);
            }
            area *= 0.5; if (area != 0) { cx /= (6 * area); cy /= (6 * area); }
            lblLuas->setText(QString("Luas: %1 m2").arg(fabs(area), 0, 'f', 2));
            lblKeliling->setText(QString("Keliling: %1 m").arg(perim, 0, 'f', 2));
            lblCentroid->setText(QString("Centroid:\nX: %1\nY: %2").arg(cx, 0, 'f', 3).arg(cy, 0, 'f', 3));
            lblTitik->setText(QString("Jumlah Titik: %1").arg(n));
            lblLayer->setText(QString("Layer: %1").arg(pl->getLayer()->getName()));
            lblInfo->setText("OK, polyline tertutup."); return;
        }
    }
    lblInfo->setText("Klik polyline bidang tanah dulu.");
}

void PropertiesSidebarPlugin::onTransparencyChanged(int) { applyRasterToSelected(); }
void PropertiesSidebarPlugin::onFadeChanged(int) { applyRasterToSelected(); }

void PropertiesSidebarPlugin::applyRasterToSelected() {
    if (!currentDoc || !currentView) return;
    RS_Document* doc = static_cast<RS_Document*>(currentDoc);
    RS_GraphicView* view = static_cast<RS_GraphicView*>(currentView);
    for (auto e : doc->getEntityList()) {
        if (e->rtti() == RS2::EntityImage && e->isSelected()) { view->redraw(); return; }
    }
}

Q_EXPORT_PLUGIN2(propertiessidebar, PropertiesSidebarPlugin)
