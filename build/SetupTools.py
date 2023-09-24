import fileinput, sys


def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)
     
    
replaceAll('xray\shader_generator\sources\shader_generator.vcxproj', "BOOST_NO_EXCEPTIONS;" , "")
