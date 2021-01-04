//const { decode } = require("../dist/binding.js")
const assert = require("assert")
const fs = require('fs').promises
const addon = require('../build/Debug/node-raw-264')

async function decode(buffer){
  return new Promise((resolve, reject)=>{
    addon.decode(buffer,(arg,frames)=>{
      resolve(frames)
    })
  })
}

async function testBasic() {

  // addon.decode(Buffer.from([0]), (arg1, arg2) => {
  //   console.log(arg1)
  //   console.log(arg2)
  // })

  let buffer = []
  for (let i = 1; i < 300; i++) {
    const frag = await fs.readFile(`./test/data/frag_${i}.h264`)
    buffer.push(frag)
    if (frag.length != 1460) {
      const bufferIn = Buffer.concat(buffer)
      buffer = []
      const frames = await decode(bufferIn)
      console.log(frames)
      
    }
  }

}


testBasic()

console.log("Tests Complete")