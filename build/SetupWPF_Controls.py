import fileinput, sys, os

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

lines = open("X-Ray-v2.0.sln", "r").readlines()
for i_, line in enumerate(lines):
    if line.find("wpf_controls.csproj")!=-1:
        line_id = i_
        break
   
guid_line = lines[line_id]
guid_raw = guid_line.split('.csproj", "')[1]
guid_end = guid_raw.split('"')[0]

lines = open("X-Ray-v2.0.sln").readlines()
for i_, line in enumerate(lines):
    if line.find("editor_controls")!=-1:
        line_id = i_
        break

prev_line = lines[line_id]
new_line = lines[line_id+1]

guid_write = '		'+guid_end+' = '+guid_end+'\n' 
 
input_file = 'X-Ray-v2.0.sln'
output_file = 'temp.sln'
 
new_content = []

with open(input_file) as in_file, open(output_file, 'w') as out_file:
    for next_line in in_file:
        if not new_content and next_line.strip() != prev_line.strip():
            out_file.write(next_line)
        elif not new_content and next_line.strip() == prev_line.strip():
            out_file.write(next_line)
            out_file.write(new_line)
            new_content += guid_write
            out_file.writelines(new_content)
 #           break
        elif new_content:
            new_content = []
            
os.remove("X-Ray-v2.0.sln")
os.rename('temp.sln', 'X-Ray-v2.0.sln')