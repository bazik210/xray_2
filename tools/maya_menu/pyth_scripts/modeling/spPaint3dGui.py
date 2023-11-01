#-----------------------------------------------------------------
#	SCRIPT			spPaint3dGui.py
#	AUTHOR			Sebastien Paviot
#				    spaviot@gmail.com
#	DATE:			July,August 2009 - April 2010
#
#	DESCRIPTION:	Build Maya main tool and setup windows
#
#	VERSION:		2011.0
#
#-----------------------------------------------------------------


import maya.cmds as mc
import random as rand
import sys

import spPaint3dContext as spPaint3dContext

spPaint3dGuiID = "spPaint3d";
spPaint3dSetupID = "spPaint3dSetup";
spPaint3dVersion = 2011.0;

#debug to log some operation down to the script editor
sp3d_log = False


# optionVar name: (type, default value, corresponding class attribute)
# For now the class methods only check for 'iv' and 'fv' types while looping stuff
sp3dOptionVars = {
					"sp3dTransformRotate": ( "iv", 1, "transformRotate"),
					"sp3dTransformScale": ( "iv", 1, "transformScale"),
					"sp3dTransformScaleUniform": ( "iv", 1, "transformScaleUniform"),
					"sp3dInstance": ( "iv", 0, "instance"),
					"sp3dRandom": ( "iv", 1, "random"),
					"sp3dAlign": ( "iv", 1, "align"),
					"sp3dPaintFlux": ( "iv", 1, "paintFlux"),
					"sp3dPaintTimer": ( "fv", 0.05, "paintTimer"),
					"sp3dPaintDistance": ( "fv", 10, "paintDistance"),
					"sp3dPreserveInConn": ( "iv", 0, "preserveConn"),
					"sp3dSmoothNormal": ( "iv", 1, "smoothNormal"),
					"sp3dSetupHierarchy": ( "iv", 0, "hierarchy"),
					"sp3dGroup": ( "fv", 0, "group"),
					"sp3dVersion": ( "fv", spPaint3dVersion, "version")
				};



class sp3dToolOption (object):
	'''
	Class to store all tool related data
	'''

	def __init__ (self):
		'''
		Build the object to store all variables.
		Initialize the default attributes and call the methods to update value from optionVars if necessary.
		'''
		self.transformRotate = True
		self.transformScale = True
		self.transformScaleUniform = True
		self.instance = False
		self.random = True
		self.align = True
		self.paintFlux = True #True=distance / False=timer
		self.paintTimer = 0.05
		self.paintDistance = 10
		self.preserveConn = False
		self.smoothNormal = False #false=decal mode, force pure normal from intersected triangle / true=smoothed normal per neighboring edges
		self.hierarchy = False #False = no grouping of painted objects
		self.group = 0 #float value so it doesnt get converted into boolean when I batch read the Vars / 0=single group / 1=stroke group / 2=source group
		self.groupID = None #used to track the group name where to sort the generated objects from the paint strokes
		self.version = spPaint3dVersion; #used to allow tracking of potentially erroneous obsolete optionVars

		if(self.checkVars()):
			#all optionVars seem to be in proper condition, will fetch the stored data and update the instance attributes
			self.loadVars();
		else:
			#seems there are issues with the optionVars and/or version mismatch
			mc.confirmDialog( title='Script options alert', message='It seems either the script is run for the first time or the version running is different than the saved data.\nAll options will be reseted to defaults!', button=['Whatever'])
			self.commitVars();


	def dumpVars (self):
		'''
		print the value of all the tool option
		'''
		print self.__dict__

	def checkVars (self):
		'''
		Method to check if the stored optionVars contain valid data, if any.
		Returns False if any issue is detected.
		Returns True if all conditions are met and the method runs its course.
		'''
		for name, info in sp3dOptionVars.iteritems():	#loop through all the optionVars from the global struct
			#iterating through all the currently valid optionVar names from the global struct, then fetching the stored value if it exists
			type, value, varname = info
			if (not mc.optionVar(exists=name)):
				#name isnt' an existing optionVar
				return False
			if (name=='sp3dVersion' and mc.optionVar(q=name)!=value):
				#locally stored script version optionVar is obsolete
				return False

		#for loop exited before breaking, about to return True then
		return True

	def loadVars (self):
		'''
		Will load the stored optionVars into self
		'''
		for name, info in sp3dOptionVars.iteritems():	#loop through all the optionVars from the global struct
			#iterating through all the currently valid optionVar names from the global struct, then fetching the stored value if it exists
			type, value, varname = info
			if (type=='iv'):
				#this is an int value to convert into bool
				self.__dict__[varname] = bool(mc.optionVar(q=name));
			elif (type=='fv'):
				self.__dict__[varname] = round(mc.optionVar(q=name), 2);

		if (sp3d_log): self.dumpVars()


	def resetVars (self):
		'''
		Flush the values and restore default settings
		'''
		for name, info in sp3dOptionVars.iteritems():	#loop through all the optionVars from the global struct
			#iterating through all the currently valid optionVar names from the global struct, then fetching the stored value if it exists
			type, value, varname = info
			if (type=='iv'):
				#this is an int value to convert into bool
				mc.optionVar(iv=(name, value))
			elif (type=='fv'):
				#float value default
				mc.optionVar(fv=(name, value))

		self.loadVars();



	def commitVars (self):
		'''
		Method to store the data from instance attributes into optionVars.
		'''
		#print self.__dict__
		for name, info in sp3dOptionVars.iteritems():	#force initialize optionVars to defaults
			#print ('looping commitVar %s' %name)
			type, value, varname = info
			#print varname
			if (type=='iv'):
				#convert the bool attribute value into an int
				#print int(self.__dict__[varname])
				mc.optionVar(iv=(name,int(self.__dict__[varname])));
			elif (type=='fv'):
				#all other optionVar types
				mc.optionVar(fv=(name,self.__dict__[varname]));
		if (sp3d_log): self.dumpVars()



	def getGroupID (self):
		'''
		Called during the onRelease event of a stroke. This method manages a consistent unique group name throughout the paint strokes
		'''
		if (not self.groupID):
			#group is not yet created
			self.groupID = mc.group(empty=True, name='spPaint3dOutput')
		return self.groupID;





