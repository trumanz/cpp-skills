rm -rf build
mkdir build
cd build  
export LLVM_DIR=/opt/homebrew/Cellar/llvm/13.0.0/lib/cmake/llvm
#cmake ../
cmake  .. "-DCMAKE_TOOLCHAIN_FILE=${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake"
make VERBOSE=1
