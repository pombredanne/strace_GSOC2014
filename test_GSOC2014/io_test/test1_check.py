#!/usr/bin/python
import sys
import subprocess

def startTest():
	print "this is original output(-r -T -i -e trace=read,write)"
	subprocess.call("../../strace -r -T -i -e trace=read,write ./test1", shell=True)

	print "this is json output(-r -T -i -j -e trace=read,write)"
	subprocess.call("../../strace -r -T -i -j -e trace=read,write ./test1", shell=True)

if __name__ == "__main__":
	startTest()
