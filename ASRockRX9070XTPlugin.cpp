#include "ASRockRX9070XTPlugin.h"
#include "ASRockRX9070XTController.h"

#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QVariant>

namespace
{
    QString HexByte(int value)
    {
        return QString("0x%1").arg(value, 2, 16, QLatin1Char('0')).toUpper();
    }
}

ASRockRX9070XTPlugin::ASRockRX9070XTPlugin()
    : resource_manager(nullptr),
      plugin_widget(nullptr),
      selected_color(255, 255, 255),
      target_combo(nullptr),
      mode_combo(nullptr),
      color_button(nullptr),
      speed_slider(nullptr),
      brightness_slider(nullptr),
      speed_value_label(nullptr),
      brightness_value_label(nullptr),
      status_label(nullptr)
{
}

ASRockRX9070XTPlugin::~ASRockRX9070XTPlugin()
{
    Unload();
}

OpenRGBPluginInfo ASRockRX9070XTPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    info.Name          = "ASRock RX 9070 XT Steel Legend";
    info.Description   = "Adds OpenRGB control for the ASRock RX 9070 XT Steel Legend GPU RGB controller.";
    info.Version       = "0.1.2";
    info.Commit        = "development";
    info.URL           = "https://github.com/VirulentArc/asrock-rx9070xt-rgb";
    info.Location      = OPENRGB_PLUGIN_LOCATION_SETTINGS;
    info.Label         = "ASRock GPU RGB";
    info.TabIconString = "";
    return info;
}

unsigned int ASRockRX9070XTPlugin::GetPluginAPIVersion()
{
    return OPENRGB_PLUGIN_API_VERSION;
}

void ASRockRX9070XTPlugin::Load(ResourceManagerInterface* resource_manager_ptr)
{
    resource_manager = resource_manager_ptr;

    if(resource_manager == nullptr)
    {
        return;
    }

    auto& buses = resource_manager->GetI2CBusses();

    // v0.1 is intentionally conservative: only use the known-good bus 7.
    // This avoids probing random I2C devices while we prove the OpenRGB controller path.
    for(i2c_smbus_interface* bus : buses)
    {
        if(bus == nullptr)
        {
            continue;
        }

        if(bus->bus_id != 7)
        {
            continue;
        }

        ASRockRX9070XTController* hardware_controller = new ASRockRX9070XTController(bus);
        RGBController_ASRockRX9070XT* rgb_controller = new RGBController_ASRockRX9070XT(hardware_controller);

        resource_manager->RegisterRGBController(rgb_controller);
        registered_controllers.push_back(rgb_controller);
        break;
    }
}

QWidget* ASRockRX9070XTPlugin::GetWidget()
{
    if(plugin_widget == nullptr)
    {
        BuildWidget();
    }

    return plugin_widget;
}

QMenu* ASRockRX9070XTPlugin::GetTrayMenu()
{
    return nullptr;
}

void ASRockRX9070XTPlugin::Unload()
{
    if(resource_manager != nullptr)
    {
        for(RGBController_ASRockRX9070XT* controller : registered_controllers)
        {
            resource_manager->UnregisterRGBController(controller);
            delete controller;
        }
    }

    registered_controllers.clear();
    resource_manager = nullptr;

    delete plugin_widget;
    plugin_widget = nullptr;

    target_combo = nullptr;
    mode_combo = nullptr;
    color_button = nullptr;
    speed_slider = nullptr;
    brightness_slider = nullptr;
    speed_value_label = nullptr;
    brightness_value_label = nullptr;
    status_label = nullptr;
}

void ASRockRX9070XTPlugin::BuildWidget()
{
    plugin_widget = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout(plugin_widget);

    QLabel* title = new QLabel("ASRock RX 9070 XT Steel Legend RGB");
    QFont title_font = title->font();
    title_font.setBold(true);
    title->setFont(title_font);
    layout->addWidget(title);

    QLabel* help = new QLabel(
        "Use this panel for per-channel hardware modes. OpenRGB's normal Mode dropdown is controller-wide, "
        "so it may reset the Zone/LED selection. This panel writes directly to the selected GPU channel.");
    help->setWordWrap(true);
    layout->addWidget(help);

    QFormLayout* form = new QFormLayout();

    target_combo = new QComboBox();
    PopulateTargets();
    form->addRow("Target", target_combo);

    mode_combo = new QComboBox();
    PopulateModes();
    form->addRow("Mode", mode_combo);

    color_button = new QPushButton();
    UpdateColorButton();
    connect(color_button, &QPushButton::clicked, this, [this]()
    {
        QColor color = QColorDialog::getColor(selected_color, plugin_widget, "Choose ASRock GPU RGB color");
        if(color.isValid())
        {
            selected_color = color;
            UpdateColorButton();
        }
    });
    form->addRow("Color", color_button);

    speed_slider = new QSlider(Qt::Horizontal);
    speed_slider->setRange(0x20, 0xFF);
    speed_slider->setValue(0x80);
    speed_value_label = new QLabel();
    QHBoxLayout* speed_layout = new QHBoxLayout();
    speed_layout->addWidget(speed_slider);
    speed_layout->addWidget(speed_value_label);
    QWidget* speed_widget = new QWidget();
    speed_widget->setLayout(speed_layout);
    form->addRow("Speed", speed_widget);

    brightness_slider = new QSlider(Qt::Horizontal);
    brightness_slider->setRange(0x20, 0xFF);
    brightness_slider->setValue(0xFF);
    brightness_value_label = new QLabel();
    QHBoxLayout* brightness_layout = new QHBoxLayout();
    brightness_layout->addWidget(brightness_slider);
    brightness_layout->addWidget(brightness_value_label);
    QWidget* brightness_widget = new QWidget();
    brightness_widget->setLayout(brightness_layout);
    form->addRow("Brightness", brightness_widget);

    connect(speed_slider, &QSlider::valueChanged, this, [this]() { UpdateSliderLabels(); });
    connect(brightness_slider, &QSlider::valueChanged, this, [this]() { UpdateSliderLabels(); });
    UpdateSliderLabels();

    layout->addLayout(form);

    QPushButton* apply_button = new QPushButton("Apply to selected target");
    connect(apply_button, &QPushButton::clicked, this, [this]() { ApplyPanelSettings(); });
    layout->addWidget(apply_button);

    status_label = new QLabel();
    status_label->setWordWrap(true);
    layout->addWidget(status_label);

    layout->addStretch();
}

