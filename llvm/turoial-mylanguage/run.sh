rm -rf build
mkdir build
cd build  
env LLVM_DIR=/opt/homebrew/Cellar/llvm/13.0.0/lib/cmake/llvm  cmake ../

make VERBOSE=1
