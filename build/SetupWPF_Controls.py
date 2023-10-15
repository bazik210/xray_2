import fileinput, sys

def replaceAll(file,searchExp,replaceExp):
    for line in fileinput.input(file, inplace=1):
        if searchExp in line:
            line = line.replace(searchExp,replaceExp)
        sys.stdout.write(line)
     
    
replaceAll('X-Ray-v2.0.sln', "xray\wpf_controls\wpf_controls.csproj" , r"..\xray\wpf_controls\wpf_controls.csproj")
