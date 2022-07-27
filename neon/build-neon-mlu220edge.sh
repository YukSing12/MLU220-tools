CURRENT_DIR=$(dirname $(readlink -f $0))

cd $CURRENT_DIR/build

cmake .. -DCMAKE_TOOLCHAIN_FILE=$CURRENT_DIR/toolchain-edge.cmake