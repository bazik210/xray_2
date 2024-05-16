import xml.etree.ElementTree as ET
import os

# Namespace has to be defined or else ElementTree inputs some ns0: govno as default namespace...
namespace = 'http://schemas.microsoft.com/developer/msbuild/2003'
ET.register_namespace('', namespace)

filename = "bullet.vcxproj"
path = os.getcwd() +"\\third_party\\bullet\\"
vcxproj = os.path.join(path, filename)

# Read .vcxproj
contents = ET.parse(vcxproj)
root = contents.getroot()

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'Optimization' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("Disabled","MaxSpeed")
     
contents.write(vcxproj, '')

filename = "physics.vcxproj"
path = os.getcwd() +"\\physics\\sources\\"
vcxproj = os.path.join(path, filename)

# Read .vcxproj
contents = ET.parse(vcxproj)
root = contents.getroot()

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'Optimization' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("Disabled","MaxSpeed")
     
contents.write(vcxproj, '')

filename = "game.vcxproj"
path = os.getcwd() +"\\game\\sources\\"
vcxproj = os.path.join(path, filename)

# Read .vcxproj
contents = ET.parse(vcxproj)
root = contents.getroot()

# Find all tags related to configuration group
for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
    groupElement = group.iter()
    for element in groupElement:
        if element.tag == '{'+ namespace + '}' + 'Optimization' and 'Release' in group.get('Condition'):
            element.text = element.text.replace("Disabled","MaxSpeed")
     
contents.write(vcxproj, '')