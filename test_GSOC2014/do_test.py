#!/usr/bin/python
import sys
import os
import subprocess

def startTest(dir):
	""" start test in dir """
	oldcwd = os.getcwd()
	os.chdir(dir)
	subprocess.call(["make", "clean"])
	subprocess.call(["make", "all"])
	
	file_list = [f for f in os.listdir(".") if os.path.isfile(f) and os.path.splitext(f)[1] == ".py"]
	for f in file_list:
		subprocess.call(["python", f])

	os.chdir(oldcwd)

def listDirectory():
	""" get test directory """
	base_dir = os.path.join(".", os.path.dirname(sys.argv[0]))
	dir_list = [os.path.join(base_dir, d) for d in os.listdir(base_dir) 
					if os.path.isdir(os.path.join(base_dir, d)) and d[-5:] == "_test" ]
	return dir_list

def main():
	print "will start test in:", listDirectory()
	for d in listDirectory():
		startTest(d)

	print "finished all tests"

if __name__ == "__main__":
	sys.exit(main())
