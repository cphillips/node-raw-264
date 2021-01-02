"use strict";
const addon = require('../build/Release/node-raw-264');
;
class Raw264 {
    constructor(name) {
        this._addonInstance = new addon.TelloVideo(name);
    }
    greet(strName) {
        return this._addonInstance.greet(strName);
    }
    decode(data) {
        return this._addonInstance.decode(data);
    }
}
module.exports = Raw264;
