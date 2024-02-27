apt-get update
apt-get -y install libboost-all-dev build-essential libtool autoconf unzip wget libssl-dev

# Install CMAKE
version=3.28
build=1
mkdir temp
cd temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
./bootstrap
make -j$(nproc)
make install
cd ../../
rm -r temp

# Build project
mkdir -p build
cd build

cmake -S .. .
make

cp buildsa ../buildsa
cp inspectsa ../inspectsa
cp querysa ../querysa
