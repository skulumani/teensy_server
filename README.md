
## Dependencies

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



## References

* [Serial Guide](https://www.cmrr.umn.edu/~strupp/serial.html)
* [libserial](https://sigrok.org/api/libserialport/unstable/index.html)
* [Cmake protobuf](https://github.com/shaochuan/cmake-protobuf-example)
* [Better Cmaek protobuf](https://github.com/ttroy50/cmake-examples/tree/master/03-code-generation/protobuf)