class sp3dTransform (object):
	'''
	contains values for the transform options
	'''
	def __init__(self, rotate=((0,0),(0,0),(0,0)), scale=((1,1),(1,1),(1,1))):
		'''
		initialise attributes
		'''
		self.rotate = rotate
		self.scale = scale

	def getRandomRotate (self):
		'''
		return a (x,y,z) tuple with properly randomized value between the self.rotate bounds
		'''
		x,y,z = self.rotate
		randxyz = (round(rand.uniform(x[0], x[1]),3), round(rand.uniform(y[0], y[1]),3), round(rand.uniform(z[0], z[1]),3))
		return randxyz

	def getRandomScale (self, uniform):
		'''
		return a (x,y,z) tuple with properly randomized value between the self.scale bounds
		'''
		x,y,z = self.scale
		if (uniform):
			randxyz = round(rand.uniform(x[0], x[1]),3)
			return (randxyz,randxyz,randxyz)
		else:
			randxyz = (round(rand.uniform(x[0], x[1]),3), round(rand.uniform(y[0], y[1]),3), round(rand.uniform(z[0], z[1]),3))
			return randxyz;


class sp3dObjectList (object):
	'''
	store a dictionnary of objects, their DAG data and some other data
	dictionnary structure: { 'name as displayed in the list, also the name of the transform' : 'fullDAGPath', Activation bool (default True), Probability float (default 50%), Align Override (default UpAxis either be Y or Z)}
	'''
 	#define the list of authorized object type used when adding objects to lists
	authType = {
		'default': ( 'mesh', ),
		'source': ( 'mesh', 'locator', ),
		'target': ('mesh', )
	};

	def __init__(self, authorized='target', errorHandle=None):
		'''
		initialize the default attributes.
		INPUT: 	authorized = specify the type of authorized geometry for that list, fetched from the authType class attributes
					valid values :	default / source / target (defined as class attributes)
		'''
		#TODO
		self.obj = {} #dictionnary of entries
		self.i=0 #index values used if this is a source object list using sequential mode distribution
		self.auth = self.authType[authorized] #used to sort 'valid' object when using the add method
		self.errorHandle = errorHandle;

	def validateObjects(self):
		'''
		Return True if all objects from the dictionnary really do/still exist in the scene (some monkeys delete objects while the script is running :))
		Return False once the method finds a missing object
		'''
		if (len(self.obj.keys()) == 0): return False; #list is empty
		for obj,data in self.obj.iteritems():
			if(self.errorHandle): self.errorHandle.raiseError("INFO: validating object: %s", data[0])
			if(not mc.objExists(data[0])): return False;
		if(self.errorHandle): self.errorHandle.raiseError("INFO: Everything checks out")
		return True;

	def hasDuplicate(self, compareobjlist):
		'''
		Return True of there's any object from self found in the 'compareobjlist' dictionnary
		Return False if all objects are unique
		(In context: there can't be an object which is both a source object and a target surface)
		'''
		for obj, data in compareobjlist.obj.iteritems():
			if(self.alreadyExists(obj)): return True;
		return False;

	def alreadyExists(self, obj):
		'''
		check if obj already exists in the self.dictionnary
		'''
		if (obj in self.obj): return True;
		else: return False;

	def addObj(self, obj=None, dagPath=None, activation=True, proba=0.5, align='Up'):
		'''
		append to the content of self.obj

		INPUT:	obj = object name (will become the entry key into the dictionnary)
				dagPath = full DAG path to that object
				activation = object is actually broadcasted into the poll of usable source objects, not used if the list is a target surface list
				proba = % probability presence when source objects are randomimzed, not used if the list is a target surface list
				align = axis override to compute the source object alignement on the target surface if align to surface option is On, not used if the list is a target surface list.
		RETURN: the name of the key if object was added properly, None otherwise
		'''
		if (self.alreadyExists(obj)):
			#key already exists, return False and break
			return None,"Object already exists in the list and can't be added again";
		else:
			#Determine if obj is valid, or if has valid children
			objtype = mc.objectType(obj)
			objchild = mc.listRelatives(obj, children=True, shapes=True) #get all the children shapes of the object

			if (objtype in self.auth):
				#a shape was selected and is part of the authorized object types for this list type. will use the shape's name as the key
				self.obj[obj] = (getDAGPath(obj,True), activation, proba, align);
				return obj,True
			elif (objtype=='transform'):
				#a transform was selected and checking now what lies underneath it
				#source valid object =
					#at least one of the children is a shape of auth type, that shape will be the key
					#if multiple shape of auth type, then the transform become the key
				#target valid object =
					#only 1 valid children of auth type
				if (objchild==None):
					#original object has no shapes among his 1st level children
					return None,"Object doesn't seem to have any direct child of the proper type";
				elif (len(objchild)==1 and mc.nodeType(objchild[0]) in self.auth):
					#obj(transform) has only one child, and is of a proper type for the list, return objchild[0] as the key
					obj=objchild[0]
					#now checking the only children and might already have been added
					if(self.alreadyExists(obj)):
						return None,"Object already exists in the list and can't be added again";
					else:
						self.obj[obj] = (getDAGPath(obj,True), activation, proba, align);
						return obj,True
				#elif(len(objchild)>1):
					#obj(transform) has multiple children and may be a group (or a mesh transform parenting other mesh)
					#check if at least one of the children is of the appropriate type
				else:
					#hierarchy too complex. TODO
					return None,"Does not compute (hierarchy too complex or whatever)"

			else:
				#doesn't fall in any valid category
				return None,"Object doesn't fit any valid category to be added to the list";

		#Never reach here

	def printObj(self):
		'''
		print the content of the dictionnary
		mostly used for debug purpose
		'''
		for obj,data in self.obj.iteritems():
			print ("key: %s || DAG: %s" % (obj, data[0]));

	def delObj(self, obj=None):
		'''
		delete obj from the self.obj dictionnary
		'''
		#TODO: check if key really exists return False
		#TODO: delete the key:data and return True
		del self.obj[obj];

	def clrObj(self):
		'''
		empty the dictionnary
		'''
		self.obj = {}
		self.i=0

	def getRandom(self, weighted=False):
		'''
		will return a random entry dagMesh from the dictionnary.
		will return a weighted random entry using the proba attributes from each entry if the weighted boolean parameter is set
		will return None if the method was unsuccessful to retrieve the selected object (if object was deleted from the scene while the script was running for example)
		'''
		#TODO: implement weight and boolean flag
		dkeys = self.obj.keys()
		dag = self.obj[dkeys[rand.randint(0,len(dkeys)-1)]]
		return dag[0];

	def getNext(self):
		'''
		will return the dagMesh of the next entry in the dictionnary. will increment the index by 1 (index will be calculated modulo the dictionnary length will polling for the next entry).
		will return None if the method was unsuccessful to retrieve the selected object (if object was deleted from the scene while the script was running for example)
		'''
		#TODO: implement boolean flag
		dkeys = self.obj.keys()
		dkeys.sort()
		dag = self.obj[dkeys[self.i%len(dkeys)]]
		self.i +=1
		return dag[0];





