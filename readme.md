# Why

I wanted to decode the video coming from the tello drone

# Thoughts

- I liked the projects that used emscripten, it's a cool approach. However, I wasn't able to make one work.

- For browser side, h264-converter is really well done, and works well, I like that it use the native video player and Media Source API

- I decide to make the API synchronous, because I was uncertain about how libav needs data feed to it, for example if data is out of order, can libav handle it? Is libav thread safe is it re-entrant? It wasn't documented well enough for me to easily know, so I used workers-threads and transfer lists is  the consuming application. I thought about just using an internal queue to deal with those issues, but code complexity and testing requirements would be larger

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








