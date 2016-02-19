rm -rf build
mkdir build
cd build
cmake -g "Unix Makefiles" .. -DCMAKE_PREFIX_PATH=/opt/Qt/5.5/gcc_64
make