import maya.cmds as cmds
import maya.mel as mel
import os

defaultSearchPath = "C:/"

version = 1.0
windowWidth = 550
pathStrHeight = 25

colorPatternPass = [(.2,.25,.2),(.25,.3,.25)]
colorPatternFail = [(.25,.2,.2),(.3,.25,.25)]
passColor = [.2,.5,.2]
failColor = [.5,.2,.2]
progressBarName = "searching"

if(cmds.window('texFix_Window', q=1, ex=1)):
	cmds.deleteUI('texFix_Window')
if(cmds.windowPref('texFix_Window', q=1, ex=1)):
	cmds.windowPref('texFix_Window', r=1)

global fileTextures
fileTextures = cmds.ls(typ="file")

def populatePathList(*args):
	global fileTextures
	fileTextures = cmds.ls(typ="file")
	currentControls = []
	try:
		currentControls = cmds.columnLayout("uiColWrapper2", q=1, ca=1)
		if(currentControls == None):
			currentControls = []
		print currentControls
	except:
		pass
	if(len(currentControls) > 0):
		for control in currentControls:
			cmds.deleteUI( control, control=True )
		cmds.refresh()
	tmpColorPatListPass = colorPatternPass
	tmpColorPatListFail = colorPatternFail
	rowLayOutCount = 0
	for tex in fileTextures:
		queryPath = cmds.getAttr("%s.fileTextureName"%(tex))
		initResultPre = os.path.exists(queryPath)
		cmds.rowLayout('_%i_rowLayout'%(rowLayOutCount), nc=2, cw2=[(int(windowWidth * .8)),(int(windowWidth * .2))], adj=1, p="uiColWrapper2")
		if(initResultPre == True):
			cmds.text("%s"%(tex), l = "%s"%(queryPath),al="left", bgc=tmpColorPatListPass[0], w=(int(windowWidth * .5)), h=pathStrHeight, p = '_%i_rowLayout'%(rowLayOutCount))
		elif(initResultPre == False):
			cmds.text("%s"%(tex), l = "%s"%(queryPath),al="left", bgc=tmpColorPatListFail[0], w=(int(windowWidth * .5)), h=pathStrHeight, p = '_%i_rowLayout'%(rowLayOutCount))
		cmds.button('_%i_texSelectButton'%(rowLayOutCount), l="%s"%(tex), w=(int(windowWidth * .4)), c='cmds.select("%s")'%(tex), p='_%i_rowLayout'%(rowLayOutCount))
		
		tmpColorPass = tmpColorPatListPass[0]
		del tmpColorPatListPass[0]
		tmpColorPatListPass.append(tmpColorPass)
		tmpColorFail = tmpColorPatListFail[0]
		del tmpColorPatListFail[0]
		tmpColorPatListFail.append(tmpColorFail)
		rowLayOutCount += 1
	cmds.button("StartTextureSearch", e=1, en=1)

def resourceAllTextures(*args):
	orphanSearchPath = cmds.textFieldButtonGrp('Dir', q=1, text=1)
	widgetQueryListPre = cmds.columnLayout("uiColWrapper2", q=1, ca=1)
	widgetCount = 0
	widgetQueryList = []
	for f in widgetQueryListPre:
		try:
			rowLayoutList = cmds.rowLayout("%s"%(f), q=1, ca=1)
		except:
			rowLayoutList = []
		if(len(rowLayoutList) != 0):
			for x in rowLayoutList:
				if(x[0] != "_"):
					widgetQueryList.append(x)
	widgetCount = len(widgetQueryList)
	commonLocations = []
	for tex in widgetQueryList:
		queryPath = cmds.getAttr("%s.fileTextureName"%(tex))
		initResult = os.path.exists(queryPath)
		queryPath = queryPath.replace("\\","/")
		splitPath = queryPath.split("/")
		searchFile = splitPath[-1]
		lostOrFound = 0
		if((initResult != True) and (len(commonLocations) != 0)):
			for commonLoc in commonLocations:
				formattedTestPath0 = commonLoc.replace("\\","/")
				formattedTestPath1 = formattedTestPath0[::-1]
				formattedTestPath2 = formattedTestPath1.split("/", 1)[-1]
				formattedTestPath3 = formattedTestPath2[::-1]
				commonPathResult = os.path.exists("%s/%s"%(formattedTestPath3,searchFile))
				if(commonPathResult == True):
					resolvedPath = "%s/%s"%(formattedTestPath3,searchFile)
					resolvedPath = resolvedPath.replace("/","\\")
					cmds.text("%s"%(tex), e=1, bgc=passColor)
					cmds.text("%s"%(tex), e=1, l=resolvedPath)
					cmds.setAttr("%s.fileTextureName"%(tex), resolvedPath, type="string")
					lostOrFound = 1
		elif((initResult != True) and (len(commonLocations) == 0) and (lostOrFound == 0)):
			searchCount = 0
			for (path) in os.walk(orphanSearchPath):
				searchCount += 1
			resolvedPath = queryPath
			cmds.text("%s"%(tex), e=1, bgc=failColor)
			print "resourcing %s"%(tex)
			makeProgressBar(progressBarName, searchCount)
			for (path, dirs, files) in os.walk(orphanSearchPath):
				print "searching in path: %s"%(path)
				moveProgressBar(progressBarName, 1)
				path = path.replace("/","\\")
				if(searchFile in files):
					resolvedPath = "%s\\%s"%(path, searchFile)
					resolvedPath = resolvedPath.replace("/","\\")
					commonLocations.append(resolvedPath)
					lostOrFound = 1
					cmds.text("%s"%(tex), e=1, bgc=passColor)
					cmds.text("%s"%(tex), e=1, l=resolvedPath)
			resolvedPath = resolvedPath.replace("/","\\")
			cmds.setAttr("%s.fileTextureName"%(tex), resolvedPath, type="string")
		else:
			pass
			cmds.text("%s"%(tex), e=1, bgc=passColor)
		killProgressWindow(progressBarName)
		cmds.setFocus("texFix_Window")
		cmds.scrollLayout("scrollLayout", e=1, sbp=["down", 25])
		cmds.refresh()

