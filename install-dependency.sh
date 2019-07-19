#!/bin/sh
sudo apt-get update
sudo apt-get install -y ragel  gawk libtool  cmake   autoconf
sudo apt-get install -y libboost-all-dev libevent-dev 
sudo apt-get install -y libyaml-dev zlib1g-dev liblzma-dev 
sudo apt-get install -y postgresql-server-dev-all 
sudo apt-get install -y libsqlite3-dev
sudo apt-get install -y libjsoncpp-dev
sudo apt-get install -y  bison flex
sudo apt-get install -y ragel
sudo apt-get install -y libgtest-dev  google-mock
sudo bash -c  'cd /usr/src/gtest  &&  cmake  CMakeLists.txt &&  make &&  cp *.a  /usr/lib/'
