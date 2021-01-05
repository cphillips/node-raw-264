import fs from 'fs/promises'
import { Raw264 } from './raw-264'

async function testBasic() {
  const raw264 = new Raw264()
  let buffer = []
  for (let i = 1; i < 300; i++) {
    const frag = await fs.readFile(`./test/data/frag_${i}.h264`)
    buffer.push(frag)
    if (frag.length != 1460) {
      console.log(raw264.decode(Buffer.concat(buffer)))
      buffer = []
    }
  }

}

testBasic()

console.log("Tests Complete")
