import tempfile
import xml.etree.ElementTree as ET
import os

csprojNames = []

print("Parsing all *.csproj files in the directory...")

# Find all project files
for file in os.listdir('.'):
    if file.endswith('.csproj'):
        csprojNames.append(file)
        print('\t' + file)

if len(csprojNames) == 0:
    print('No files found.\n')

matches = ['.cs', 'resx', '.bmp', '.png', '.xaml']
marker1 = '\"'
marker2 = '\"'

# Parse each project file and save output to .txt file
for csproj in csprojNames:

    # Prepare outputs
    if not os.path.exists('parsed'):
        os.makedirs('parsed')
    outputFile = open('./parsed/' + csproj[:-7] + '_parsed.txt', 'w+')

    inputFile = open(csproj, 'r+')

    for line in inputFile:
        if 'Include' in line:
            if any(ext in line for ext in matches):
                marker1 = line.find('\"') + 1
                marker2 = line.find('\"', marker1)
                outputFile.write(line[marker1 : marker2] + '\n')

    outputFile.close()
