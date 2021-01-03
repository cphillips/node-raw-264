
const sharp = require('sharp')
const fs = require('fs')



function convert(path, width, height) {
  fs.readFile(path, function (err, buffer) {
    console.log(err)
    sharp(buffer, { raw: { width: width, height: height, channels: 3 } })
      .toFile(`test.jpg`, function (err) {
        console.log(err)
      })
  })
}

convert('./test/frame_267.bmp', 960, 720)