class sp3derror (object):
	'''
	class used to raise script error during the execution and display runtime info to the user in the bottom textfield of the main UI
	'''
	def __init__(self, initerror, uifield):
		'''
		initialise default attributes
		'''
		self.error = initerror
		self.ui = uifield
		self.broadcastError();

	def broadcastError(self):
		'''
		display the error in the proper field in the UI (self.uiInfoTextField)
		'''
		mc.textField(self.ui, edit=True, text=self.error)

	def raiseError(self, newerror):
		'''
		update the error and call out to display it in the main UI
		'''
		self.error = newerror
		self.broadcastError();


class spPaint3dWin (object):
	'''
	Main UI window class definition
	'''
	def __init__(self):
		#delete ui window if opened
		if mc.window(spPaint3dGuiID, exists=True):
			mc.deleteUI(spPaint3dGuiID);
		if mc.windowPref(spPaint3dGuiID, exists=True):
			mc.windowPref(spPaint3dGuiID, remove=True);

		#delete option window if opened
		if mc.window(spPaint3dSetupID, exists=True):
			mc.deleteUI(spPaint3dSetupID);

		self.mayaVersion = getMayaVersion()

		self.uiWin = mc.window(spPaint3dGuiID, title=("spPaint3d | "+str(spPaint3dVersion)),width=255, resizeToFitChildren=False, sizeable=False, titleBar=True,minimizeButton=False,maximizeButton=False,menuBar=False,menuBarVisible=False,toolbox=True)

		self.uiTopColumn = mc.columnLayout(adjustableColumn=True, columnAttach=('both',5))

		#----------------------
		# Top buttons
		#----------------------
		self.uiTopForm = mc.formLayout(numberOfDivisions=100)
		self.uiBtnHelp = mc.button(label='Help', command=lambda *args:self.uiButtonCallback("uiBtnHelp", args))
		self.uiBtnOptions = mc.button(label='Options', command=lambda *args:self.uiButtonCallback("uiBtnOptions", args))

		mc.formLayout(self.uiTopForm, edit=True, attachControl=[(self.uiBtnOptions, 'left', 5, self.uiBtnHelp)])

		mc.setParent(self.uiTopColumn)

		#----------------------
		# Source Frame
		#----------------------
		self.uiSourceFrame = mc.frameLayout(label='Brush Geometry', cll=True, collapseCommand=lambda:self.resizeWindow('collapse',98), expandCommand=lambda:self.resizeWindow('expand',98), mh=5, mw=5, borderStyle='etchedIn')
		self.uiSourceForm = mc.formLayout(numberOfDivisions=100, width=255)
		self.uiSourceList = mc.textScrollList(numberOfRows=5, allowMultiSelection=True, width=215)
		self.uiSourceBtnAdd = mc.symbolButton(w=60, h=18, ann='Add selected object(s) to the list', image='sp3dadd.xpm', command=lambda *args:self.uiListCallback("add", "uiSourceList"))
		self.uiSourceBtnRem = mc.symbolButton(w=60, h=18, ann='Remove selected object(s) from the list', image='sp3drem.xpm', command=lambda *args:self.uiListCallback("rem", "uiSourceList"))
		self.uiSourceBtnClr = mc.symbolButton(w=60, h=18, ann='Clear the list', image='sp3dclr.xpm', command=lambda *args:self.uiListCallback("clr", "uiSourceList"))

		mc.formLayout(self.uiSourceForm, edit=True, attachForm=[(self.uiSourceList,'top',0)], attachControl=[(self.uiSourceBtnAdd,'top',3, self.uiSourceList),(self.uiSourceBtnRem, 'left', 5, self.uiSourceBtnAdd),(self.uiSourceBtnRem, 'top', 3, self.uiSourceList),(self.uiSourceBtnClr, 'left', 5, self.uiSourceBtnRem),(self.uiSourceBtnClr, 'top', 3, self.uiSourceList)])

		mc.setParent(self.uiTopColumn)

		#----------------------
		# Transform Setup
		#----------------------
		self.uiTransformFrame = mc.frameLayout(label='Transform Setup', cll=True, collapseCommand=lambda:self.resizeWindow('collapse',177), expandCommand=lambda:self.resizeWindow('expand',177), mh=5, mw=5, borderStyle='etchedIn')
		self.uiTransformForm = mc.formLayout(numberOfDivisions=100, width=255)
		self.uiTransformRotateCheck = mc.checkBox(label='Rotate', ann='Activate the rotate transform while painting', changeCommand=lambda *args:self.uiCheckBoxCallback("transformRotate", args))
		self.uiTransformRotateFieldX = mc.floatFieldGrp(numberOfFields=2, label='Min', backgroundColor=(.81,.24,0), extraLabel='Max', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), changeCommand=lambda *args:self.uiTransformCallback())
		self.uiTransformRotateFieldY = mc.floatFieldGrp(numberOfFields=2, label='', backgroundColor=(.41,.75,0), extraLabel='', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), changeCommand=lambda *args:self.uiTransformCallback())
		self.uiTransformRotateFieldZ = mc.floatFieldGrp(numberOfFields=2, label='', backgroundColor=(.17,.4,.63), extraLabel='', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), changeCommand=lambda *args:self.uiTransformCallback())
		self.uiTransformSeparator = mc.separator(height=5,width=250, style='in')
		self.uiTransformScaleCheck = mc.checkBox(label='Scale', ann='Activate the scale transform while painting', changeCommand=lambda *args:self.uiCheckBoxCallback("transformScale", args))
		self.uiTransformScaleUniformCheck = mc.checkBox(label='Uniform', ann='Force uniform scale while painting', changeCommand=lambda *args:self.uiCheckBoxCallback("transformScaleUniform", args))
		self.uiTransformScaleFieldX = mc.floatFieldGrp(numberOfFields=2, label='Min', backgroundColor=(.81,.24,0), extraLabel='Max', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), v1=1.0, v2=1.0, changeCommand=lambda *args:self.uiTransformCallback())
		self.uiTransformScaleFieldY = mc.floatFieldGrp(numberOfFields=2, label='', backgroundColor=(.41,.75,0), extraLabel='', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), v1=1.0, v2=1.0, changeCommand=lambda *args:self.uiTransformCallback())
		self.uiTransformScaleFieldZ = mc.floatFieldGrp(numberOfFields=2, label='', backgroundColor=(.17,.4,.63), extraLabel='', cw4=(22,50,50,30), precision=2, ct4=('right', 'both', 'both', 'right'), co4=(2,2,2,8), v1=1.0, v2=1.0, changeCommand=lambda *args:self.uiTransformCallback())

		mc.formLayout(self.uiTransformForm, edit=True,
			attachForm=[(self.uiTransformRotateCheck,'top',4), (self.uiTransformRotateFieldX,'top',0)],
			attachControl=[	(self.uiTransformRotateFieldX, 'left',10,self.uiTransformRotateCheck),
							(self.uiTransformRotateFieldY,'top',5,self.uiTransformRotateFieldX), (self.uiTransformRotateFieldY,'left',10,self.uiTransformRotateCheck),
							(self.uiTransformRotateFieldZ,'top',5,self.uiTransformRotateFieldY), (self.uiTransformRotateFieldZ,'left',10,self.uiTransformRotateCheck),
							(self.uiTransformSeparator,'top',5,self.uiTransformRotateFieldZ), (self.uiTransformScaleCheck, 'top',5,self.uiTransformSeparator), (self.uiTransformScaleUniformCheck, 'top',5,self.uiTransformScaleCheck),
							(self.uiTransformScaleFieldX, 'top',5, self.uiTransformSeparator),(self.uiTransformScaleFieldX, 'left',15, self.uiTransformScaleCheck),
							(self.uiTransformScaleFieldY, 'top',5, self.uiTransformScaleFieldX),(self.uiTransformScaleFieldY, 'left',15, self.uiTransformScaleCheck),
							(self.uiTransformScaleFieldZ, 'top',5, self.uiTransformScaleFieldY),(self.uiTransformScaleFieldZ, 'left',15, self.uiTransformScaleCheck),
							])

		mc.setParent(self.uiTopColumn)
		self.transform = sp3dTransform()

		#----------------------
		# Target Surface(s)
		#----------------------
		self.uiTargetFrame = mc.frameLayout(label='Target Surface(s)', cll=True, collapseCommand=lambda:self.resizeWindow('collapse',98), expandCommand=lambda:self.resizeWindow('expand',98), mh=5, mw=5, borderStyle='etchedIn')
		self.uiTargetForm = mc.formLayout(numberOfDivisions=100, width=255)
		self.uiTargetList = mc.textScrollList(numberOfRows=5, allowMultiSelection=True, width=215)
		self.uiTargetBtnAdd = mc.symbolButton(w=60, h=18, ann='Add selected object(s) to the list', image='sp3dadd.xpm', command=lambda *args:self.uiListCallback("add", "uiTargetList"))
		self.uiTargetBtnRem = mc.symbolButton(w=60, h=18, ann='Remove selected object(s) from the list', image='sp3drem.xpm', command=lambda *args:self.uiListCallback("rem", "uiTargetList"))
		self.uiTargetBtnClr = mc.symbolButton(w=60, h=18, ann='Clear the list', image='sp3dclr.xpm', command=lambda *args:self.uiListCallback("clr", "uiTargetList"))

		mc.formLayout(self.uiTargetForm, edit=True, attachForm=[(self.uiTargetList,'top',0)], attachControl=[(self.uiTargetBtnAdd,'top',3, self.uiTargetList),(self.uiTargetBtnRem, 'left', 5, self.uiTargetBtnAdd),(self.uiTargetBtnRem, 'top', 3, self.uiTargetList),(self.uiTargetBtnClr, 'left', 5, self.uiTargetBtnRem),(self.uiTargetBtnClr, 'top', 3, self.uiTargetList)])

		mc.setParent(self.uiTopColumn)

		#----------------------
		# Paint Contexts
		#----------------------
		self.uiPaintFrame = mc.frameLayout(label='Paint', cll=True, collapseCommand=lambda:self.resizeWindow('collapse',88), expandCommand=lambda:self.resizeWindow('expand',88), mh=5, mw=5, borderStyle='etchedIn')
		self.uiPaintForm = mc.formLayout(numberOfDivisions=100, width=255)
		self.uiPaintDupSCB = mc.symbolCheckBox(w=52, h=18, ann='Duplicate: Instance or Copy', ofi='sp3dduplicate.xpm', oni='sp3dinstance.xpm', changeCommand=lambda *args:self.uiCheckBoxCallback("instance", args))
		self.uiPaintRandSCB = mc.symbolCheckBox(w=52, h=18, ann='Object distribution: Random or Sequential', ofi='sp3dsequence.xpm', oni='sp3drandom.xpm', changeCommand=lambda *args:self.uiCheckBoxCallback("random", args))
		self.uiPaintAlignSCB = mc.symbolCheckBox(w=100, h=18, ann='Align generated objects to the target surface', ofi='sp3dalignoff.xpm', oni='sp3dalign.xpm', changeCommand=lambda *args:self.uiCheckBoxCallback("align", args))
		self.uiPaintCtxBtn = mc.symbolButton(w=105, h=28, ann='Paint', image='sp3dpaint.xpm', command=lambda *args:self.genericContextCallback("PaintCtx"))
		self.uiPlaceCtxBtn = mc.symbolButton(w=105, h=28, ann='Place', image='sp3dplace.xpm', command=lambda *args:self.genericContextCallback("PlaceCtx"))
		self.uiPaintTimer = mc.floatSliderGrp(label='Sensibility', field=1, minValue=0.0, maxValue=0.2, fieldMaxValue=1.0, precision=2, vis=False, w=250, cw=[(1,55), (2,35)],changeCommand=lambda *args:self.uiFluxCallback("paintTimer", args))
		self.uiPaintDistance = mc.floatFieldGrp(label='Distance threshold', precision=2, vis=False, w=250, cw=[(1,100), (2,50)],changeCommand=lambda *args:self.uiFluxCallback("paintDistance", args))

		mc.formLayout(self.uiPaintForm, edit=True,
						attachForm=[(self.uiPaintDupSCB, 'top', 0)],
						attachControl=[	(self.uiPaintRandSCB, 'left', 5, self.uiPaintDupSCB), (self.uiPaintAlignSCB, 'left', 5, self.uiPaintRandSCB),
						 				(self.uiPaintCtxBtn, 'top', 5, self.uiPaintDupSCB), (self.uiPlaceCtxBtn, 'top', 5, self.uiPaintDupSCB), (self.uiPlaceCtxBtn, 'left', 5, self.uiPaintCtxBtn),
										(self.uiPaintTimer, 'top', 5, self.uiPaintCtxBtn), (self.uiPaintDistance, 'top', 5, self.uiPaintCtxBtn)])

		mc.setParent(self.uiTopColumn)

		#----------------------
		# Footer
		#----------------------
		self.uiInfoFrame = mc.frameLayout(borderStyle='etchedIn', labelVisible = False)
		self.uiInfoTextField = mc.textField(editable=False)
		self.errorHandle = sp3derror((spPaint3dGuiID + ' ' + str(spPaint3dVersion) + ' | Sebastien Paviot - 2009'), self.uiInfoTextField)

		#----------------------
		# Update UI controls
		#----------------------
		self.uiValues = sp3dToolOption()
		self.updateUIControls(self.uiValues)

		#----------------------
		# Source & Target object lists
		#----------------------
		self.sourceList = sp3dObjectList('source')
		self.targetList = sp3dObjectList('target')

		#----------------------
		# Context tracking
		#----------------------
		self.ctx = None

		mc.showWindow(self.uiWin)
		self.resizeWindow('winui',620) # force a resize to prevent some weird UI issue on mac
		if(sp3d_log): self.debugFrameSize() #display actual corrected ui frame sizes


	def uiTransformCallback(self, *args):
		'''
		rebuild the transform combo tuple and feed the class when changed in the UI
		'''
		rotate = ((mc.floatFieldGrp(self.uiTransformRotateFieldX, q=True, v1=True),mc.floatFieldGrp(self.uiTransformRotateFieldX, q=True, v2=True)), (mc.floatFieldGrp(self.uiTransformRotateFieldY, q=True, v1=True),mc.floatFieldGrp(self.uiTransformRotateFieldY, q=True, v2=True)), (mc.floatFieldGrp(self.uiTransformRotateFieldZ, q=True, v1=True),mc.floatFieldGrp(self.uiTransformRotateFieldZ, q=True, v2=True)))
		scale = ((mc.floatFieldGrp(self.uiTransformScaleFieldX, q=True, v1=True),mc.floatFieldGrp(self.uiTransformScaleFieldX, q=True, v2=True)), (mc.floatFieldGrp(self.uiTransformScaleFieldY, q=True, v1=True),mc.floatFieldGrp(self.uiTransformScaleFieldY, q=True, v2=True)), (mc.floatFieldGrp(self.uiTransformScaleFieldZ, q=True, v1=True),mc.floatFieldGrp(self.uiTransformScaleFieldZ, q=True, v2=True)))
		self.transform = sp3dTransform(rotate, scale)
		self.updateCtx();


	def uiTransformReset(self):
		'''
		reset transform values and update UI
		'''
		self.transform = sp3dTransform()
		mc.floatFieldGrp(self.uiTransformRotateFieldX, e=True, v1=0, v2=0)
		mc.floatFieldGrp(self.uiTransformRotateFieldY, e=True, v1=0, v2=0)
		mc.floatFieldGrp(self.uiTransformRotateFieldZ, e=True, v1=0, v2=0)
		mc.floatFieldGrp(self.uiTransformScaleFieldX, e=True, v1=1, v2=1)
		mc.floatFieldGrp(self.uiTransformScaleFieldY, e=True, v1=1, v2=1)
		mc.floatFieldGrp(self.uiTransformScaleFieldZ, e=True, v1=1, v2=1);


	def uiFluxCallback(self, *args):
		'''
		Callback for timer slider and distance float field change
		INPUT: [variable name, (value to update,)]
		'''
		self.uiValues.__dict__[args[0]] = float(args[1][0])
		self.uiValues.commitVars()
		self.updateCtx();


	def uiButtonCallback(self, *args):
		'''
		Callback for top buttons
		INPUT: [variable name]
		'''
		button=args[0]
		if (button=='uiBtnHelp'):
			mc.confirmDialog(title=spPaint3dGuiID + ' ' + str(spPaint3dVersion) + ' Help', message='Please refer to the included spPaint3d_ReadMe.html file for detailed help on how to use the script.\n Or use the Homepage button in the Options.', button='Whatever');
		elif (button=='uiBtnOptions'):
			self.setupWin(self.uiValues);

	def uiCheckBoxCallback(self, *args):
		'''
		Callback for checkbox and symbolCheckbox
		INPUT: [variable name, (string value for bool state,)]
		'''
		if (sp3d_log): print ('input from UI: %s of type %s' % (args, args[1][0].__class__))
		self.uiValues.__dict__[args[0]] = getBoolFromMayaControl(args[1][0], self.mayaVersion)
		self.uiValues.commitVars()
		self.updateCtx();


	def updateUIControls(self,ui):
		'''
		Will update the self ui controls with the values stores in the passed instance object
		'''
		mc.checkBox(self.uiTransformRotateCheck, edit=True, value=ui.transformRotate)
		mc.checkBox(self.uiTransformScaleCheck, edit=True, value=ui.transformScale)
		mc.checkBox(self.uiTransformScaleUniformCheck, edit=True, value=ui.transformScaleUniform)
		mc.symbolCheckBox(self.uiPaintDupSCB, edit=True, value=ui.instance)
		mc.symbolCheckBox(self.uiPaintRandSCB, edit=True, value=ui.random)
		mc.symbolCheckBox(self.uiPaintAlignSCB, edit=True, value=ui.align)

		# toggling the proper paint flux control
		mc.floatSliderGrp(self.uiPaintTimer, edit=True, visible=(not ui.paintFlux), value=ui.paintTimer)
		mc.floatFieldGrp(self.uiPaintDistance, edit=True, visible=ui.paintFlux, v1=ui.paintDistance)

	def debugFrameSize(self):
		'''
		Will query and print the actual size of the frame to collapse. Will already remove 21 from the total height, so ready to use with the resizeWindow onCollapse command
		'''
		print('(height) sourceFrame : %i || transformFrame : %i || targetFrame : %i || paintFrame : %i ||\n' % (mc.frameLayout(self.uiSourceFrame, query=True, height=True)-21, mc.frameLayout(self.uiTransformFrame, query=True, height=True)-21, mc.frameLayout(self.uiTargetFrame, query=True, height=True)-21, mc.frameLayout(self.uiPaintFrame, query=True, height=True)-21))

	def resizeWindow(self,direction,offset):
 		'''
		Will adjust the size of the window, used when collapsing/expanding frames
		'''
		if mc.window(spPaint3dGuiID, exists=True):
			currentSize = mc.window(self.uiWin, query=True, height=True);
			if (direction=='collapse'):
				currentSize -= offset
			elif (direction=='expand'):
				currentSize += offset
			elif (direction=='winui'):
				currentSize=offset
			if(currentSize>0):
				mc.window(self.uiWin, edit=True, height=currentSize);

		#self.debugFrameSize();

	def updateCtx(self):
		'''
		method to check if there's any running context to update
		'''
		if (self.ctx):
			#there's a context object that can be updated
			self.ctx.runtimeUpdate(self.uiValues, self.transform, self.sourceList, self.targetList);


	def uiListCallback(self,*args):
		'''
		textScrollList callback to manage the addition/removal/reset of the source & target object lists
		'''
		#print "listStuff: " + str(args)
		mode = args[0]
		textlist = args[1]
		if (textlist=='uiSourceList'): objlist = 'sourceList';
		else: objlist = 'targetList';

		if (mode=='add'):
			#ADD
			objselected = mc.ls(selection=True)
			for obj in objselected:
				addresult, addcomment = self.__dict__[objlist].addObj(obj)
				if (addresult==None):
					#an exception occured, printing debut info
					self.errorHandle.raiseError(addcomment)
				else:
					#an object was added to the dict, to be added to the UI list
					if (mc.objExists(addresult)):
						#making sure the object does exist
						mc.textScrollList(self.__dict__[textlist], edit=True, append=addresult)
						#print ('added succesfully:', addresult);

		elif(mode=='clr'):
			#CLEAR
			mc.textScrollList(self.__dict__[textlist], edit=True, removeAll=True)
			self.__dict__[objlist].clrObj()

		elif(mode=='rem'):
			#REMOVE
			remlist = mc.textScrollList(self.__dict__[textlist], query=True, selectItem=True)
			if(remlist):
				#list is not empty
				for remobj in remlist:
					#iterate through all the selected obj to remove
					self.__dict__[objlist].delObj(remobj)
					mc.textScrollList(self.__dict__[textlist], edit=True, removeItem=remobj);


		#self.__dict__[objlist].printObj()
		self.updateCtx();




	def genericContextCallback(self, *args):
		'''
		Handle the paint/place context button callbacks
		'''
		#print "genericCallback called: " + str(args)

		#delete the setup option UI if it's opened
		if mc.window(spPaint3dSetupID, exists=True):
			mc.deleteUI(spPaint3dSetupID);

		#validate the objects from both lists and raise an error if necessary
		sourcevalid = self.sourceList.validateObjects()
		targetvalid = self.targetList.validateObjects()
		duplicateerror = self.sourceList.hasDuplicate(self.targetList)

		if(not sourcevalid):
			self.errorHandle.raiseError("Source list is empty or object(s) have been deleted. FIX!");
		elif(not targetvalid):
			self.errorHandle.raiseError("Target list is empty or object(s) have been deleted. FIX!");
		elif(duplicateerror):
			self.errorHandle.raiseError("Object(s) can't be in both lists, FIX!");
		else:
			#if we reach here, then there seem to be no errors caught
			#call the appropriate context and set self attributes
			if (args[0]=='PaintCtx'):
				#creating (or overwritring with) a paint context
				self.errorHandle.raiseError("Engage!! Maximum Paint...")
				self.ctx = spPaint3dContext.paintContext(self.uiValues, self.transform, self.sourceList, self.targetList)
				self.ctx.runContext();
			elif (args[0]=='PlaceCtx'):
				#creating (or overwritring with) a place context
				self.errorHandle.raiseError("Engage!! Maximum Place...")
				self.ctx = spPaint3dContext.placeContext( self.uiValues, self.transform, self.sourceList, self.targetList)
				self.ctx.runContext();


	def setupWin(self, uiOptions):
		'''
		Create setup UI
		'''
		self.uiSetupWin = mc.window(spPaint3dSetupID, title=("spPaint3dSetup | "+str(spPaint3dVersion)),width=200, resizeToFitChildren=False, sizeable=False, titleBar=True,minimizeButton=False,maximizeButton=False,menuBar=False,menuBarVisible=False,toolbox=True)

		self.uiSetupTopColumn = mc.columnLayout(adjustableColumn=True, columnAttach=('both',5))
		self.uiSetupTopForm = mc.formLayout(numberOfDivisions=100)
		self.uiSetupBtnHelp = mc.button(label='Help', command=lambda *args:self.setupButtonCallback('uiSetupBtnHelp', args))
		self.uiSetupBtnHomepage = mc.button(label='Homepage', command=lambda *args:self.setupButtonCallback('uiSetupBtnHomepage', args))
		self.uiSetupBtnReset = mc.button(label='Reset', command=lambda *args:self.setupButtonCallback('uiSetupBtnReset', args))

		mc.formLayout(self.uiSetupTopForm, edit=True, attachControl=[(self.uiSetupBtnHomepage, 'left', 5, self.uiSetupBtnHelp),(self.uiSetupBtnReset, 'left',5,self.uiSetupBtnHomepage)])

		mc.setParent(self.uiSetupTopColumn)

		self.uiSetupDuplicateFrame = mc.frameLayout(label='Duplicate Options', borderStyle='etchedIn', marginHeight=5, marginWidth=20)
		self.uiSetupDuplicateForm = mc.formLayout(numberOfDivisions=100)
		self.uiSetupChkInputConn = mc.checkBoxGrp(label='Preserve input connections', changeCommand=lambda *args:self.setupCallback('uiSetupChkInputConn', args), numberOfCheckBoxes=1, width=170)

		mc.setParent(self.uiSetupTopColumn)

		self.uiSetupNormalFrame = mc.frameLayout(label='Surface Normals', borderStyle='etchedIn', marginHeight=5, marginWidth=20)
		self.uiSetupNormalForm = mc.formLayout(numberOfDivisions=100)
		self.uiSetupNormalCol = mc.radioCollection()
		self.uiSetupNormalSmooth = mc.radioButton(label='Geometry normal', align='right', onCommand=lambda *args:self.setupCallback('uiSetupNormalCol',True))
		self.uiSetupNormalHard = mc.radioButton(label='Force hard normal', align='right', onCommand=lambda *args:self.setupCallback('uiSetupNormalCol',False))

		mc.formLayout(self.uiSetupNormalForm, edit=True, attachControl=[(self.uiSetupNormalHard, 'top', 5, self.uiSetupNormalSmooth)])

		mc.setParent(self.uiSetupTopColumn)

		self.uiSetupFluxFrame = mc.frameLayout(label='Flux Control', borderStyle='etchedIn', marginHeight=5, marginWidth=20)
		self.uiSetupFluxForm = mc.formLayout(numberOfDivisions=100)
		self.uiSetupFluxCol = mc.radioCollection()
		self.uiSetupFluxTimer = mc.radioButton(label='Timer', align='right', onCommand=lambda *args:self.setupCallback('uiSetupFluxCol', False))
		self.uiSetupFluxDistance = mc.radioButton(label='Distance threshold', align='right', onCommand=lambda *args:self.setupCallback('uiSetupFluxCol', True))

		mc.formLayout(self.uiSetupFluxForm, edit=True, attachControl=[(self.uiSetupFluxDistance, 'top', 5, self.uiSetupFluxTimer)])

		mc.setParent(self.uiSetupTopColumn)

		self.uiSetupHierarchyFrame = mc.frameLayout(label='Hierarchy Management', borderStyle='etchedIn', marginHeight=5, marginWidth=20)
		self.uiSetupHierarchyForm = mc.formLayout(numberOfDivisions=100)
		self.uiSetupHierarchyActive = mc.checkBoxGrp(label='Activate objects grouping', changeCommand=lambda *args:self.setupCallback('uiSetupHierarchyActive', args), numberOfCheckBoxes=1)
		self.uiSetupHierarchyCol = mc.radioCollection()
		self.uiSetupHierarchySession = mc.radioButton(label='Single paint session group', align='right', onCommand=lambda *args:self.setupCallback('uiSetupHierarchySession', args))
		self.uiSetupHierarchyStroke = mc.radioButton(label='Stroke sorted group(s)', align='right', onCommand=lambda *args:self.setupCallback('uiSetupHierarchyStroke', args))
		self.uiSetupHierarchySource = mc.radioButton(label='Source sorted group(s)', align='right', onCommand=lambda *args:self.setupCallback('uiSetupHierarchySource', args))

		mc.formLayout(self.uiSetupHierarchyForm, edit=True, attachControl=[(self.uiSetupHierarchySession, 'top', 5, self.uiSetupHierarchyActive), (self.uiSetupHierarchyStroke, 'top', 5, self.uiSetupHierarchySession), (self.uiSetupHierarchySource, 'top', 5, self.uiSetupHierarchyStroke)])
		mc.formLayout(self.uiSetupHierarchyForm, edit=True, attachForm=[(self.uiSetupHierarchySession, 'left', 25), (self.uiSetupHierarchyStroke, 'left', 25), (self.uiSetupHierarchySource, 'left', 25)])

		mc.setParent(self.uiSetupTopColumn)

		self.updateUISetupControls(uiOptions);
		mc.showWindow(self.uiSetupWin)


	def updateUISetupControls(self,ui):
		'''
		Will update the self ui controls with the values stores in the passed instance object
		'''
		if(sp3d_log): print ui.__dict__
		mc.checkBoxGrp(self.uiSetupChkInputConn, edit=True, value1=ui.preserveConn)
		mc.radioButton(self.uiSetupNormalSmooth, edit=True, select=ui.smoothNormal)
		mc.radioButton(self.uiSetupNormalHard, edit=True, select=(not ui.smoothNormal))
		mc.radioButton(self.uiSetupFluxTimer, edit=True, select=(not ui.paintFlux))
		mc.radioButton(self.uiSetupFluxDistance, edit=True, select=ui.paintFlux)

		# toggling the proper hierarchy grouping options
		mc.checkBoxGrp(self.uiSetupHierarchyActive, edit=True, value1=ui.hierarchy)
		if(sp3d_log): print ("ui.hierarchy %s" % ui.hierarchy)
		if (ui.hierarchy):
			#toggling radio button enabled
			if(sp3d_log): print "toggling grouping option ON"
			mc.radioButton(self.uiSetupHierarchySession, edit=True, enable=True)
			mc.radioButton(self.uiSetupHierarchyStroke, edit=True, enable=True)
			mc.radioButton(self.uiSetupHierarchySource, edit=True, enable=True)
		else:
			#toggling radio button disable
			if(sp3d_log): print "toggling grouping option OFF"
			mc.radioButton(self.uiSetupHierarchySession, edit=True, enable=False)
			mc.radioButton(self.uiSetupHierarchyStroke, edit=True, enable=False)
			mc.radioButton(self.uiSetupHierarchySource, edit=True, enable=False)

		if(ui.group==0.0):
			mc.radioButton(self.uiSetupHierarchySession, edit=True, select=True)
		elif(ui.group==1.0):
			mc.radioButton(self.uiSetupHierarchyStroke, edit=True, select=True)
		elif(ui.group==2.0):
			mc.radioButton(self.uiSetupHierarchySource, edit=True, select=True)

	def resetOptions(self):
		'''
		Will reset to defaults the options
		'''
		#TODO: loop/reset the option
		#TODO: update setup window UI
		#TODO: callback main UI window for update
		#TODO: callback to context if active with new options
		self.uiValues.resetVars()
		self.updateUISetupControls(self.uiValues)
		self.updateUIControls(self.uiValues)
		self.uiTransformReset();

	def setupButtonCallback(self, *args):
		'''
		Manage top buttons commands
		'''
		button=args[0]
		if(button=='uiSetupBtnHelp'):
			mc.confirmDialog(title=spPaint3dGuiID + ' ' + str(spPaint3dVersion) + ' Help', message='Please refer to the included spPaint3d_ReadMe.html file for detailed help on how to use the script.\n Or use the Homepage button right there.', button='Whatever');
		elif(button=='uiSetupBtnHomepage'):
			mc.showHelp('http://www.creativecrash.com/maya/downloads/scripts-plugins/utility-external/misc/c/sppaint3d', absolute=True);
		elif(button=='uiSetupBtnReset'):
			self.resetOptions();

	def setupCallback(self, *args):
		'''
		Manage checkbox and radio buttons
		uiNormalCol
		uiFluxCol
		uiSetupHierarchyActive
		'''
		radiocol=args[0]
		if (sp3d_log): print ('setupCallback control:%s | value: %s' % (args[0], args[1]))
		if(radiocol=='uiSetupNormalCol'):
			self.uiValues.smoothNormal=args[1];
		elif(radiocol=='uiSetupFluxCol'):
			self.uiValues.paintFlux=args[1];
		elif(radiocol=='uiSetupChkInputConn'):
			#Maya callback sends a tuple back for checkbox but seems not a boolean and has to be processed???
			self.uiValues.preserveConn=getBoolFromMayaControl(args[1][0], self.mayaVersion)
		elif(radiocol=='uiSetupHierarchyActive'):
			self.uiValues.hierarchy=getBoolFromMayaControl(args[1][0], self.mayaVersion)

		elif(radiocol=='uiSetupHierarchySession'):
			self.uiValues.group=0.0
		elif(radiocol=='uiSetupHierarchyStroke'):
			self.uiValues.group=1.0
		elif(radiocol=='uiSetupHierarchySource'):
			self.uiValues.group=2.0
		else:
			print args

		self.uiValues.commitVars()
		self.updateUIControls(self.uiValues)
		self.updateUISetupControls(self.uiValues)
		if(sp3d_log): print "done updating SetupUI\n"