def browseIt():
	searchPathRaw = cmds.fileDialog2(fm=2, cap="Browse for a directory to search within..", dir=defaultSearchPath, okc="Choose Directory", dialogStyle=2)
	searchPath = searchPathRaw[0].replace("/", "\\")
	if(len(searchPath) == 2 and searchPath[-1] == ":"):
		searchPath = searchPath + "\\"
	cmds.textFieldButtonGrp('Dir', e=1, text=searchPath)
	return 1

def makeProgressBar(name, maxVal):
	if(cmds.window("%s"%(name), q=1, ex=1)):
		cmds.deleteUI("%s"%(name))
	if(cmds.windowPref("%s"%(name), q=1, ex=1)):
		cmds.windowPref("%s"%(name), r=1)

	progWindow = cmds.window("%s"%(name), title="%s Progress Window"%(name), widthHeight=(300, 50))
	cmds.columnLayout("%s_colLayout", p="%s"%(name))
	progressControl = cmds.progressBar("%s_Progress"%(name), maxValue=maxVal, width=300, height=50)
	cmds.showWindow( progWindow )

def moveProgressBar(name, stepSize):
	cmds.progressBar("%s_Progress"%(name), edit=True, step=stepSize)

def killProgressWindow(name):
	if(cmds.window("%s"%(name), q=1, ex=1)):
		cmds.deleteUI("%s"%(name))
	if(cmds.windowPref("%s"%(name), q=1, ex=1)):
		cmds.windowPref("%s"%(name), r=1)

def makeGui():
	texFix_Window = cmds.window('texFix_Window', title="Texture Search/Source Tool - V%s"%(version), iconName='texFix_Window', widthHeight=(windowWidth+15, 400) )
	
	scrollLayout = cmds.scrollLayout("scrollLayout", w=windowWidth, cr=1, p="texFix_Window" )
	
	cmds.columnLayout('uiColWrapper', w = (windowWidth - 10), adj=1, adjustableColumn=True, p="scrollLayout")
	
	cmds.text(l = "Automatic 1-click texture resource tool", bgc=[1,1,1], p = 'uiColWrapper')
	cmds.separator( height=10, style='double', p = 'uiColWrapper')
	
	cmds.textFieldButtonGrp('Dir', label="Root path for search", ad3=2 , cw3 = [105,380,50], text=defaultSearchPath, buttonLabel='browse', buttonCommand=browseIt, parent = 'uiColWrapper')
	cmds.button('populateResultArea', l = "-Display Texture Paths-", bgc = [.35, .35, .35], h=40, c = populatePathList, parent = 'uiColWrapper' )
	cmds.button('StartTextureSearch', l = "-Fix Texture Paths-", bgc = [.35, .6, .35], h=40, en=0, c = resourceAllTextures, parent = 'uiColWrapper' )
	
	cmds.separator( height=10, style='double', p = 'uiColWrapper')
	
	cmds.columnLayout('uiColWrapper2', w = (windowWidth - 10), adjustableColumn=True, p="scrollLayout")
	
	
	cmds.showWindow( texFix_Window )
makeGui()