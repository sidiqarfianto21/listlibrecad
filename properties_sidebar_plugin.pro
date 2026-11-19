QT += widgets gui core
CONFIG += plugin c++17
TEMPLATE = lib

TARGET = propertiessidebar
DESTDIR = release

# INI YANG BARU, DIPAKSA C++17 BIAR std::optional KE-BACA
QMAKE_CXXFLAGS += /std:c++17
QMAKE_CXXFLAGS += /permissive-

SOURCES += Properties-Sidebar-Plugin.cpp
HEADERS += Properties-Sidebar-Plugin.h

# Folder dasar LibreCAD
INCLUDEPATH += C:/LibreCAD/librecad/src
INCLUDEPATH += C:/LibreCAD/librecad/src/plugins
INCLUDEPATH += C:/LibreCAD/librecad/src/lib

# Folder dxflib - INI YANG BARU, BUAT drw_classes.h
INCLUDEPATH += C:/LibreCAD/libraries/dxflib/src
INCLUDEPATH += C:/LibreCAD/libraries

# Semua folder LibreCAD yang dibuat otomatis sama build.yml
include(librecad_all.pri)

DEFINES += QT_PLUGIN
