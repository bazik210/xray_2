import xml.etree.ElementTree as ET
import os
import fileinput
import sys

# Namespace has to be defined or else ElementTree inputs some ns0: govno as default namespace...
namespace = 'http://schemas.microsoft.com/developer/msbuild/2003'
ET.register_namespace('', namespace)

filename = "engine_pc_dx11.vcxproj"
path = os.getcwd() +"\\xray\\engine\\pc\\\dx11\\sources\\"
vcxproj = os.path.join(path, filename)

# Read .vcxproj
contents = ET.parse(vcxproj)
root = contents.getroot()

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'PropertyGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'ConfigurationType' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("StaticLibrary","DynamicLibrary")
     
contents.write(vcxproj, '')

filename = "stalker2_pc_dx11.vcxproj"
path = os.getcwd() +"\\xray\\stalker2\\pc\\sources\\"
vcxproj = os.path.join(path, filename)

# Read .vcxproj
contents = ET.parse(vcxproj)
root = contents.getroot()

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'LinkLibraryDependencies' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("true","false")
     
contents.write(vcxproj, '')

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'IgnoreSpecificDefaultLibraries' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("%(IgnoreSpecificDefaultLibraries)","libcmt;libcpmt;%(IgnoreSpecificDefaultLibraries)")
     
contents.write(vcxproj, '')

for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'AdditionalDependencies' and 'Release' in group.get('Condition'):
            element.text = element.text.replace(";legacy_stdio_definitions.lib;xapobase.lib;ucrt.lib","")
     
contents.write(vcxproj, '')


def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line) 
        
replaceAll('xray\engine\pc\dx11\sources\engine_pc_dx11.vcxproj', 'binaries\Release\lib</OutDir>', r"binaries\Release\</OutDir>")

line_in = "'Release|x64'" + '">.lib</TargetExt>'
line_out = "'Release|x64'" + '">.dll</TargetExt>'
replaceAll('xray\engine\pc\dx11\sources\engine_pc_dx11.vcxproj', line_in, line_out)