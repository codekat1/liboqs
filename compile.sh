

sudo apt install -y astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml

# For conio.h
#sudo apt-get install -y libncurses5-dev libncursesw5-dev

rm -rf build
mkdir build
cd build

#cmake -DBUILD_SHARED_LIBS=ON -GNinja ..
#cmake -DLIBOQS_INCLUDE_DIR=/usr/local/include -DLIBOQS_LIB_DIR=/usr/local/lib -DBUILD_SHARED_LIBS=ON -GNinja ..
cmake -DBUILD_SHARED_LIBS=ON -GNinja ..
ninja
#sudo ninja install