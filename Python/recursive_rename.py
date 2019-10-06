# Python3 code to rename multiple 
# files in a directory or folder 
#give root directory as argument when executing program and all the file in root directory and subsequent folders will be renamed

#renaming parameter are to remove any '[xyz123]', '(xyz123)' and to replace '_' by ' '

# importing os module 
import os 
import re
import sys

# Function to rename multiple files 
def main():
	
	pathToDir = str(sys.argv[1])
	for root, dirs, files in os.walk(pathToDir):
		for filename in files:
			newFile = re.sub(r'(\s)*\[(.*?)\](\s)*|(\s)*\((.*?)\)(\s)*', '', filename) #regex to match all patterns that are of the form striong inside square or normal brackets
			newFile = re.sub(r'_', ' ', newFile) #replace _ by whitespace
			
			if (newFile != filename):
				os.rename(root + "\\" + filename, root + "\\" + newFile)

# Driver Code 
if __name__ == '__main__': 
	
	# Calling main() function 
	main() 
