var zscout = require('zetta-scout');
var util = require('util');
var sensor = require('./sensor');
var lightDriver = require('./lightDriver.js');

// constructor
scout = module.exports = function() {
		zscout.call(this);
}
util.inherits(scout, zscout);

//init()

