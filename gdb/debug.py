import sys
import gdb 
import os


class BugReport(gdb.Command):
    """example on internet"""

    def __init__(self):
         super(BugReport, self).__init__("bugreport", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        pagination = gdb.parameter("pagination")
        if pagination: gdb.execute("set pagination off")
        f = open("/tmp/bugreport.txt", "w")
        f.write(gdb.execute("thread apply all backtrace full", to_string=True))
        f.close()
        os.system("uname -a >> /tmp/bugreport.txt")
        if pagination: gdb.execute("set pagination on")
BugReport()


class MyPrint(gdb.Command):
    """Break print and continue"""

    def __init__(self):
         super(MyPrint, self).__init__("myprint", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        pagination = gdb.parameter("pagination")
        if pagination: gdb.execute("set pagination off")
        str = gdb.execute("print "  + arg, to_string = True)
        str = str.replace('= ','= ' + arg + ' = ', 1)
        print(str)
        if pagination: gdb.execute("set pagination on")
MyPrint()