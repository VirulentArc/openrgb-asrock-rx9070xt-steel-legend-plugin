TEMPLATE = lib
CONFIG += plugin c++17
QT += core gui widgets

TARGET = OpenRGBASRockRX9070XTPlugin
DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

# Use OPENRGB_ROOT from the environment if set, otherwise default to ~/OpenRGB.
OPENRGB_ROOT = $$(OPENRGB_ROOT)
isEmpty(OPENRGB_ROOT) {
    OPENRGB_ROOT = $$(HOME)/OpenRGB
}

INCLUDEPATH += . \
    $$OPENRGB_ROOT \
    $$OPENRGB_ROOT/RGBController \
    $$OPENRGB_ROOT/i2c_smbus

HEADERS += \
    ASRockRX9070XTPlugin.h \
    ASRockRX9070XTController.h \
    RGBController_ASRockRX9070XT.h \
    $$OPENRGB_ROOT/Colors.h \
    $$OPENRGB_ROOT/OpenRGBPluginInterface.h \
    $$OPENRGB_ROOT/ResourceManagerInterface.h \
    $$OPENRGB_ROOT/RGBController/RGBController.h \
    $$OPENRGB_ROOT/i2c_smbus/i2c_smbus.h

SOURCES += \
    ASRockRX9070XTPlugin.cpp \
    ASRockRX9070XTController.cpp \
    RGBController_ASRockRX9070XT.cpp \
    $$OPENRGB_ROOT/RGBController/RGBController.cpp \
    $$OPENRGB_ROOT/i2c_smbus/i2c_smbus.cpp
