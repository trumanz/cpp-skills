set pagination off
set confirm off


set print pretty on
set print address on

set logging file gdb.log
set logging on

source ./debug.py

b dprint
commands
   myprint a
   myprint vs
   #info locals
   continue
end




