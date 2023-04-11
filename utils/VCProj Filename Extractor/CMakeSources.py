import os

sourceNames = []

print("Reformatting all parsed .vcproj files in the directory...")

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
        # Mark beginning of Visual Studio Solution Explorer directory
        if 'begin ' in line:
            # Write '# folderName'
            outputFile.write('# ' + line[6:])
        elif 'end ' in line:
            #outputFile.write('------------------------\n')
            continue
        else:
            # Skip source file if it's excluded from any of these:
            # - Release|Win32
            # - Release|x64
            # - Debug|Win32
            # - Debug|x64
            if 'X:Release|Win32' in line or 'X:Release|x64' in line or 'X:Debug|Win32' in line or 'X:Debug|x64' in line:
                print('\t\t' + 'Skipped file because it didn\'t meet criteria: ' + line.split(';')[0])
                continue

            # Separate file name and additional info
            contents = line.split(';')

            # Extract file path
            sourceName = contents[0]

            # Strip all spaces and F: prefixes
            sourceName = sourceName.strip()
            sourceName = sourceName.strip('F:')

            # Reformat to fit CMake filepath style
            sourceName = sourceName.replace('\\', '/')

            # Write path for CMakeLists.txt
            if '../' in sourceName:
                outputFile.write('\"${CMAKE_CURRENT_SOURCE_DIR}/' + sourceName + '\"\n')
            elif './' in sourceName:
                outputFile.write('\"${CMAKE_CURRENT_SOURCE_DIR}/' + sourceName[2:] + '\"\n')
            else:
                outputFile.write('\"${CMAKE_CURRENT_SOURCE_DIR}/' + sourceName + '\"\n')
    outputFile.close()

    # Check for duplicates (sanity check to ensure correctness of previous step)
    outputFile = open('./commands/' + parsedFile[:-10] + 'commands.txt', 'r')
    seen = set()
    for line in outputFile:
        if line in seen:
            print('Found duplicate in output! File: ' + line)
        else:
            if '------------------------' in line or '# ' in line:
                continue
            seen.add(line)
    outputFile.close()