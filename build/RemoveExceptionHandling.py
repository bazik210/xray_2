import os, fnmatch
def findReplace(directory, find, replace, filePattern):
    for path, dirs, files in os.walk(os.path.abspath(directory)):
        for filename in fnmatch.filter(files, filePattern):
            filepath = os.path.join(path, filename)
             # Close w+ mode and open in r mode
            outputFile = open(filepath, 'r')
                    
            # Save existing lines to temp file
            tempFile = open(filepath[:-7] + '_temp.vcxproj', 'w+')
            for line in outputFile:
                if find not in line:
                    tempFile.write(line)
                else:
                    tempFile.write(replace)
            tempFile.close()
                        
            # Close r+ mode and open in a w+ mode to clear file
            outputFile.close()
            outputFile = open(filepath, 'w+')
                        
            # Open temp file to re-write it to output file
            tempFile = open(filepath[:-7] + '_temp.vcxproj', 'r')
            for line in tempFile:
                outputFile.write(line)
            tempFile.close()

findReplace(os.getcwd(), '<ExceptionHandling>Async</ExceptionHandling>', '\t<ExceptionHandling></ExceptionHandling>\n', '*.vcxproj')