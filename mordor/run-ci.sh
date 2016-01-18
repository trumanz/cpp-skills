#!/bin/sh

if [ -d mordor ]; then
  echo "use downloaded mordor"
else
  git clone  https://github.com/mozy/mordor.git  \
  && cd  mordor  && mkdir install_dir \
  &&  ./buildtools/build.sh  --prefix=$(pwd)/install_dir
  if [ $? != 0 ]; then
     exit $?
  fi
fi

