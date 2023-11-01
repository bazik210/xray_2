# Script to RePath textures in a new directory
# Also allows you to change the extension of the textures
# Select the texture nodes and then execute the script
# Script by - Kanishk Chouhan
# Email - kanishk2391@gmail.com

import maya.cmds as cmds
import os


def RePath():
    extn = cmds.textField(ExtnText, text=True, q=True)
    newPath = cmds.textField(pathText, text=True, q=True)
    textures = cmds.ls(sl=True, type="file")
    for tex in textures:
        oldpath = cmds.getAttr("%s.fileTextureName" % tex)
        print(oldpath)
        fileName = oldpath.split("/")[-1]
        NewFileName = fileName[:-3]
        print(NewFileName)
        ConvertedName = NewFileName + extn
        print(ConvertedName)
        FinalPath = os.path.join(newPath, ConvertedName)
        cmds.setAttr("%s.fileTextureName" % tex, FinalPath, type="string")


### Window Settings ###
win = cmds.window(title="RePath Textures", w=350, h=150)
cmds.columnLayout(adjustableColumn=True)
cmds.text("FIRST > select your file nodes in Hypershade!!", bgc=(.2, .4, .6))
cmds.frameLayout(l="Browse folder for New Textures")
cmds.flowLayout()
cmds.text("File Path", h=25)
pathText = cmds.textField(w=300, h=25)
cmds.setParent('..')
cmds.flowLayout()
cmds.text("Extn      ", h=25)
ExtnText = cmds.textField(w=50, h=25)

cmds.setParent('..')
fixit = cmds.button(l="Fix textures", c="RePath()")
cmds.setParent('..')

cmds.text("Script by - Kanishk Chouhan", bgc=(.2, .4, .6), h=15)
cmds.text("kanishk2391@gmail.com", bgc=(.2, .4, .6), h=15)
cmds.showWindow(win)
