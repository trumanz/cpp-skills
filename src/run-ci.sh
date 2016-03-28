#!/bin/bash

cd $(dirname $0)


cd thrift &&  rm -rf gen-cpp  && thrift --gen cpp *.thrift && cd -

make  test
