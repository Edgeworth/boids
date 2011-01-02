#!/usr/bin/python
import os, sys

flags = " -DCMAKE_BUILD_TYPE=Release "
for arg in sys.argv[1:]:
	if (arg == '-d'): flags = flags.replace('-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_BUILD_TYPE=Debug')

os.chdir('scripts')
os.system('cmake .'+flags)
os.system('make clean && make')
os.chdir('../')
