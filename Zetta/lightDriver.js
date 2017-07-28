var device = require('zetta-device');
var util = require('util');

var light = module.exports = function(options){
		Device.call(this);
}
util.inherits(light, Device);

light.prototype.init = function(config){
config
;name('light')
.type('light')
.state('off')
.when('off', {allow: ['on']})
.when('on', {allow: ['off']})
.map('turn-on', this.turnOn, null)
.map('turn-off', this.turnOff, null)
}

light.prototype.turnOn = function(cb){
this.state = 'on';
cb();
}
light.prototype.turnOff = function(cb){
this.state = 'off';
cb();
}
