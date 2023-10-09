import xml.etree.ElementTree as ET
import os
import fnmatch

# Namespace has to be defined or else ElementTree inputs some ns0: govno as default namespace...
namespace = 'http://schemas.microsoft.com/developer/msbuild/2003'
ET.register_namespace('', namespace)

# Parse each project file and remove _DEBUG and NDEBUG from RelWithDebInfo configuration
for path, dirs, files in os.walk(os.path.abspath(os.getcwd())):
        for filename in fnmatch.filter(files, '*.vcxproj'):
            vcxproj = os.path.join(path, filename)

            # Read .vcxproj
            contents = ET.parse(vcxproj)
            root = contents.getroot()

            # Find all tags related to configuration groups
            for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
                # Iterate through contents and look for preprocessor definitions that meet criteria
                groupElement = group.iter()
                for element in groupElement:
                    # Remove preprocessor definitions
                    if element.tag == '{'+ namespace + '}' + 'PreprocessorDefinitions' and 'RelWithDebInfo' in group.get('Condition'):
                        element.text = element.text.replace('_DEBUG;', '')
                        element.text = element.text.replace('NDEBUG;', '')
                        
            contents.write(vcxproj, '')
