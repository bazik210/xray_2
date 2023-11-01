#-----------------------------------------------------------------
#	SCRIPT			spPaint3dContext.py
#	AUTHOR			Sebastien Paviot
#				    spaviot@gmail.com
#	DATE:			July,August 2009 - April 2010
#
#	DESCRIPTION:	Define tool contexts
#
#	VERSION:	    2011.0
#
#-----------------------------------------------------------------

#used for maya versions using pre-2.6 python engine
from __future__ import with_statement
###################################################

import maya.cmds as mc

import maya.OpenMaya as om
import maya.OpenMayaUI as omui
import math as math
import sys

spPaint3dContextID = "spPaint3dContext";
spPaint3dTempGroupID = "spPaint3dTempGroup";

#unit conversion dictionnary relative to 1 cm (default unit system)
sp3dUnit = {
					"mm": 10,
					"cm": 1,
					"m": 0.01,
					"in": 0.393701,
					"ft": 0.0328084,
					"yd": 0.0109361,};

sp3d_dbgfile = "C:\\sp3ddbg_log.txt"
sp3d_dbg = False #debug flag to log to file
sp3d_log = False #debug flag to log to script editor log
sp3d_MFn = False #debug flag for MFn stuff

class point (object):
	'''
	define an object of 3 attributes used for various purposes
	'''
	def __init__(self, x, y, z):
		'''
		initialise object attributes
		'''
		self.x = x
		self.y = y
		self.z = z

	def asMPoint(self):
		'''
		return point as MPoint()
		'''
		return om.MPoint(self.x,self.y,self.z);

	def asMFPoint(self):
		'''
		return point as MFloatPoint()
		'''
		return om.MFloatPoint(self.x,self.y,self.z);

	def asMVector(self):
		'''
		return point as MVector()
		'''
		return om.MVector(self.x,self.y,self.z);

	def asMFVector(self):
		'''
		return point as MFloatVector()
		'''
		return om.MFloatVector(self.x,self.y,self.z);



class intersectionPoint (object):
	'''
	define an intersection point
	'''
	def __init__(self, hitPoint, hitFace, hitTriangle, dagMesh):
		'''
		initial setup
		'''
		self.valid = None        #bool to validate iteration later on
		self.hitPoint = hitPoint 	#point.position tuple
		self.hitFace = hitFace 		#face number of the intersection
		self.hitTriangle = hitTriangle	#triangle number in the above face
		self.dagMeshTargetSurface = dagMesh		#MDagPath where the intersection occured (used to avoid having to compute all normals on soon-to-be discarded intersections
		self.timestamp = None 	#used to track the creation of an object at that intersectionPoint (later used in the strokePointList class)
		self.dagMeshSourceObject = None 	#used to store the DAG path of the created geometry if it's actually a valid intersection
		self.generatedDAG = None	#used to store the DAG path of the created object

	def getHitNormal(self, smooth=False):
		'''
		return the normal (MVector) at the self.hitPoint, compute the normal differently according to the smooth boolean argument
		'''
		if (smooth):
			#getting the intersection normal from the MFnMesh method
			normal = om.MVector()
			fnMesh = om.MFnMesh( self.dagMeshTargetSurface )
			fnMesh.getClosestNormal(self.hitPoint.asMPoint(), normal, om.MSpace.kWorld, None);

			return normal;
		else:
			#compute hard normal (decal mode)
			hitFacept =om.MScriptUtil()
			om.MScriptUtil().setInt(hitFacept.asIntPtr(),self.hitFace)
			itMesh = om.MItMeshPolygon(self.dagMeshTargetSurface)
			itMesh.setIndex(self.hitFace, hitFacept.asIntPtr())
			triVertsArray = om.MPointArray()
			triIndexArray = om.MIntArray()
			itMesh.getTriangle(self.hitTriangle, triVertsArray, triIndexArray, om.MSpace.kWorld)

			return self.getCrossProduct(triVertsArray[0],triVertsArray[1],triVertsArray[2]);

	def convertUnit(self, currentUnit):
		'''
		will convert the stored intersection internal coordinates into the current unit of the scene
		'''
		if(currentUnit!='cm'):
			self.hitPoint.x = float(mc.convertUnit(self.hitPoint.x, fromUnit='cm', toUnit=currentUnit))
			self.hitPoint.y = float(mc.convertUnit(self.hitPoint.y, fromUnit='cm', toUnit=currentUnit))
			self.hitPoint.z = float(mc.convertUnit(self.hitPoint.z, fromUnit='cm', toUnit=currentUnit))

	def getCrossProduct(self, p1, p2, p3):
		'''
		return the cross product from the 3 points
		'''
		vectA = om.MVector( (p2.x-p1.x), (p2.y-p1.y), (p2.z-p1.z) )
		vectB = om.MVector( (p3.x-p1.x), (p3.y-p1.y), (p3.z-p1.z) )
		vectA.normalize()
		vectB.normalize()
		return self.doCrossProduct(vectA, vectB);


	def doCrossProduct(self, v1, v2):
		'''
		compute the cross product vectors
		'''
		vectNorm = om.MVector( ((v1.y*v2.z) - (v1.z*v2.y)) , ((v1.z*v2.x) - (v1.x*v2.z)), ((v1.x*v2.y) - (v1.y*v2.x)) )
		vectNorm.normalize()
		return vectNorm;

	def startTimer(self):
		'''
		start the timerX for the current object
		'''
		self.timestamp = mc.timerX();

	def updateDAGSourceObject(self, dagstring):
		'''
		update self.dagMeshSourceObject with the dagstring argument
		'''
		self.dagMeshSourceObject = dagstring

	def createdObjectDAG(self, dagstring):
		'''
		update the self.generatedDAG with the dagstring argument
		'''
		self.generatedDAG = dagstring;
	
	def isValid(self,confirm=None):
		'''
		put the valid flag to the passed value. return the current flag state 
		'''
		if confirm != None: self.valid=confirm
		return self.valid




