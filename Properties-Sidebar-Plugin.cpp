#include "Properties-Sidebar-Plugin.h"
#include <QMainWindow>
#include <QStatusBar>
#include "rs_polyline.h"
#include "rs_line.h"
#include "rs_circle.h"
#include "rs_arc.h"
#include "rs_layer.h" // INI YANG KURANG TADI
#include "rs_vector.h"
#include "rs_entitycontainer.h"

void PropertiesSidebarPlugin::init(QMainWindow* mainWindow)
{
    m_mainWindow = mainWindow;
    if (!m_mainWindow) return;

    m_dock = new QDockWidget(QString("Properties"), m_mainWindow);
    m_label = new QLabel(QString("Pilih polyline tertutup\nuntuk lihat Luas / Keliling / Centroid"), m_dock);
    m_label->setWordWrap(true);
    m_label->setMargin(10);

    auto* w = new QWidget(m_dock);
    auto* lay = new QVBoxLayout(w);
    lay->addWidget(m_label);
    lay->addStretch();
    w->setLayout(lay);
    m_dock->setWidget(w);

    // INI FIX-NYA: addDockWidget itu punya QMainWindow, bukan QWidget
    m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_dock);
}

void PropertiesSidebarPlugin::unload()
{
    if (m_dock) {
        m_dock->deleteLater();
        m_dock = nullptr;
    }
}

void PropertiesSidebarPlugin::updateForEntity(RS_Entity* e)
{
    if (!e || !m_label) return;
    if (e->rtti() != RS2::EntityPolyline) return;

    auto* pl = static_cast<RS_Polyline*>(e);

    // INI FIX getVertices: di 2.2 pakai cara iterasi container
    QList<RS_Vector> pts;
    for (RS_Entity* sub = pl->firstEntity(RS2::ResolveNone);
         sub != nullptr;
         sub = pl->nextEntity(RS2::ResolveNone)) {
        if (sub->rtti() == RS2::EntityLine) {
            auto* line = static_cast<RS_Line*>(sub);
            if (pts.isEmpty()) pts.append(line->getStartpoint());
            pts.append(line->getEndpoint());
        }
    }

    if (pts.size() < 3) return;

    // Shoelace - Luas
    double area2 = 0;
    double cx = 0, cy = 0;
    double perim = 0;
    for (int i = 0; i < pts.size(); i++) {
        RS_Vector p1 = pts.at(i);
        RS_Vector p2 = pts.at((i + 1) % pts.size());
        double cross = p1.x * p2.y - p2.x * p1.y;
        area2 += cross;
        cx += (p1.x + p2.x) * cross;
        cy += (p1.y + p2.y) * cross;
        perim += p1.distanceTo(p2);
    }
    double area = area2 / 2.0;
    if (qFuzzyIsNull(area)) return;
    cx /= (6.0 * area);
    cy /= (6.0 * area);

    QString layerName = QString("?");
    if (e->getLayer()) layerName = e->getLayer()->getName();

    m_label->setText(QString(
        "Layer: %1\n"
        "Titik: %2\n"
        "Luas: %3 m2\n"
        "Keliling: %4 m\n"
        "Centroid: (%5, %6)")
        .arg(layerName)
        .arg(pts.size())
        .arg(qAbs(area), 0, 'f', 2)
        .arg(perim, 0, 'f', 2)
        .arg(cx, 0, 'f', 2)
        .arg(cy, 0, 'f', 2));
}
