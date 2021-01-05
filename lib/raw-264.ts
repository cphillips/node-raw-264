const addon = require('../build/Release/node-raw-264')

export interface IFrame {
  height: number
  width: number
  rowSize: number
  buffer: Buffer
}


export class Raw264 {
  instance:any
  constructor() {
    this.instance = new addon.Raw264()
  }
  decode(data: Buffer): IFrame[] {
    return this.instance.decode(data) as IFrame[]
  }
}



