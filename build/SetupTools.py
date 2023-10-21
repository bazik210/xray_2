import fileinput, sys


def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)
     
    
replaceAll('xray\shader_generator\sources\shader_generator.vcxproj', "BOOST_NO_EXCEPTIONS;" , "")


def replaceAll_Config(maya_type):
    src_dir = 'xray\maya\sources\\' 
    prj_name = '.vcxproj' 
    bins_out = "\</OutDir>"
    replaceAll(src_dir + maya_type + prj_name, r"binaries\Debug\</OutDir>", r"binaries\Debug"'\\'+ maya_type + bins_out)
    replaceAll(src_dir + maya_type + prj_name, r"binaries\Release\</OutDir>", r"binaries\Release"'\\'+ maya_type + bins_out)
    replaceAll(src_dir + maya_type + prj_name, r"binaries\MinSizeRel\</OutDir>", r"binaries\MinSizeRel"'\\'+ maya_type + bins_out)
    replaceAll(src_dir + maya_type + prj_name, r"binaries\RelWithDebInfo\</OutDir>", r"binaries\RelWithDebInfo"'\\'+ maya_type + bins_out)
    replaceAll(src_dir + maya_type + prj_name, r"<SubSystem>Console</SubSystem>", r"<SubSystem>NotSet</SubSystem>")


replaceAll_Config("maya2011")
replaceAll_Config("maya2012")
replaceAll_Config("maya2018")
replaceAll_Config("maya2019")
replaceAll_Config("maya2023")