class intersectionList (object):
	'''
	define list of intersection points
	'''
	def __init__(self, ipoint=None):
		'''
		initial setup
		input: point object, (optional point object)
		'''
		self.intersectionList = []
		if (ipoint): self.addPoint(ipoint)
		if(sp3d_log): print ("creating a new intersectionList (empty? %s)" % self.intersectionList);

	def addPoint(self, ipoint):
		'''
		add a point to the list
		'''
		self.intersectionList.append(ipoint)
		if(sp3d_log):
			print ("adding a new intersection to intersectionList (length of list: %i)" % len(self.intersectionList))
		#	self.printList();


	def getClosest(self, sortorigin):
		'''
		will parse the list to return the closest intersectionPoint to the sortorigin argument
		return None if the list is empty
		'''
		length = len(self.intersectionList)
		closestintersection = None
		closestdistance = None

		if (length==0): return None
		elif (length==1): return self.intersectionList[0]
		else:
			#list is at least 2 intersections
			for intersection in self.intersectionList:
				if not closestintersection:
					#first intersection considered
					closestintersection = intersection
					closestdistance = getDistanceBetween(intersection.hitPoint, sortorigin)
				else:
					#compare distances
					newdistance = getDistanceBetween(intersection.hitPoint, sortorigin)
					if ( newdistance < closestdistance):
						#new closest
						closestintersection = intersection
						closestdistance = newdistance
			#at this point there must be a closesintersetcion
			return closestintersection;

	def printList(self):
		'''
		debug: print the dag of all objects in the list
		'''
		for obj in self.intersectionList:
			print ("object created: %s (using source: %s)" % (obj.generatedDAG, obj.dagMeshSourceObject))




