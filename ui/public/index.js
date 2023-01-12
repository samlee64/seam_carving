const config = require('../config.json');
const { Elm } = require('../src/Main');

require('../src/interop/removeObject.js');
require('../src/interop/removeObject2.js');
require('../src/interop/paintCanvas.js');

var app = Elm.Main.init({ flags: config });
