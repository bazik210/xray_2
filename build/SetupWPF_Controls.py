import fileinput, sys

def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)
     
    
replaceAll('X-Ray-v2.0.sln', "xray\wpf_controls\wpf_controls.csproj" , r"..\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\controls\sources\editor_controls.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll("xray\editor\\base\sources\editor_base.vcxproj", r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\\animation\sources\\animation_editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\dialog\sources\dialog_editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\model\sources\model_editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\particle\sources\particle_editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\sound\sources\sound_editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")
replaceAll('xray\editor\world\sources\editor.vcxproj', r"xray\build\xray\wpf_controls\wpf_controls.csproj", r"xray\xray\wpf_controls\wpf_controls.csproj")