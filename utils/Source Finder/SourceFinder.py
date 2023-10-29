import os
import re
from fnmatch import fnmatch

sourceNames = []

print("Looking for source files in target directory...")

# Example path 'F:\\Dezowave\\Repos\\xray\\third_party\\scaleform'
root = '<FILL THE PATH>'

# List all files
for path, subdirs, files in os.walk(root):
    for filename in files:
        if fnmatch(filename, '*.cpp'):
            sourceNames.append(os.path.join(path, filename))
        if fnmatch(filename, '*.hpp'):
            sourceNames.append(os.path.join(path, filename))
        if fnmatch(filename, '*.c'):
            sourceNames.append(os.path.join(path, filename))
        if fnmatch(filename, '*.h'):
            sourceNames.append(os.path.join(path, filename))

if len(sourceNames) == 0:
    print('No files found.\n')
else:
    print('Matching files found! Writing to file...')

# Prepare outputs
if not os.path.exists('found'):
    os.makedirs('found')
outputFile = open('./found/foundSources.txt', 'w+')

for filepath in sourceNames:
    # Example regex '.:.+scaleform\\?' - you can replace scaleform with trailing directory from root path
    entry = re.sub(r'.:.+<DIRECTORY NAME>\\?', '"${CMAKE_CURRENT_SOURCE_DIR}/', filepath + '"\n').replace('\\', '/')
    outputFile.write(entry)
outputFile.close()

print('Done!')
