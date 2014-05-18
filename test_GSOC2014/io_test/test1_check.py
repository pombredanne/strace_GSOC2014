#!/usr/bin/python
import sys
import subprocess

def startTest():
	print "this is original output(-y -e trace=read,write)"
	subprocess.call("../../strace -y -e trace=read,write ./test1", shell=True)

	print "this is json output(-j -y -e trace=read,write)"
	subprocess.call("../../strace -j -y -e trace=read,write ./test1", shell=True)

if __name__ == "__main__":
	startTest()
