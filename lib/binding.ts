const addon = require('../build/Release/node-raw-264')

export interface IRaw264Frame {
  height: number
  width: number
  rowSize: number
  buffer: Buffer
}

export async function decode(data: ArrayBuffer, callback: (...args:any[]) => void) {
  return addon.decode(data, callback)
}


