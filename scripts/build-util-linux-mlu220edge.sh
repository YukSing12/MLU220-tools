CURRENT_DIR=$(dirname $(readlink -f $0))
ROOT_DIR=$CURRENT_DIR/../
sudo apt install gettext autopoint bison libtool git build-essential pkg-config

cd $ROOT_DIR/util-linux

make clean

./autogen.sh

export CC=aarch64-linux-gnu-gcc
./configure --host=aarch64-linux-gnu LDFLAGS=-static --without-python --without-tinfo --without-ncursesw --without-ncurses

make lscpu -j$(nproc)

mv lscpu $ROOT_DIR