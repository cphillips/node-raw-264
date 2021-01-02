const addon = require('../build/Release/node-raw-264');

interface IRaw264Native {
  greet(strName: string): string;
  decode(data: ArrayBuffer): object;
};

class Raw264 {
  constructor(name: string) {
    this._addonInstance = new addon.TelloVideo(name)
  }

  greet(strName: string) {
    return this._addonInstance.greet(strName);
  }

  decode(data: ArrayBuffer) {
    return this._addonInstance.decode(data);
  }

  // private members
  private _addonInstance: IRaw264Native;
}

export = Raw264;
