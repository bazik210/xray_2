import tempfile
import xml.etree.ElementTree as ET
import os

vcprojNames = []

print("Parsing all *.vcproj files in the directory...")

# Find all project files
for file in os.listdir('.'):
    if file.endswith('.vcproj'):
        vcprojNames.append(file)
        print('\t' + file)

if len(vcprojNames) == 0:
    print('No files found.\n')

# Parse each project file and save output to .txt file
for vcproj in vcprojNames:
    # Read .vcproj
    contents = ET.parse(vcproj)

    # Prepare outputs
    if not os.path.exists('parsed'):
        os.makedirs('parsed')
    outputFile = open('./parsed/' + vcproj[:-7] + '_parsed.txt', 'w+')

    seen = set()

    # Iterate over each element of .vcproj and look for filters and files
    for element in contents.iter():
        if element.tag == 'Filter':
            # Write begin to mark beginning of filter file list
            outputFile.write('begin ')
            # Write filter name (folder name from Solution Explorer view)
            outputFile.write(element.attrib['Name'] + '\n')

            # Write each file inside the filter
            for file in element.iter():
                if file.tag == 'File':
                    # Remove prior entry of file to move it further down the hierarchy tree
                    # This has to be done because XML elements are always read with their children
                    # It means that if we have a situation like this:
                    # WORLD
                    # |-> SUBSCRIBERS
                    # |     | damage_subscriber.h
                    # |     | game_object_subscriber.h
                    # |     | sensors_subscriber.h
                    # |     | sound_subscriber.h
                    # | ai_world.cpp
                    # | ai_world.h
                    # The script will enter WORLD first and process all of its children including SUBSCRIBERS
                    # and then it will step into SUBSCRIBERS to process its children (but they were already added)
                    # removing first entry of file from WORLD and then writing it in SUBSCRIBERS is necessary to maintain the hierarchy info
                    if file.attrib['RelativePath'] in seen:
                        # Close w+ mode and open in r mode
                        outputFile.close()
                        outputFile = open('./parsed/' + vcproj[:-7] + '_parsed.txt', 'r')
                    
                        # Save existing lines to temp file
                        tempFile = open('./parsed/' + vcproj[:-7] + '_temp.txt', 'w+')
                        for line in outputFile:
                            if file.attrib['RelativePath'] not in line:
                                tempFile.write(line)
                        tempFile.close()
                        
                        # Close r+ mode and open in a w+ mode to clear file
                        outputFile.close()
                        outputFile = open('./parsed/' + vcproj[:-7] + '_parsed.txt', 'w+')
                        
                        # Open temp file to re-write it to output file
                        tempFile = open('./parsed/' + vcproj[:-7] + '_temp.txt', 'r')
                        for line in tempFile:
                            outputFile.write(line)
                        tempFile.close()
                    else:
                        # Add it to seen files list to avoid duplicates
                        seen.add(file.attrib['RelativePath'])

                    # Write file name
                    outputFile.write('\tF:' + file.attrib['RelativePath'])

                    # Get additional info
                    for config in file.iter():
                        # Check if the file is excluded from compilation
                        if config.tag == 'FileConfiguration':
                            if 'ExcludedFromBuild' in config.attrib:
                                # Write excluded build
                                outputFile.write(';X:' + config.attrib['Name'])

                        # Check tool that should be used to compile it
                        if config.tag == 'Tool':
                            outputFile.write(';T:' + config.attrib['Name'])

                    # Go to newline (new file)
                    outputFile.write('\n')
            # Write end to mark end of filter file list
            outputFile.write('end \n')

    # Look for remaining files in root directory
    contents = None
    contents = ET.parse(vcproj)
    outputFile.write('begin ./\n')
    for element in contents.iter():
        if element.tag == 'File':
            if element.attrib['RelativePath'] in seen:
                continue
            else:
                # Add it to seen files list to avoid duplicates
                seen.add(element.attrib['RelativePath'])
    
                # Write file name
                outputFile.write('\tF:' + element.attrib['RelativePath'])
    
                # Get additional info
                for config in element.iter():
                    # Check if the file is excluded from compilation
                    if config.tag == 'FileConfiguration':
                        if 'ExcludedFromBuild' in config.attrib:
                            # Write excluded build
                            outputFile.write(';X:' + config.attrib['Name'])
    
                    # Check tool that should be used to compile it
                    if config.tag == 'Tool':
                        outputFile.write(';T:' + config.attrib['Name'])

                # Go to newline (new file)
                outputFile.write('\n')
    outputFile.write('end \n')

    outputFile.close()
