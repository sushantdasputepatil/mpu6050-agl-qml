TARGET = mpu-temp
QT = quickcontrols2 qml

SOURCES = main.cpp

CONFIG += link_pkgconfig

RESOURCES += \
    mpu-temp.qrc

include(app.pri)
