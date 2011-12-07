#RCC Utility Program
#for use in makefile command arcclean
#deletes all files in the given directory that do not contain 'svn'

import re
import sys
import os

svn_regex = re.compile("svn")

os.chdir(sys.argv[1])
entries = os.listdir(os.getcwd())

for entry in entries:
	if(not svn_regex.search(entry)):
		os.remove(entry)


