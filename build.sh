apt-get update
apt-get -y install libboost-all-dev cmake make

mkdir -p build
cd build

cmake -S .. .
make

cp buildsa ../buildsa
cp inspectsa ../inspectsa
cp querysa ../querysa
