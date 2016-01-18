#!/bin/sh

[ -d 3pp ] || mkdir 3pp

cd 3pp

mordor_lib(){
   if [ -d mordor ]; then
      echo "use downloaded mordor"
   else
      git clone  https://github.com/mozy/mordor.git  \
      && cd  mordor  \
      && mkdir install_dir \
      && ./buildtools/build.sh  --prefix=$(pwd)/install_dir \
      && make install
      if [ $? != 0 ]; then
        exit $?
      fi
   fi
}

mordor_lib

