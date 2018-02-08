{
  "targets": [
  {
    "target_name": "rf24hub",
    "sources": ["src/rf24/rf24hub.cpp"],
    "cflags": ["-Wall", "-std=c++11", "-lrf24-bcm", "-lrf24network"],
    "cflags!": ["-fno-exceptions"],
    "cflags_cc": ["-Ofast", "-mfpu=vfp", "-mfloat-abi=hard", "-mtune=arm1176jzf-s"],
    "cflags_cc!": ["-fno-exceptions"],
    "libraries": ["-lrf24network"],
    "include_dirs": ["<!(node -e \"require('nan')\")", "<!(node -e \"require('streaming-worker-sdk')\")"]
  }]
}
