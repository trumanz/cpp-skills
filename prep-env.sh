#!/bin/sh

BASE_DIR=$(pwd)/3pp
[ -d $BASE_DIR ] || mkdir $BASE_DIR

INSTALL_DIR=$BASE_DIR/installdir
#mkdir  $INSTALL_DIR

mordor_lib(){
   if [ -d mordor ]; then
      echo "use downloaded mordor"
   else
      git clone  https://github.com/mozy/mordor.git  \
      && cd  mordor  \
      && mkdir install_dir \
      && ./buildtools/build.sh  --prefix=$INSTALL_DIR \
      && make -j4 && make install
      if [ $? != 0 ]; then
        exit $?
      fi
   fi
}

gtest(){
   #if [ -d googletest ]; then
   #  echo "user donloaded googletest"
   #else
     git clone https://github.com/google/googletest.git  \
     && cd googletest \
     && cmake  -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR \
     && make -j4 && make  install
     if [ $? != 0 ]; then
        exit $?
     fi
  #fi
}

thrift() {
      #git clone https://github.com/apache/thrift.git \
      cd thrift &&  git checkout  0.9.3 \
      && ./bootstrap.sh &&  ./configure --prefix=$INSTALL_DIR \
      && make -j4 && make install
      if [ $? != 0 ]; then
        echo "Warning, TODO, fix this "
      fi
}

#cd $BASE_DIR
#mordor_lib

#cd $BASE_DIR
#gtest

cd $BASE_DIR
thrift