class paintContext (object):
	'''
	define paintContext
	'''
	def __init__(self, uioptions, transformoptions, sourcelist, targetlist):
		'''
		initial setup
		'''
		#create the tool context
		if (mc.draggerContext(spPaint3dContextID, exists=True)):
			mc.deleteUI(spPaint3dContextID);
		mc.draggerContext(spPaint3dContextID, pressCommand=self.onPress, dragCommand=self.onDrag, releaseCommand=self.onRelease, name=spPaint3dContextID, cursor='crossHair', undoMode='step')

		#create context local options
		self.runtimeUpdate(uioptions, transformoptions, sourcelist, targetlist)

		#debug purpose
		self.reentrance = 0

		#initialise world up vector
		if ( (mc.upAxis(q=True, axis=True)) == "y" ):
			self.worldUp = om.MVector (0,1,0);
		elif ( (mc.upAxis(q=True, axis=True)) == "z" ):
			self.worldUp = om.MVector (0,0,1);
		else:
			#can't figure out up vector
			mc.confirmDialog(title='Weird stuff happening', message='Not getting any proper info on what the current up vector is. Quitting...')
			sys.exit()

		#fetch current scene unit
		self.unit = mc.currentUnit(query=True, linear=True)


	def runContext(self):
		'''
		set maya tool to the context
		'''
		if (mc.draggerContext(spPaint3dContextID, exists=True)): mc.setToolTo(spPaint3dContextID);

	def onPress(self):
		'''
		on mouse press initial event
		'''
		if(sp3d_dbg): logDebugInfo('entered paintContext onPress')

		#initialise the intersection list that will contain all the created objects within the same stroke
		self.strokeIntersectionList = intersectionList()

		#create the temporary group used through the stroke to store geometry as they are created
		self.tempgroup = mc.group (empty=True, name=spPaint3dTempGroupID)

		pressPosition = mc.draggerContext(spPaint3dContextID, query=True, anchorPoint=True);

		worldPos, worldDir = getViewportClick(pressPosition[0],pressPosition[1])

		intersected = targetSurfaceLoopIntersect(self.targetList, worldPos, worldDir)

		if(intersected):
			#there was a usable intersection found
			
			#first converting the returned internal coordinates into the current unit system
			intersected.convertUnit(self.unit)
			
			intersected.isValid(True)
			if(sp3d_dbg): logDebugInfo('found intersected')
			if(sp3d_log): print ('intersection at X: %f | Y: %f | Z: %f' % (intersected.hitPoint.x,intersected.hitPoint.y,intersected.hitPoint.z))
			if (not self.uiValues.paintFlux):
				#paintFlux set on timer
				intersected.startTimer()
			#feed various data into the intersectionPoint object
			if (self.uiValues.random): intersected.dagMeshSourceObject = self.sourceList.getRandom()
			else: intersected.dagMeshSourceObject = self.sourceList.getNext()
			if(sp3d_dbg): logDebugInfo('got the dag for the source object to use')

			if(sp3d_dbg): logDebugInfo('creating object from the dag')
			intersected.createdObjectDAG(self.createObject(intersected))
			if(sp3d_dbg): logDebugInfo('finished creating object from the dag, appending to intersection list')
			self.strokeIntersectionList.addPoint(intersected);

		if(sp3d_dbg): logDebugInfo('finished paintContext onPress')


	def onDrag(self):
		'''
		on mouse drag event
		'''
		#testing something
		if(self.reentrance==0): self.reentrance=1
		else: return;
		
		if(sp3d_dbg): logDebugInfo('entered paintContext onDrag')

		dragPosition = mc.draggerContext(spPaint3dContextID, query=True, dragPoint=True);

		worldPos, worldDir = getViewportClick(dragPosition[0],dragPosition[1])

		intersected = targetSurfaceLoopIntersect(self.targetList, worldPos, worldDir)
  		if(intersected):
			#there was a usable intersection found, checking if it's coherent with the paintFlux settings

			#first converting the returned internal coordinates into the current unit system
			intersected.convertUnit(self.unit)
			
			if(len(self.strokeIntersectionList.intersectionList) == 0):
				#there was no intersection during onPress
				if(sp3d_log): print ('intersection at X: %f | Y: %f | Z: %f' % (intersected.hitPoint.x,intersected.hitPoint.y,intersected.hitPoint.z))
				if (not self.uiValues.paintFlux):
					#paintFlux set on timer
					intersected.startTimer()

			else:
				#there was at least a previous intersection to test stuff
				if (self.uiValues.paintFlux):
					#paintFlux set on distance
					distanceToPrevious = getDistanceBetween(self.strokeIntersectionList.intersectionList[-1].hitPoint, intersected.hitPoint)
					correctedPaintDistance = self.uiValues.paintDistance
					#correctedPaintDistance = getCorrectedDistance(self.uiValues.paintDistance, self.unit)
					if(sp3d_log): print ('intersection at X: %f | Y: %f | Z: %f |||| distance from previous: %f (x: %f | y: %f | z: %f)(threshold: %f)(length of list: %i)' % (intersected.hitPoint.x,intersected.hitPoint.y,intersected.hitPoint.z, getDistanceBetween(self.strokeIntersectionList.intersectionList[-1].hitPoint, intersected.hitPoint),self.strokeIntersectionList.intersectionList[-1].hitPoint.x,self.strokeIntersectionList.intersectionList[-1].hitPoint.y,self.strokeIntersectionList.intersectionList[-1].hitPoint.z,correctedPaintDistance,len(self.strokeIntersectionList.intersectionList)))
					if (distanceToPrevious < correctedPaintDistance):
						#distance with the new intersection is below threshold, forcing an end of proc
						if sp3d_log: print ("breaking from onDrag since < threshold")
						intersected.isValid(False)
					else:
						#distance to previous >= threshold
						intersected.isValid(True)
				else:
					#paintFlux set on timer
					if (mc.timerX(startTime=self.strokeIntersectionList.intersectionList[-1].timestamp) < self.uiValues.paintTimer):
						#timer with the new intersection is below threshold, forcing an end of proc
						intersected.isValid(False)
					else:
						#this intersection is valid, starting its timer
						intersected.isValid(True)
						intersected.startTimer()


			if intersected.isValid():
				#should only reach here when intersected is above thresholds
	
				if sp3d_log: print ("valid intersection, creating object")
				#feed various data into the intersectionPoint object
				if (self.uiValues.random): intersected.updateDAGSourceObject(self.sourceList.getRandom())
				else: intersected.updateDAGSourceObject(self.sourceList.getNext())
	
				intersected.createdObjectDAG(self.createObject(intersected))
				self.strokeIntersectionList.addPoint(intersected);
		if(sp3d_dbg): logDebugInfo('finished paintContext onDrag')
		self.reentrance=0


	def onRelease(self):
		'''
		on mouse release event: CLEANUP
		'''
		if(self.uiValues.hierarchy):
			#grouping objects
			if (self.uiValues.group==0.0):
				#single group sorting
				groupName = self.uiValues.getGroupID()
				for obj in self.strokeIntersectionList.intersectionList:
					#print ("object created: %s (using source: %s) || will be sorted here: %s" % (obj.generatedDAG, obj.dagMeshSourceObject, groupName))
					if (not mc.objExists(groupName)): groupName = mc.group(empty=True, name=groupName);
					mc.parent(obj.generatedDAG, groupName, relative=True)

			elif (self.uiValues.group==1.0):
				#stroke group sorting
				groupName = mc.group(empty=True, name='spPaint3dStrokeOutput')
				for obj in self.strokeIntersectionList.intersectionList:
					#print ("object created: %s (using source: %s) || will be sorted here: %s" % (obj.generatedDAG, obj.dagMeshSourceObject, groupName))
					mc.parent(obj.generatedDAG, groupName, relative=True)

			elif (self.uiValues.group==2.0):
				#source group sorting
				for obj in self.strokeIntersectionList.intersectionList:
					#print ("object created: %s (using source: %s) || will be sorted here: %s" % (obj.generatedDAG, obj.dagMeshSourceObject, groupName))
					shapeParent = mc.listRelatives(obj.dagMeshSourceObject, parent=True)
					groupName = 'spPaint3dOutput_' + shapeParent[0]
					if(not mc.objExists(groupName)):
						#group doesnt exists, creating
						groupName = mc.group(name=groupName, empty=True)

					mc.parent(obj.generatedDAG, groupName, relative=True)

		#last cleanup, removing the temp group
		if mc.objExists(self.tempgroup):
			if(sp3d_log): print "tempGroup exists, attempting to remove if empty"
			if (not mc.listRelatives(self.tempgroup, children=True)):
				#tempGroup is empty, deleting
				if(sp3d_log): print ("tempGroup (%s) is empty, removing." % self.tempgroup)
				mc.delete(self.tempgroup)


	def createObject(self,intersection):
		'''
		will create the object at the intersection object gathered data, pending all ui and transform options
		will update the stored data to store the created object DAG path and return the newly created object DAG Path back
		'''

		newObjectDAG = None
		if (self.uiValues.instance):
			if(sp3d_dbg): logDebugInfo('creating instance')
			#fetching the transform for that shape (instance dont create object of child objects if it's the shape that gets instanced)
			tempDAG = mc.listRelatives(intersection.dagMeshSourceObject, parent=True)
			newObjectDAG = mc.instance(tempDAG[0])
		else:
			#fetching the parent transform to prevent some issue while duplicating with preserve input connections and stuff docked onto the transform and not the shape
			if(sp3d_dbg): logDebugInfo('duplicating object')
			tempDAG = mc.listRelatives(intersection.dagMeshSourceObject, parent=True)
			newObjectDAG = mc.duplicate(tempDAG[0], ic=self.uiValues.preserveConn)
		if(sp3d_dbg): logDebugInfo('DONE creating instance / duplicating object')

		if (len(newObjectDAG)>1):
			print ('warning: multiple objects created')

		moveTo(newObjectDAG[0], intersection.hitPoint)


		if (self.uiValues.align):
			#align to surface normal
			if(sp3d_dbg): logDebugInfo('aligning object with surface normal')
			rx, ry, rz = getEulerRotationQuaternion(self.worldUp, intersection.getHitNormal(self.uiValues.smoothNormal))
			mc.xform(newObjectDAG[0], ro=(rx, ry, rz) )
			if(sp3d_dbg): logDebugInfo('DONE aligning object with surface normal')

		if (self.uiValues.transformRotate):
			#rotate transform
			randrotate = self.transform.getRandomRotate()
			mc.rotate(randrotate[0], randrotate[1], randrotate[2], newObjectDAG[0], os=True, r=True, rotateXYZ=True)

		if (self.uiValues.transformScale):
			#scale transform
			randscale = self.transform.getRandomScale(self.uiValues.transformScaleUniform)
			mc.scale(randscale[0],randscale[1],randscale[2],newObjectDAG[0], relative=True)

		forceRefresh()
		if(self.uiValues.hierarchy):
			#grouping objects
			grouped = None
			if mc.objExists(self.tempgroup):
				grouped = mc.parent(newObjectDAG[0], self.tempgroup, relative=True)
			else:
				print 'error: temp group does not exist'
				print grouped
			return grouped[0];
		else: return newObjectDAG[0]


	def runtimeUpdate(self, uioptions, transformoptions, sourcelist, targetlist):
		'''
		entry method used from GUI to pass changes in the UI options through self.ctx.runtimeUpdate(...)
		'''
		self.uiValues = uioptions
		self.transform = transformoptions
		self.sourceList = sourcelist
		self.targetList = targetlist;



