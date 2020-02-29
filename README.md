## TOOD

* [ ] GUI display of sensor parameters
* [ ] FSM to handle calibration, normal modes
* [ ] HDF5 to store archive data

## Dependencies

Cmake - `sudo apt-get install cmake`

Installing libserialport
~~~
git submodule update --init
cd libraries/libserialport

# Linux
sudo apt-get -y install autoconf libtool automake 

# Mac (make sure Homebrew is installed)
brew install autoconf libtool automake

./autogen.sh
./configure
make
sudo make install
~~~

Installing protobuf

~~~
sudo apt-get install autoconf automake libtool curl make g++ unzip

brew install autoconf automake libtool
git submodule update --init --recursive
cd libraries/protobuf
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig
~~~

Eigen - `sudo apt-get install libeigen3-dev`

## Vulkan

* [Tutorial](https://vulkan-tutorial.com/)
* [GLFW Tutorial](https://www.glfw.org/docs/latest/quick.html)
* [Imgui](https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html)

## References

* [Serial Guide](https://www.cmrr.umn.edu/~strupp/serial.html)
* [libserial](https://sigrok.org/api/libserialport/unstable/index.html)
* [Cmake protobuf](https://github.com/shaochuan/cmake-protobuf-example)
* [Better Cmaek protobuf](https://github.com/ttroy50/cmake-examples/tree/master/03-code-generation/protobuf)