void ASRockRX9070XTPlugin::PopulateTargets()
{
    target_combo->clear();
    target_combo->addItem("All GPU RGB zones", -1);

    if(!registered_controllers.empty())
    {
        RGBController_ASRockRX9070XT* controller = registered_controllers.front();
        for(unsigned int index = 0; index < controller->GetHardwareChannelCount(); index++)
        {
            target_combo->addItem(QString::fromStdString(controller->GetHardwareChannelName(index)), static_cast<int>(index));
        }
    }
    else
    {
        target_combo->addItem("ARGB Header", 0);
        target_combo->addItem("Top / Side", 1);
        target_combo->addItem("Fan", 2);
    }
}

void ASRockRX9070XTPlugin::PopulateModes()
{
    mode_combo->clear();
    mode_combo->addItem("Static",         0x01);
    mode_combo->addItem("Breathing",      0x02);
    mode_combo->addItem("Strobe",         0x03);
    mode_combo->addItem("RGB Cycle",      0x04);
    mode_combo->addItem("Random",         0x05);
    mode_combo->addItem("Color Shift",    0x07);
    mode_combo->addItem("Visor",          0x08);
    mode_combo->addItem("Stacking",       0x09);
    mode_combo->addItem("Fill Wave",      0x0A);
    mode_combo->addItem("Traveling Wave", 0x0B);
    mode_combo->addItem("Marquee Color",  0x0C);
    mode_combo->addItem("Marquee Random", 0x0D);
    mode_combo->addItem("Color Wave",     0x0E);
    mode_combo->addItem("Rainbow",        0x0F);
}

void ASRockRX9070XTPlugin::UpdateColorButton()
{
    if(color_button == nullptr)
    {
        return;
    }

    const QString text = QString("#%1%2%3")
        .arg(selected_color.red(),   2, 16, QLatin1Char('0'))
        .arg(selected_color.green(), 2, 16, QLatin1Char('0'))
        .arg(selected_color.blue(),  2, 16, QLatin1Char('0'))
        .toUpper();

    color_button->setText(text);
    color_button->setStyleSheet(QString("background-color: %1;").arg(text));
}

void ASRockRX9070XTPlugin::UpdateSliderLabels()
{
    if(speed_value_label != nullptr && speed_slider != nullptr)
    {
        speed_value_label->setText(HexByte(speed_slider->value()));
    }

    if(brightness_value_label != nullptr && brightness_slider != nullptr)
    {
        brightness_value_label->setText(HexByte(brightness_slider->value()));
    }
}

void ASRockRX9070XTPlugin::ApplyPanelSettings()
{
    if(registered_controllers.empty())
    {
        if(status_label != nullptr)
        {
            status_label->setText("No ASRock RX 9070 XT RGB controller was registered. Check that OpenRGB can see I2C bus 7.");
        }
        return;
    }

    RGBController_ASRockRX9070XT* controller = registered_controllers.front();

    const int target_index = target_combo->currentData().toInt();
    const uint8_t mode_value = static_cast<uint8_t>(mode_combo->currentData().toInt());
    const uint8_t speed_value = static_cast<uint8_t>(speed_slider->value());
    const uint8_t brightness_value = static_cast<uint8_t>(brightness_slider->value());

    controller->ApplyCustomSettings(target_index,
                                    mode_value,
                                    static_cast<uint8_t>(selected_color.red()),
                                    static_cast<uint8_t>(selected_color.green()),
                                    static_cast<uint8_t>(selected_color.blue()),
                                    speed_value,
                                    brightness_value,
                                    0x00);

    if(status_label != nullptr)
    {
        status_label->setText(QString("Applied %1 to %2.")
            .arg(mode_combo->currentText())
            .arg(target_combo->currentText()));
    }
}