class modifierManager (object):
	'''
	Wrapper to manage the modifier keypress / release used in the place context
	'''
	modifierMask = { 	'shift' : 1,
						'ctrl' : 4,
						'alt' : 8}

	def __init__(self):
		'''
		setup various variables used
		'''
		self.ctrlReleased = True
		self.shiftReleased = True
		self.altReleased = True


	def resetCtrl(self):
		'''
		reset ctrl back to default settings, usually after the key press was acted upon
		'''
		pass


	def getState(self):
		'''
		return 3 booleans for ctrl / shift / alt state keypress event
		'''
		ctrl = shift = alt = False
		modifiers = mc.getModifiers()

		# SHIFT EVENT
		if (modifiers & 1) > 0 :
			if (self.shiftReleased):
				# shift was released in the previous iteration, this is a keypress
				self.shiftReleased = False
				shift = True
		else:
			# shift is not currently pressed, reseting the tracking self variable to released state
			self.shiftReleased = True

		# CTRL EVENT
		if (modifiers & 4) > 0 :
			if (self.ctrlReleased):
				# ctrl was released in the previous iteration, this is a keypress
				self.ctrlReleased = False
				ctrl = True
		else:
			# ctrl is not currently pressed, reseting the tracking self variable to released state
			self.ctrlReleased = True

		# ALT EVENT
		if (modifiers & 8) > 0 :
			if (self.altReleased):
				# alt was released in the previous iteration, this is a keypress
				self.altReleased = False
				alt = True
		else:
			# alt is not currently pressed, reseting the tracking self variable to released state
			self.altReleased = True

		return ctrl, shift, alt

	def isPressed(self, modifier):
		'''
		returns True if the modifier is currently pressed
		'''
		bitmask = mc.getModifiers() & self.modifierMask[modifier]
		if bitmask > 0 : return True
		else: return False;




