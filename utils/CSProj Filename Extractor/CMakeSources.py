import os

sourceNames = []

print("Reformatting all parsed .csproj files in the directory...")

# Find all parsed project files
for file in os.listdir('./parsed'):
    if file.endswith('_parsed.txt'):
        sourceNames.append(file)
        print('\t' + file)

if len(sourceNames) == 0:
    print('No files found.\n')

# Generate CMake commands based on parsed .vcproj files
for parsedFile in sourceNames:
    # Read parsed file
    file = open('./parsed/' + parsedFile, 'r')

    # Prepare outputs
    if not os.path.exists('commands'):
        os.makedirs('commands')
    outputFile = open('./commands/' + parsedFile[:-10] + 'commands.txt', 'w+')

    # Generate CMake paths
    for line in file:
        outputFile.write('\"${CMAKE_CURRENT_SOURCE_DIR}/' + line.strip().replace('\\', '/') + '\"' + '\n')
    outputFile.close()