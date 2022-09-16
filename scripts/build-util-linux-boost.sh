CURRENT_DIR=$(dirname $(readlink -f $0))
ROOT_DIR=$CURRENT_DIR/../

cd $ROOT_DIR/boost_1_70_0

./bootstrap.sh --with-libraries=all --with-toolset=gcc
sed -i 's|    using gcc ; |    using gcc : : /opt/work/gcc-linaro-6.2.1-2016.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc ; |' project-config.jam
./b2
./b2 install
