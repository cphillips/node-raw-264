const TelloVideo = require("../dist/binding.js")
const assert = require("assert")
const fs = require('fs/promises')

assert(TelloVideo, "The expected module is undefined");

async function testBasic()
{
    const instance = new TelloVideo("mr-yeoman");
    assert(instance.greet, ".greet is not defined");
    assert(instance.decode, ".decode is not defined");

  
    //assert.strictEqual(instance.greet("kermit"), "mr-yeoman", "Unexpected value returned");
    //assert.strictEqual(instance.decode(new ArrayBuffer()), {}, "Decode");
    let buffer = []
    for(let i=1; i<300;i++){
      const frag = await fs.readFile(`./test/data/frag_${i}.h264`)
      instance.decode(frag)
      buffer.push(frag)
      if(frag.length != 1460){
        console.log(instance.decode(Buffer.concat(buffer)))
        buffer = []
      }
    }

}

function testInvalidParams()
{
    const instance = new TelloVideo();
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");
assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw");

console.log("Tests passed- everything looks OK!");