#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T23:43:48
#
#-------------------------------------------------

QT += core gui
QT += serialport bluetooth


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = selftest_ble
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


DEFINES += CONFIG_CTRL_IFACE
DEFINES += CONFIG_CTRL_IFACE_UNIX


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainwindow_ui_impl.cpp \
    plat/plat_event.cpp \
    plat/plat_signal.cpp \
    plat/plat_thread.cpp \
    plat/plat.cpp \
    apprun.cpp \
    plat/libbb_stub.cpp \
    hw_gpio.c \
    G4BLEController.cpp

HEADERS += \
    mainwindow.h \
    mainwindow_ui_impl.h \
    config.h \
    plat/libbb_stub.h \
    plat/plat_event.h \
    plat/plat_signal.h \
    plat/plat_thread.h \
    plat/plat.h \
    apprun.h \
    hw_gpio.h \
    G4BLEController.h


DEFINES += _GNU_SOURCE _LARGEFILE_SOURCE _LARGEFILE64_SOURCE _FILE_OFFSET_BITS=64


# glib and gstreamer
INCLUDEPATH += \
    "/opt/platform/MX6ULL_H35/fsl-release-bsp/build-x11/tmp/sysroots/imx6ull14x14evk/usr/include/glib-2.0" \
    "/opt/platform/MX6ULL_H35/fsl-release-bsp/build-x11/tmp/sysroots/imx6ull14x14evk/usr/lib/glib-2.0/include" \
    "/opt/platform/MX6ULL_H35/fsl-release-bsp/build-x11/tmp/sysroots/imx6ull14x14evk/usr/include/gstreamer-1.0" \
    "/opt/platform/MX6ULL_H35/fsl-release-bsp/build-x11/tmp/sysroots/imx6ull14x14evk/usr/lib/gstreamer-1.0/include" \
    "/opt/platform/MX6ULL_H35/fsl-release-bsp/build-x11/tmp/sysroots/imx6ull14x14evk/usr/include/gdk-pixbuf-2.0"



FORMS += \
  mainwindow.ui

LIBS += \
  -lresolv \
  -lpthread -ludev

LIBS += \
  -lX11 \
  -lXtst \
  -lXext

LIBS += \
  -lgstreamer-1.0 \
  -lgstbase-1.0 \
  -lgstvideo-1.0 \
  -lgstapp-1.0 \
  -lgobject-2.0 \
  -lglib-2.0 \
  -ljpeg \
  -lgdk_pixbuf-2.0



target.path = /opt/selftest_ble/bin
INSTALLS += target

RESOURCES += \
    resource.qrc
