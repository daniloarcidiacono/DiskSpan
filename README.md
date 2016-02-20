# Prerequisites
Qt >= 5.5

# How to build
qmake
make

# Troubleshooting
## Windows

## Unix
### Failed to find "GL/gl.h" in "/usr/include/libdrm"
sudo apt-get install mesa-common-dev

### /usr/bin/ld: cannot find -lGL
sudo apt-get install libglu1-mesa-dev