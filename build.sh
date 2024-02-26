apt-get update
apt-get -y install libboost-all-dev

cd build

make

cp buildsa ../buildsa
cp inspectsa ../inspectsa
