#!/bin/bash
echo "starting"

source ./source.env

currdir=$(pwd)
dirs=$(find . -type d  -maxdepth 1 2>/dev/null | grep -v .git   | grep -v  "\.$"  | grep -v 3pp)
dirs="./src"
for dir in $dirs; do
   echo "running ci within $dir"
   cd $dir  && ./run-ci.sh
   if [ $? != 0 ]; then
      exit $0
   fi
done