#-----------------------------------------------------------------------------------
#	UTILITIES
#-----------------------------------------------------------------------------------

def getBoolFromMayaControl(uicontrol,version):
	'''
	return the bool state of the passed uicontrol, return false by default
	Maya returns control state as string pre-2011 and as bool 2011+
	'''
	state=False
	if (version-2011 >= 0):
		#at least a maya 2011
		state = uicontrol
	else:
		#pre2011, UI controls were returning the state as string and not as a bool prior to 2011
		if(uicontrol=='true'):
			state = True;
	return state


def getMayaVersion():
	'''
	attempt to detect the version of maya and return it as a numerical value.
	'''
	version=mc.about(v=True)
	supportedversion=False
	while not supportedversion:
		try:
			version = int(version[:4])
			supportedversion=True
		except ValueError:
			result=mc.promptDialog(title='Enter Maya version', message='Couldn\'t determine the version of Maya\n, please enter the 4 digits of the maya version you are using (ie: 2011)', button=['OK', 'Cancel & Quit'], defaultButton='OK', cancelButton='Cancel & Quit', dismissString='Cancel & Quit')
			if result == 'OK':
				version=mc.promptDialog(query=True, text=True)
			else:
				sys.exit()
	if (version<=2010): mc.confirmDialog(title='Maya version alert', message='This version of the script was updated for Maya 2011 and above.\nThere will be unexpected stuff happening with older versions, or not...')
	return version


def getDAGPath(node,depth=False):
	'''
	Return the DAG path of the node argument (node must be a transform, if not, will attempt to locate its immediate parent and make sure it's a transform and will proceed from there)
	Return the extended DAG path to the node's shape when depth=True
	Return None if the node doesn't have any shape children, or more than one children shape.
	'''
	dag = None
	nodetype = mc.objectType(node);

	if(nodetype!='transform'):
		#node is not a transform, will proceed upstream to its immediate parent and will verify if the parent is a transform
		tempdag = mc.listRelatives(node, parent=True)
		if(tempdag):
			#node has a parent
			if(len(tempdag)==1):
				#node only has 1 parent, will proceed from there and see if it's a transform with a shape' child
				node=tempdag[0]
				nodetype = mc.objectType(node);

	if(nodetype=='transform'):
		#node is a transform, making sure it has only one children of shape type
		childlist = mc.listRelatives(node, children=True, shapes=True)
		if(childlist):
			if(len(childlist)==1):
				#there's only 1 shape below the transform
				if(depth): dag = mc.listRelatives(node, fullPath=True, shapes=True);
				else: dag = mc.listRelatives(node, path=True, shapes=True);

	if(dag):
		return dag[0]
	else:
		return dag