class placeCursor (object):
	'''
	define a cursor object for use with placeContext
	'''
	def __init__(self, sourcedag=None, cursordag=None):
		'''
		initialize variables
		'''
		self.position=None
		self.cursorAlign=None
		if(sourcedag and cursordag):
			self.setCursorDAG(sourcedag, cursordag)

	def setCursorDAG(self, sourcedag, cursordag, deleteprevious=False):
		'''
		update the cursor dag
		'''
		self.sourceDAG = sourcedag
		self.sourceDAGPos = getPosition(sourcedag)
		if (deleteprevious):
			if(mc.objExists(self.cursorDAG)):
				#deleting the previous cursor object and its parent group if it's empty
				parentgroup = mc.listRelatives(self.cursorDAG, parent=True)
				mc.delete(self.cursorDAG)
				if (not mc.listRelatives(parentgroup)): mc.delete(parentgroup)
		self.cursorDAG = cursordag

	def setCursorTransform(self, rotate, scale):
		'''
		store transform tuples for rotate and scale of the cursor
		'''
		self.rotate = rotate
		self.scale = scale

	def move(self, position=None):
		'''
		move the cursor object to position of point type
		'''
		if (not position):
			#reseting current position to previously stored position if any
			if (self.position): moveTo(self.cursorDAG, self.position)
		else:
			self.position = position
			moveTo(self.cursorDAG, position)

	def align(self, rx=None, ry=None, rz=None):
		'''
		orient the cursor along the supplied rotation angles
		'''
		if ((rx!=None) and (ry!=None) and (rz!=None)):
			self.cursorAlign = rx, ry, rz
			mc.xform(self.cursorDAG, ro=(rx, ry, rz) )
		else:
			#realigning cursor with previously stored rotate tuple if it exists
			if(self.cursorAlign):
				rx, ry, rz = self.cursorAlign
				mc.xform(self.cursorDAG, ro=(rx, ry, rz) )


	def transform(self, rotate=False, scale=False):
		'''
		transform the cursor with self transform scale and rotate
		'''
		if (rotate):
			mc.rotate(self.rotate[0], self.rotate[1], self.rotate[2], self.cursorDAG, os=True, r=True, rotateXYZ=True)
		if (scale):
			mc.scale(self.scale[0], self.scale[1], self.scale[2], self.cursorDAG, relative=True)

	def asTemplate(self, mode=True):
		'''
		set cursor object in or out of template mode
		'''
		#TODO: Check if cursorDAG is a transform
		if (mode):
			#set to template
			mc.setAttr(self.cursorDAG+'.overrideEnabled', 1)
			mc.setAttr(self.cursorDAG+'.overrideDisplayType', 1)
		else:
			#set to normal
			mc.setAttr(self.cursorDAG+'.overrideEnabled', 0)
			mc.setAttr(self.cursorDAG+'.overrideDisplayType', 0)


