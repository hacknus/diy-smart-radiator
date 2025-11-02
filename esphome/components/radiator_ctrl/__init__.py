import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

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

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RadiatorCtrl),
    cv.Optional(CONF_STEPPER_STEP_PIN, default=18): cv.int_,
    cv.Optional(CONF_STEPPER_DIR_PIN, default=19): cv.int_,
    cv.Optional(CONF_STEPPER_ENABLE_PIN, default=21): cv.int_,
    cv.Optional(CONF_TMC2209_TX_PIN, default=20): cv.int_,
    cv.Optional(CONF_TMC2209_RX_PIN, default=21): cv.int_,
    cv.Optional(CONF_DISPLAY_MOSI_PIN, default=23): cv.int_,
    cv.Optional(CONF_DISPLAY_CLK_PIN, default=18): cv.int_,
    cv.Optional(CONF_DISPLAY_CS_PIN, default=5): cv.int_,
    cv.Optional(CONF_DISPLAY_DC_PIN, default=2): cv.int_,
    cv.Optional(CONF_DISPLAY_RST_PIN, default=4): cv.int_,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_step_pin(config[CONF_STEPPER_STEP_PIN]))
    cg.add(var.set_dir_pin(config[CONF_STEPPER_DIR_PIN]))
    cg.add(var.set_enable_pin(config[CONF_STEPPER_ENABLE_PIN]))
    cg.add(var.set_tmc2209_tx_pin(config[CONF_TMC2209_TX_PIN]))
    cg.add(var.set_tmc2209_rx_pin(config[CONF_TMC2209_RX_PIN]))
    cg.add(var.set_display_mosi_pin(config[CONF_DISPLAY_MOSI_PIN]))
    cg.add(var.set_display_clk_pin(config[CONF_DISPLAY_CLK_PIN]))
    cg.add(var.set_display_cs_pin(config[CONF_DISPLAY_CS_PIN]))
    cg.add(var.set_display_dc_pin(config[CONF_DISPLAY_DC_PIN]))
    cg.add(var.set_display_rst_pin(config[CONF_DISPLAY_RST_PIN]))
