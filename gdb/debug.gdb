set pagination off


set logging file gdb.log
set logging on

b dprint
commands
   print a
   print vs
   continue
end



