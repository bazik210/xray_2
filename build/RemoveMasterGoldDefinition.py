import xml.etree.ElementTree as ET
import os
import fnmatch
import json

# Namespace has to be defined or else ElementTree inputs some ns0: govno as default namespace...
namespace = 'http://schemas.microsoft.com/developer/msbuild/2003'
ET.register_namespace('', namespace)

# Load config file
cfg = []
try:
    with open('./script_configs/RemoveDebugDefinition.json') as cfgFile:
        cfg = json.load(cfgFile)
except IOError:
    print('Config file not found (./script_configs/RemoveDebugDefinition.json) - falling back to example file.')
    cfgFile = open('./script_configs/RemoveDebugDefinition.json.example')
    cfg = json.load(cfgFile)

# Parse each project file and remove MASTER_GOLD from Release configuration
for path, dirs, files in os.walk(os.path.abspath(os.getcwd())):
    for filename in fnmatch.filter(files, '*.vcxproj'):
        skip = 1
        for dir in cfg['included_directories']:
            if os.getcwd() + '\\' + dir + '\\' in path:
                skip = 0
                continue
        for proj in cfg['included_projects']:
            if proj + '.vcxproj' in filename:
                skip = 0
                continue
        if skip == 1:
            continue

        vcxproj = os.path.join(path, filename)
        #print(vcxproj)

        # Read .vcxproj
        contents = ET.parse(vcxproj)
        root = contents.getroot()

        # Find all tags related to configuration group
        for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
            # Iterate through contents and look for preprocessor definitions that meet criteria
            groupElement = group.iter()
            for element in groupElement:
                # Remove preprocessor definition
                if element.tag == '{'+ namespace + '}' + 'PreprocessorDefinitions' and 'Release' in group.get('Condition'):
                    element.text = element.text.replace("MASTER_GOLD;", "")
     
        contents.write(vcxproj, '')