class placeContext (object):
	'''
	define placeContext
	'''
	def __init__(self, uioptions, transformoptions, sourcelist, targetlist):
		'''
		initial setup
		'''
		#create the tool context
		if (mc.draggerContext(spPaint3dContextID, exists=True)):
			mc.deleteUI(spPaint3dContextID);
		mc.draggerContext(spPaint3dContextID, pressCommand=self.onPress, prePressCommand=self.onBeforePress, dragCommand=self.onDrag, holdCommand=self.onHold, releaseCommand=self.onRelease, name=spPaint3dContextID, cursor='crossHair', undoMode='step')

		#create context local options
		self.runtimeUpdate(uioptions, transformoptions, sourcelist, targetlist)

		#initialise world up vector
		if ( (mc.upAxis(q=True, axis=True)) == "y" ):
			self.worldUp = om.MVector (0,1,0);
		elif ( (mc.upAxis(q=True, axis=True)) == "z" ):
			self.worldUp = om.MVector (0,0,1);

		#fetch current scene unit
		self.unit = mc.currentUnit(query=True, linear=True)

		self.reentrance=0
		self.mState = modifierManager()

	def runContext(self):
		'''
		set maya tool to the context
		'''
		if (mc.draggerContext(spPaint3dContextID, exists=True)): mc.setToolTo(spPaint3dContextID);

	def fetchCursorObject(self):
		'''
		gather necessary data and generate a new cursor Object and return the string to the source DAG and the cursor DAG
		'''
		#fetch a new cursorObject
		if (self.uiValues.random): sourceDAG = self.sourceList.getRandom()
		else: sourceDAG = self.sourceList.getNext()

		#checking if the sourceDAG is a transform, if not attempting to find the parent transform
		#instancing multiple object doesnt work if using a shape
		#duplicating input connections on the parent transform is not carried over if using the shape
		if(mc.nodeType(sourceDAG)!='transform'):
			tempDAG = mc.listRelatives(sourceDAG, parent=True)
			sourceDAG = tempDAG[0]

		newObjectDAG = None
		if (self.uiValues.instance):
				newObjectDAG = mc.instance(sourceDAG)
		else:
				newObjectDAG = mc.duplicate(sourceDAG, ic=self.uiValues.preserveConn)

		if (len(newObjectDAG)>1):
			print ('warning: multiple objects created')

		#create the temporary group used through the stroke to store geometry as they are created
		self.tempgroup = mc.group (empty=True, name=spPaint3dTempGroupID)
		newObjectDAG = mc.parent(newObjectDAG[0], self.tempgroup, relative=True)

		return sourceDAG, newObjectDAG[0]

	def fetchCursorTransform(self):
		'''
		compute transform data for cursor object and return the tuple for rotate and scale
		'''
		# getting the proper transform tuples
		if (self.uiValues.transformRotate):
			cursorRotate = self.transform.getRandomRotate()
		else:
			#transform rotate off
			cursorRotate = (0,0,0)

		if (self.uiValues.transformScale):
			cursorScale = self.transform.getRandomScale(self.uiValues.transformScaleUniform)
		else:
			#transform scale off
			cursorScale = (1,1,1)

		return cursorRotate, cursorScale

	def ctrlEvent(self):
		'''
		ctrl event stuff
		'''
		#fetching a new cursor
		newSourceDAG, newCursorDAG = self.fetchCursorObject()
		self.cursor.setCursorDAG(newSourceDAG,newCursorDAG,True) #flagging to delete previous cursor
		self.cursor.move() #defaulting without parameters to position the new cursor to the previously stored position
		if(self.uiValues.align):
			self.cursor.align()
		self.cursor.transform(self.uiValues.transformRotate, self.uiValues.transformScale)
		
	def shiftEvent(self):
		'''
		shift event stuff (rotate on upvector)
		'''
		pass


	def onBeforePress(self):
		'''
		prePress event to setup the temp data for the cursor object
		'''
		sourceDAG, cursorDAG = self.fetchCursorObject()
		self.cursor = placeCursor(sourceDAG, cursorDAG)

		cursorRotate, cursorScale = self.fetchCursorTransform()
		self.cursor.setCursorTransform(cursorRotate, cursorScale)
		self.cursor.transform(self.uiValues.transformRotate, self.uiValues.transformScale)
		#self.cursor.asTemplate() #issues with place and object hierarchies, breaking the method for some reason

	def onPress(self):
		'''
		on mouse press initial event
		'''
		pressPosition = mc.draggerContext(spPaint3dContextID, query=True, anchorPoint=True);

		ctrl, shift, alt = self.mState.getState()

		worldPos, worldDir = getViewportClick(pressPosition[0],pressPosition[1])

		intersected = targetSurfaceLoopIntersect(self.targetList, worldPos, worldDir)
  		if(intersected):
			#there was a usable intersection found
			#first checking and converting units if necessary
			intersected.convertUnit(self.unit)
			#now moving the cursor
			self.cursor.move(intersected.hitPoint)
			if(self.uiValues.align):
				rx, ry, rz = getEulerRotationQuaternion(self.worldUp, intersected.getHitNormal(self.uiValues.smoothNormal))
				self.cursor.align(rx,ry,rz)

			#retransforming the cursor rotation if necessary
			self.cursor.transform(self.uiValues.transformRotate)

		else:
			pass
			#no intersection found
			#TODO
			#moving cursor to worldPos and aligning to worldDir

		#forcing a viewport redraw
		forceRefresh()


	def onDrag(self):
		'''
		on mouse drag event
		'''
		if self.reentrance==1: return
		self.reentrance=1
		
		dragPosition = mc.draggerContext(spPaint3dContextID, query=True, dragPoint=True);

		worldPos, worldDir = getViewportClick(dragPosition[0],dragPosition[1])

		ctrl, shift, alt = self.mState.getState()
		#
		#TODO scale and rotate depending on mouse drag direction
		#
		intersected = targetSurfaceLoopIntersect(self.targetList, worldPos, worldDir)
  		if(intersected):
			#there was a usable intersection found
			#first checking and converting units if necessary
			intersected.convertUnit(self.unit)
			#now moving the cursor
			self.cursor.move(intersected.hitPoint)
			if(self.uiValues.align):
				rx, ry, rz = getEulerRotationQuaternion(self.worldUp, intersected.getHitNormal(self.uiValues.smoothNormal))
				self.cursor.align(rx,ry,rz)

			#retransforming the cursor rotation if necessary
			self.cursor.transform(self.uiValues.transformRotate)

			#forcing a viewport redraw
			forceRefresh()

		else:
			pass
			#no intersection found
			#TODO
			#moving cursor to worldPos and aligning to worldDir

		if(ctrl):
			self.ctrlEvent()
			forceRefresh()
		if(shift):
			self.shiftEvent()
			forceRefresh()
			
		self.reentrance=0

	def onHold(self):
		'''
		on mouse hold event
		'''
		if self.reentrance==1: return
		self.reentrance=1
		
		dragPosition = mc.draggerContext(spPaint3dContextID, query=True, dragPoint=True);

		ctrl, shift, alt = self.mState.getState()

		if(ctrl):
			self.ctrlEvent()
			forceRefresh()
		if(shift):
			self.shiftEvent()
			forceRefresh()

		if sp3d_log:
			message = 'key press detected: '
			if (ctrl): message += 'ctrl pressed... '
			if (shift): message += 'shift pressed... '
			if (alt): message += 'alt pressed... '

		self.reentrance=0


	def onRelease(self):
		'''
		on mouse release event: CLEANUP
		'''
		#reverting template
		#self.cursor.asTemplate(False) #issues with object hierarchies for some reason

		if self.uiValues.hierarchy:
			#grouping object
			#parenting the cursor object in the appropriate groupe
			if (self.uiValues.group==0.0):
				#single group sorting
				groupName = self.uiValues.getGroupID()
				if (not mc.objExists(groupName)): groupName = mc.group(empty=True, name=groupName);
				mc.parent(self.cursor.cursorDAG, groupName, relative=True)

			elif (self.uiValues.group==1.0):
				#stroke group sorting
				groupName = mc.group(empty=True, name='spPaint3dStrokeOutput')
				mc.parent(self.cursor.cursorDAG, groupName, relative=True)

			elif (self.uiValues.group==2.0):
				#source group sorting
				shapeParent = mc.listRelatives(self.cursor.sourceDAG, parent=True)
				groupName = 'spPaint3dOutput_' + shapeParent[0]
				if(not mc.objExists(groupName)):
					#group doesnt exists, creating
					groupName = mc.group(name=groupName, empty=True)

				mc.parent(self.cursor.cursorDAG, groupName, relative=True)

		#last cleanup, removing the temp group
		if(mc.objExists(self.tempgroup)):
			if (not mc.listRelatives(self.tempgroup, children=True)):
				#tempGroup is empty, deleting
				mc.delete(self.tempgroup)


	def runtimeUpdate(self, uioptions, transformoptions, sourcelist, targetlist):
		'''
		entry method used from GUI to pass changes in the UI options through self.ctx.runtimeUpdate(...)
		'''
		self.uiValues = uioptions
		self.transform = transformoptions
		self.sourceList = sourcelist
		self.targetList = targetlist;



