#!/bin/bash
echo "starting"


currdir=$(pwd)
for dir in $(find . -type d  -maxdepth 1 2>/dev/null | grep -v .git   | grep -v  "\.$"  | grep -v 3pp); do
   echo "running ci within $dir"
   cd $dir  && ./run-ci.sh
   if [ $? != 0 ]; then
      exit $0
   fi
done
