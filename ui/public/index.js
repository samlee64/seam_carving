const config = require("../config.json")
const { Elm }  = require("../src/Main")

require('../src/interop/removeObject.js');

var app = Elm.Main.init({flags: config})
