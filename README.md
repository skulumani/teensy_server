## Dependencies

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

