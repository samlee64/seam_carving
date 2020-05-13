const config = require("../config.json")
const { Elm }  = require("../src/Main")

Elm.Main.init({flags: config})
