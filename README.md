# Prerequisites
Qt >= 5.5

# How to build
mkdir build

cd build

cmake -g "Unix Makefiles" .. -DCMAKE_PREFIX_PATH=/opt/Qt/5.5/gcc_64

make

# Troubleshooting
## Failed to find "GL/gl.h" in "/usr/include/libdrm"
sudo apt-get install mesa-common-dev