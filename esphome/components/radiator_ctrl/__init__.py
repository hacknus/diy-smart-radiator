import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from esphome import pins

DEPENDENCIES = ['uart', 'i2c']

radiator_ctrl_ns = cg.esphome_ns.namespace('radiator_ctrl')
RadiatorCtrl = radiator_ctrl_ns.class_('RadiatorCtrl', cg.Component)

CONF_STEPPER_STEP_PIN = "step_pin"
CONF_STEPPER_DIR_PIN = "dir_pin"
CONF_STEPPER_ENABLE_PIN = "enable_pin"
CONF_TMC2209_TX_PIN = "tmc2209_tx_pin"
CONF_TMC2209_RX_PIN = "tmc2209_rx_pin"
CONF_DISPLAY_MOSI_PIN = "display_mosi_pin"
CONF_DISPLAY_CLK_PIN = "display_clk_pin"
CONF_DISPLAY_CS_PIN = "display_cs_pin"
CONF_DISPLAY_DC_PIN = "display_dc_pin"
CONF_DISPLAY_RST_PIN = "display_rst_pin"

# ESP32-C6 D-pin to GPIO mapping (adjust based on your specific board)
D_PIN_MAP = {
    'D0': 0, 'D1': 1, 'D2': 2, 'D3': 3, 'D4': 4, 'D5': 5,
    'D6': 6, 'D7': 7, 'D8': 8, 'D9': 9, 'D10': 10, 'D11': 11,
    'D12': 12, 'D13': 13, 'D14': 14, 'D15': 15, 'D16': 16,
    'D17': 17, 'D18': 18, 'D19': 19, 'D20': 20, 'D21': 21,
    'D22': 22, 'D23': 23
}

def validate_pin(value):
    if isinstance(value, str):
        if value.upper().startswith('D') and value.upper() in D_PIN_MAP:
            return D_PIN_MAP[value.upper()]
        elif value.upper().startswith('GPIO'):
            return int(value[4:])  # Remove 'GPIO' prefix
    return cv.int_(value)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RadiatorCtrl),
    cv.Optional(CONF_STEPPER_STEP_PIN, default=18): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_STEPPER_DIR_PIN, default=19): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_STEPPER_ENABLE_PIN, default=21): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_TMC2209_TX_PIN, default=20): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_TMC2209_RX_PIN, default=21): cv.All(validate_pin, pins.gpio_input_pin_schema),
    cv.Optional(CONF_DISPLAY_MOSI_PIN, default=23): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_DISPLAY_CLK_PIN, default=18): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_DISPLAY_CS_PIN, default=5): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_DISPLAY_DC_PIN, default=2): cv.All(validate_pin, pins.gpio_output_pin_schema),
    cv.Optional(CONF_DISPLAY_RST_PIN, default=4): cv.All(validate_pin, pins.gpio_output_pin_schema),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    step_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_STEP_PIN])
    cg.add(var.set_step_pin(step_pin))

    dir_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_DIR_PIN])
    cg.add(var.set_dir_pin(dir_pin))

    enable_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_ENABLE_PIN])
    cg.add(var.set_enable_pin(enable_pin))

    tx_pin = await cg.gpio_pin_expression(config[CONF_TMC2209_TX_PIN])
    cg.add(var.set_tmc2209_tx_pin(tx_pin))

    rx_pin = await cg.gpio_pin_expression(config[CONF_TMC2209_RX_PIN])
    cg.add(var.set_tmc2209_rx_pin(rx_pin))

    mosi_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_MOSI_PIN])
    cg.add(var.set_display_mosi_pin(mosi_pin))

    clk_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_CLK_PIN])
    cg.add(var.set_display_clk_pin(clk_pin))

    cs_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_CS_PIN])
    cg.add(var.set_display_cs_pin(cs_pin))

    dc_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_DC_PIN])
    cg.add(var.set_display_dc_pin(dc_pin))

    rst_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_RST_PIN])
    cg.add(var.set_display_rst_pin(rst_pin))
