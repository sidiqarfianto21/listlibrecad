QT += widgets gui core
CONFIG += plugin c++17
TEMPLATE = lib

TARGET = propertiessidebar
DESTDIR = release

# DIPAKSA C++17 BIAR std::optional KE-BACA
QMAKE_CXXFLAGS += /std:c++17
QMAKE_CXXFLAGS += /permissive-

# INI YANG BARU, BUAT M_PI / M_PI_2 DI WINDOWS
DEFINES += _USE_MATH_DEFINES
DEFINES += QT_PLUGIN

SOURCES += Properties-Sidebar-Plugin.cpp
HEADERS += Properties-Sidebar-Plugin.h

# Folder dasar LibreCAD
INCLUDEPATH += C:/LibreCAD/librecad/src
INCLUDEPATH += C:/LibreCAD/librecad/src/plugins
INCLUDEPATH += C:/LibreCAD/librecad/src/lib

# Folder dxflib - biarin aja, nggak ngaruh
INCLUDEPATH += C:/dxflib/src
INCLUDEPATH += C:/LibreCAD/libraries/dxflib/src
INCLUDEPATH += C:/LibreCAD/libraries

# Semua folder LibreCAD yang dibuat otomatis sama build.yml
include(librecad_all.pri)