#-------------------------------
# Misc Utils
#-------------------------------

def forceRefresh():
	'''
	force a current viewport refresh
	'''
	mc.refresh(cv=True);


def moveTo(dag, pos):
	'''
	move the dag object to pos position
	attemps to compensate for unfrozen transform by reading the scalepivot of the object
	'''
	scalePivot = mc.xform(dag, query=True, ws=True, sp=True)
	transform = mc.xform(dag, query=True, ws=True, t=True)

	mc.xform(dag, t=( (transform[0]-scalePivot[0])+pos.x, (transform[1]-scalePivot[1])+pos.y, (transform[2]-scalePivot[2])+pos.z ));


def getPosition(dag):
	'''
	retrieve the world position of the dag parameter object and return a point object containing the position
	'''
	tempdag = dag
	if (mc.nodeType(tempdag)!='transform'):
		#get the transform to that shape
		temprelatives = mc.listRelatives(tempdag, parent=True)
		tempdag=temprelatives[0]

	scalePivot = mc.xform(tempdag, query=True, ws=True, sp=True)
	transform = mc.xform(tempdag, query=True, ws=True, t=True)

	return point( (transform[0]-scalePivot[0]), (transform[1]-scalePivot[1]), (transform[2]-scalePivot[2]) )


def getEulerRotationQuaternion(upvector, directionvector):
	'''
	returns the x,y,z degree angle rotation corresponding to a direction vector
	input: upvector (MVector) & directionvector (MVector)
	'''
	quat = om.MQuaternion(upvector, directionvector)
	quatAsEuler = om.MEulerRotation()
	quatAsEuler = quat.asEulerRotation()

 	return math.degrees(quatAsEuler.x), math.degrees(quatAsEuler.y), math.degrees(quatAsEuler.z)


