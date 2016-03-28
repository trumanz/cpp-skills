#!/bin/bash

cd $(dirname $0)

cd  ../  && source  source.env && cd -

cd thrift &&  rm -rf gen-cpp  && thrift --gen cpp *.thrift && cd -

make  test
