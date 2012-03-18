#!/usr/bin/env python
import os, sys

def error(val):
	if val != 0:
		print "Error during build."
		sys.exit(1)

flags = " -DCMAKE_BUILD_TYPE=Release "
for arg in sys.argv[1:]:
	if (arg == '-d'): flags = flags.replace('-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_BUILD_TYPE=Debug')

os.chdir('scripts')
error(os.system('cmake .'+flags))
error(os.system('make clean && make'))
os.chdir('../')
