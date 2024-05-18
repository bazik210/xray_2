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

# Parse each project file and replace MultiThreadedDLL to MultiThreaded in Release configuration
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
        vcxproj_output = os.path.join(path, "temp.vcxproj")
        #print(vcxproj)

        # Read .vcxproj
        contents = ET.parse(vcxproj)
        root = contents.getroot()

        # Find all tags related to configuration group
        for group in root.findall('{'+ namespace + '}' + 'ItemDefinitionGroup'):
            groupElement = group.iter()
            for element in groupElement:
                if element.tag == '{'+ namespace + '}' + 'Optimization' and 'Release' in group.get('Condition'):
                    element.text = element.text.replace("Full","MaxSpeed")
     
        contents.write(vcxproj, '')

        sub = "ItemDefinitionGroup Condition="
        child = sub+'"'+"'$(Configuration)|$(Platform)'=='Release|x64'"
        subchild = "</ClCompile>"
        sublink_child = "</Link>"
        c_write = "	  <WholeProgramOptimization>true</WholeProgramOptimization>\n"
        fiber_read = "<EnableFiberSafeOptimizations>"
        linker_write = "	  <LinkTimeCodeGeneration>UseLinkTimeCodeGeneration</LinkTimeCodeGeneration>\n	  <OptimizeReferences>true</OptimizeReferences>\n	  <EnableCOMDATFolding>true</EnableCOMDATFolding>\n"
        fiber_write = "	  <EnableFiberSafeOptimizations>true</EnableFiberSafeOptimizations>\n"

#       print(vcxproj)
#       print(vcxproj_output)
 
        new_content = []
        f = False
        c = False
        l = False

        with open(vcxproj) as in_file, open(vcxproj_output, 'w') as out_file:
            for next_line in in_file:
                if not f and not c and not l and not new_content and not child in next_line:
                    out_file.write(next_line)
                elif not f and not c and not l and not new_content and child in next_line:
                    out_file.write(next_line)
#                   print("Release namespace found!")
                    f = True
                elif f and not c and not l and fiber_read in next_line:
                    f = False
                    c = True
                    out_file.write(next_line)
                elif f and not c and not l and not fiber_read in next_line:
                    if subchild in next_line:
                        f = False
                        l = True
                        new_content = fiber_write
                        out_file.writelines(new_content)
                        new_content = c_write
                        out_file.writelines(new_content)
                        out_file.write(next_line)
                        new_content = []
                        
                    else:
                        out_file.write(next_line)  
                elif c and not f and not l and subchild in next_line: 
                    c = False
                    l = True
                    new_content = c_write
                    out_file.writelines(new_content)
                    out_file.write(next_line)
                    new_content = []
                elif not f and c and not l and not subchild in next_line:
                    out_file.write(next_line)
                elif l and not f and not c and not sublink_child in next_line:
                    out_file.write(next_line)
                elif l and  not f and not c and sublink_child in next_line:
                    l = False
                    new_content = linker_write
                    out_file.writelines(new_content)
                    out_file.write(next_line)
                    new_content = []

        os.remove(vcxproj)
        os.rename(vcxproj_output, vcxproj)