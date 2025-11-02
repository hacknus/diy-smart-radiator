import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome import pins

DEPENDENCIES = ['uart']

radiator_ctrl_ns = cg.esphome_ns.namespace('radiator_ctrl')
RadiatorCtrl = radiator_ctrl_ns.class_('RadiatorCtrl', cg.Component)

CONF_STEPPER_STEP_PIN = "step_pin"
CONF_STEPPER_DIR_PIN = "dir_pin"
CONF_STEPPER_ENABLE_PIN = "enable_pin"
CONF_UART_ID = "uart_id"
CONF_DISPLAY_MOSI_PIN = "display_mosi_pin"
CONF_DISPLAY_CLK_PIN = "display_clk_pin"
CONF_DISPLAY_CS_PIN = "display_cs_pin"
CONF_DISPLAY_DC_PIN = "display_dc_pin"
CONF_DISPLAY_RST_PIN = "display_rst_pin"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RadiatorCtrl),
    cv.GenerateID(CONF_UART_ID): cv.use_id('uart::UARTComponent'),
    cv.Optional(CONF_STEPPER_STEP_PIN, default=18): pins.gpio_output_pin_schema,
    cv.Optional(CONF_STEPPER_DIR_PIN, default=19): pins.gpio_output_pin_schema,
    cv.Optional(CONF_STEPPER_ENABLE_PIN, default=23): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_MOSI_PIN, default=23): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_CLK_PIN, default=14): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_CS_PIN, default=5): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_DC_PIN, default=2): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DISPLAY_RST_PIN, default=14): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Add the source files to build
    cg.add_build_flag("-I.")  # Add current directory to include path
    cg.add_platformio_option("build_src_filter", ["+<*>", "+<esphome/components/radiator_ctrl/control/*.cpp>", "+<esphome/components/radiator_ctrl/motor/*.cpp>", "+<esphome/components/radiator_ctrl/display/*.cpp>", "+<esphome/components/radiator_ctrl/tmc2209/*.cpp>"])

    uart = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_parent(uart))

    step_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_STEP_PIN])
    cg.add(var.set_step_pin(step_pin))

    dir_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_DIR_PIN])
    cg.add(var.set_dir_pin(dir_pin))

    enable_pin = await cg.gpio_pin_expression(config[CONF_STEPPER_ENABLE_PIN])
    cg.add(var.set_enable_pin(enable_pin))

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
