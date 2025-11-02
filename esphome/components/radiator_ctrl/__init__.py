import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core

AUTO_LOAD = []

CONF_RADIATOR_CTRL = "radiator_ctrl"

radiator_ctrl_ns = cg.esphome_ns.namespace("radiator_ctrl")
RadiatorCtrl = radiator_ctrl_ns.class_("RadiatorCtrl", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(RadiatorCtrl),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)