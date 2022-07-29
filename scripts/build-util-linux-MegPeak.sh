CURRENT_DIR=$(dirname $(readlink -f $0))
ROOT_DIR=$CURRENT_DIR/../

cd $ROOT_DIR/MegPeak

mkdir -p build && cd build
cmake .. -DMEGPEAK_ENABLE_OPENCL=ON -DCMAKE_TOOLCHAIN_FILE=$ROOT_DIR/toolchain-edge.cmake
make