def getViewportClick(screenX, screenY):
	'''
	return world position and direction of the viewport clicked point (returns point objects)
	'''
	maya3DViewHandle = omui.M3dView()
	activeView = maya3DViewHandle.active3dView()

	clickPos = om.MPoint()
	clickDir = om.MVector()

	activeView.viewToWorld(int(screenX), int(screenY), clickPos, clickDir)

	worldPos = point(clickPos.x, clickPos.y, clickPos.z)
	worldDir = point(clickDir.x, clickDir.y, clickDir.z)

	return worldPos,worldDir



def getCameraFarClip():
	'''
	Return current camera far clip
	'''
	maya3DViewHandle = omui.M3dView()
	activeView = maya3DViewHandle.active3dView()

	cameraDP = om.MDagPath()
	maya3DViewHandle.active3dView().getCamera(cameraDP)

	camFn = om.MFnCamera(cameraDP)
	return camFn.farClippingPlane();


def targetSurfaceLoopIntersect(targetList, clickPos, clickDir):
	'''
	loop through all the object in targetList and intersect them with click (world pos, direction). creates an intersectionPoint object for each intersection
	sort the list of intersection and return the closest intersectionPoint object from the click world position, return None if no intersection found
	'''
	ilist = intersectionList()
	farclip = getCameraFarClip()
	for obj,data in targetList.obj.iteritems():
		#loop through each object in the targetList
		intersected = intersectTargetSurface(data[0], clickPos, clickDir, farclip)
		if (intersected):
			#got meh an intersected
			ilist.addPoint(intersected)

	return ilist.getClosest(clickPos);


def intersectTargetSurface(targetdag, clickPos, clickDir, farclip=1.0):
	'''
	intersect a single object from the click world pos and direction. optional farclip distance
	return an intersectionPoint object if there was any intersection
	return None otherwise
	'''
	currentHitFP = om.MFloatPoint() #current intersection
	hitFace = om.MScriptUtil()
	hitTri = om.MScriptUtil()
	
	hitFace.createFromInt(0)
	hitTri.createFromInt(0)
	
	hitFaceptr = hitFace.asIntPtr()
	hitTriptr = hitTri.asIntPtr() 

	targetDAGPath = getDAGObject(targetdag)

	if (targetDAGPath):
		#returned targetDAGPath is sort of valid
		fnMesh = om.MFnMesh( targetDAGPath )
		hit = fnMesh.closestIntersection( clickPos.asMFPoint(),
								clickDir.asMFVector(),
								None,
								None,
								True,
								om.MSpace.kWorld,
								farclip,
								True,
								None,
								currentHitFP,
								None,
								hitFaceptr,
								hitTriptr,
								None,
								None)
		if (hit):
			#there was a positive intersection
			if (sp3d_MFn): print ("Face Hit: %i || Tri Hit: %i" % (hitFace.getInt(hitFaceptr),hitTri.getInt(hitTriptr)))
			return intersectionPoint(point(currentHitFP.x, currentHitFP.y, currentHitFP.z), hitFace.getInt(hitFaceptr), hitTri.getInt(hitTriptr), targetDAGPath);

	#reaches here only if no intersection or not a valid targetDAGPath
	return None




def getDAGObject(dagstring):
	'''
	return the DAG Api object from the dagstring argument
	return None if the minimum checks on dagstring don't checkout
	'''
	sList = om.MSelectionList()
	meshDP = om.MDagPath()
	#sList.clear() #making sure to clear the content of the MSelectionList in case we are looping through multiple objects
	om.MGlobal.getSelectionListByName(dagstring, sList)
	sList.getDagPath(0,meshDP)

	return meshDP;

def getDistanceBetween(source,target):
	'''
	return the distance between source and target
	'''
	distance = math.sqrt(	math.pow((source.x - target.x), 2) +
        						math.pow((source.y - target.y), 2) +
        						math.pow((source.z - target.z), 2) );
	return distance;

def getCorrectedDistance(distance, unit):
	'''
	return the corrected distance using proper unit to convert back to centimeters
	'''
	if(unit=='cm'): return distance
	else: return (distance/(sp3dUnit[unit]));

def logDebugInfo(info):
	'''
	overwrite the default debug file content with <info>
	'''
	with open(sp3d_dbgfile,'w') as f:
		f.write(info)
