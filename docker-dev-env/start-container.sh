#!/bin/bash
docker run -d --cap-add sys_ptrace -p127.0.0.1:1022:22  -v "$(pwd)"/example:/root/example  --name cppdev01  cppdev
