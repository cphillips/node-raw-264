# Why

I wanted to decode the video coming from the tello drone

# Thoughts

- I liked the projects that used emscripten, it's a cool approach. However, I wasn't able to make one work.

- For browser side, h264-converter is really well done, and works well, I like that it use thes native video player and Media Source API

# Reference projects

## Example from DJI
- https://github.com/dji-sdk/Tello-Python

## Uses Browser Media Source API
- https://github.com/xevojapan/h264-converter

## Web Asm and Android Source Code
- https://github.com/oneam/h264bsd
- https://github.com/udevbe/tinyh264
- https://github.com/mbebenita/Broadway
- https://github.com/gliese1337/h264decoder

## Web Asm and libav ( the one from FFMPEG )
- https://github.com/shengbinmeng/ffmpeg-h264-dec

## Pure javascript decoders 
- https://github.com/phoboslab/jsmpeg ( typically with ffmpeg transforming the data)

## Native Node Libraries
- https://github.com/Streampunk/beamcoder

## Spawn FFMPEG CLI ( typically with jsmpeg as viewer )
- https://github.com/johnwalicki/Node-RED-DroneViewer
- https://github.com/SovGVD/nodetello
- and many, many more

## Other ( non JS )
- https://github.com/SMerrony/tello
- https://github.com/bgromov/TelloSwift








