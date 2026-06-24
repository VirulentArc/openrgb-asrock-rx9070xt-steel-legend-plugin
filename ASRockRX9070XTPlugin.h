#pragma once

#include <vector>

#include <QColor>
#include <QMenu>
#include <QObject>
#include <QWidget>

#include "OpenRGBPluginInterface.h"
#include "ResourceManagerInterface.h"
#include "RGBController_ASRockRX9070XT.h"

class QComboBox;
class QLabel;
class QPushButton;
class QSlider;

class ASRockRX9070XTPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    ASRockRX9070XTPlugin();
    ~ASRockRX9070XTPlugin() override;

    OpenRGBPluginInfo GetPluginInfo() override;
    unsigned int GetPluginAPIVersion() override;

    void Load(ResourceManagerInterface* resource_manager_ptr) override;
    QWidget* GetWidget() override;
    QMenu* GetTrayMenu() override;
    void Unload() override;

private:
    ResourceManagerInterface* resource_manager;
    QWidget* plugin_widget;
    std::vector<RGBController_ASRockRX9070XT*> registered_controllers;

    QColor selected_color;
    QComboBox* target_combo;
    QComboBox* mode_combo;
    QPushButton* color_button;
    QSlider* speed_slider;
    QSlider* brightness_slider;
    QLabel* speed_value_label;
    QLabel* brightness_value_label;
    QLabel* status_label;

    void BuildWidget();
    void PopulateTargets();
    void PopulateModes();
    void UpdateColorButton();
    void UpdateSliderLabels();
    void ApplyPanelSettings();
};
