#!/bin/bash

cd $(dirname $0)

cd  ../  && source  source.env && cd -

make  test
