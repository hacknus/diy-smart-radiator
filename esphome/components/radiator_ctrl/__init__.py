import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome import pins

DEPENDENCIES = ['uart', 'spi']

radiator_ctrl_ns = cg.esphome_ns.namespace('radiator_ctrl')
RadiatorCtrl = radiator_ctrl_ns.class_('RadiatorCtrl', cg.Component)

CONF_STEPPER_STEP_PIN = "step_pin"
CONF_STEPPER_DIR_PIN = "dir_pin"
CONF_STEPPER_ENABLE_PIN = "enable_pin"
CONF_UART_ID = "uart_id"
CONF_SPI_ID = "spi_id"
CONF_DISPLAY_CS_PIN = "display_cs_pin"
CONF_DISPLAY_DC_PIN = "display_dc_pin"
CONF_DISPLAY_RST_PIN = "display_rst_pin"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RadiatorCtrl),
    cv.GenerateID(CONF_UART_ID): cv.use_id('uart::UARTComponent'),
    cv.GenerateID(CONF_SPI_ID): cv.use_id('spi::SPIComponent'),
    cv.Optional(CONF_STEPPER_STEP_PIN, default=1): pins.gpio_output_pin_schema,
    cv.Optional(CONF_STEPPER_DIR_PIN, default=0): pins.gpio_output_pin_schema,
    cv.Optional(CONF_STEPPER_ENABLE_PIN, default=2): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_CS_PIN, default=21): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_DC_PIN, default=20): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_RST_PIN, default=19): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_parent(uart))

    spi = await cg.get_variable(config[CONF_SPI_ID])
    cg.add(var.set_spi_parent(spi))

    step_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_STEP_PIN])
    cg.add(var.set_step_pin(step_pin))

    dir_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_DIR_PIN])
    cg.add(var.set_dir_pin(dir_pin))

    enable_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_ENABLE_PIN])
    cg.add(var.set_enable_pin(enable_pin))

    cs_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_CS_PIN])
    cg.add(var.set_display_cs_pin(cs_pin))

    dc_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_DC_PIN])
    cg.add(var.set_display_dc_pin(dc_pin))

    rst_pin = await cg.gpio_pin_expression(config[CONF_DISPLAY_RST_PIN])
    cg.add(var.set_display_rst_pin(rst_pin))
