import shutil, fileinput, sys

shutil.copyfile('xray/editor/world/sources/editor.vcxproj', '../xray/editor/world/sources/editor.vcxproj')
shutil.copyfile('xray/editor/world/sources/editor.sln', '../xray/editor/world/sources/editor.sln')
shutil.copyfile('xray/editor/world/sources/editor.vcxproj.filters', '../xray/editor/world/sources/editor.vcxproj.filters')

def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)
     
    
replaceAll('X-Ray-v2.0.sln', "xray\editor\world\sources\editor.vcxproj" , r"..\xray\editor\world\sources\editor.vcxproj")
