BUILD COMMAND
mkdir build && cd build &&
cmake --j 4 -DCMAKE_BUILD_TYPE=Debug ../ && make -j6 && ./Debug/custom_cef_app
