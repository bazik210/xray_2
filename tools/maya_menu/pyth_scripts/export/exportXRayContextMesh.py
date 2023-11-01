#-----------------------------------------------------------------
#	SCRIPT			exportXRayContextMesh.py
#	AUTHOR			Zaitsev Evgeniy
#                   ev.zaitsev@gmail.com
#	DATE:			28 September 2010
#
#	DESCRIPTION:	Define tools for mesh exporter
#                   S.T.A.L.K.E.R. 2 project
#
#  VERSION:	        201102
#
#-----------------------------------------------------------------
import maya.OpenMaya as OpenMaya    # work with maya API
import maya.cmds as cmds            # work with maya commands
import maya.mel as mel              # work with mel
import ConfigParser                 # config parser
import string                       # string parser
import re                           # regular expression
import os                           # work with file system
import sys                          # python and maya environment
from exportXRayContext import *     # common functions
#-----------------------------------------------------------------------------------
#	MESH NODE CLASS
#-----------------------------------------------------------------------------------
class m_meshNode(object):
    """
    store a dictionary of objects, their DAG data in the script Node
    """
    def __init__(self):
        """
        initialize the default attributes
        """    
        self.errorHandle = m_meshTrace() # error tracer    

    def getNode(self):
        m_selectionList = OpenMaya.MSelectionList()
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kBreadthFirst )

        while not m_dagIterator.isDone():
            m_objCurr = m_dagIterator.currentItem()
            m_objPath = OpenMaya.MDagPath()
            m_dagIterator.getPath( m_objPath )

            if ( m_objCurr.hasFn( OpenMaya.MFn.kTransform ) ):
                m_nodeFn = OpenMaya.MFnDependencyNode()
                m_nodeFn.setObject( m_objCurr )
                if ( not m_nodeFn.isFromReferencedFile() ):
                    try:
                        m_nodeFn.attribute( '_201102_objects' )
                        m_nodeFn.attribute( '_201102_isSel'   )
                        m_nodeFn.attribute( '_201102_type'    )
                        m_nodeFn.attribute( '_201102_selTags' )
                        m_nodeFn.attribute( '_201102_saveDir' )
                        #print( "%s %s" %( m_objPath.fullPathName(), m_nodeFn.name() ) )
                        m_selectionList.add( m_objPath )
                    except:
                        pass
            m_dagIterator.next() # next item                
        return m_selectionList
        
    def addObj(self, m_obj, m_IsSelected = False, m_objType = m_objectType[0], m_objSelTags = [], m_saveDir = m_xrayResourcesPathMeshes):
        """
        add '%s._201102_objects' attribute to the m_obj node
        
        INPUT:  m_obj         - String, full path to the node, like: '|group6|group5' 
                m_IsSelected  - set to True if object in list selected
                m_objType     - set type of object based on global array m_objectType 
                m_objSelTags  - list of object tags based on gloabl m_categoryTree

        STORED: m_IsSelected  - False
                m_objType     - 'Static'
                m_objSelTags  - ['Location|Industrial',
                                 'Architectural|Building',
                                 'Object|Garbage']
                m_saveDir     - 'XRAY_RESOURCES_PATH/models'
        RETURN: True if object was added properly, False otherwise        
        """
        m_objPath = getMDagPathFromNodeName( m_obj )

        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False

        m_nodeMObj = m_objPath.node()

        if ( not m_nodeMObj.hasFn( OpenMaya.MFn.kTransform ) ):
            self.errorHandle.raiseMessage( "Select Transform node please" )
            return False
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute('_201102_objects')
            self.errorHandle.raiseMessage( "Attribute %s._201102_objects already exists" %m_nodeFn.name() )
            return False
        except:
            pass

        m_nodeName = m_objPath.fullPathName()

        try:
            cmds.addAttr( m_nodeName, longName = '_201102_objects',
                                         niceName = 'Objects',
                                         attributeType = 'compound', 
                                         numberOfChildren = 4 )
            cmds.addAttr( m_nodeName, parent = '_201102_objects', longName = '_201102_isSel',
                                                                     niceName = 'Is Selected',
                                                                     attributeType = 'bool')
            cmds.addAttr( m_nodeName, parent = '_201102_objects', longName = '_201102_type',
                                                                     niceName = 'Type',
                                                                     dataType = 'string')
            cmds.addAttr( m_nodeName, parent = '_201102_objects', longName = '_201102_selTags',
                                                                     niceName = 'Tags',
                                                                     dataType = 'stringArray')
            cmds.addAttr( m_nodeName, parent = '_201102_objects', longName = '_201102_saveDir',
                                                                     niceName = 'Save to',
                                                                     dataType = 'string')
        except:
            self.errorHandle.raiseMessage( "Can't add attribute '_201102_objects' to node %s" %m_nodeName )
            return False

        m_saveDir = getPartialXrayResourcesPath( 'models' )

        # ----------- unlcok --------------
        cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = False )
        cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = False )
        cmds.setAttr( '%s._201102_type'    %m_nodeName, lock = False )
        cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = False )
        cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = False )
        # ------------- set ---------------
        cmds.setAttr( '%s._201102_isSel'   %m_nodeName, bool(m_IsSelected) )
        cmds.setAttr( '%s._201102_type'    %m_nodeName, m_objType, type = 'string' )
        cmds.setAttr( '%s._201102_selTags' %m_nodeName, type = 'stringArray', *([len(m_objSelTags)] + m_objSelTags) )
        cmds.setAttr( '%s._201102_saveDir' %m_nodeName, m_saveDir, type = 'string' )
        # ------------ lock ---------------
        cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = True )
        cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = True )
        cmds.setAttr( '%s._201102_type'    %m_nodeName, lock = True )
        cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = True )
        cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = True )
        return True
        
    def delObj(self, m_obj):
        """
        delete '%s._201102_objects' attribute from the m_objPath node
        INPUT:  m_obj     - String, full path to the node, like: '|group6|group5'
        RETURN: True if attr was deleted from the node, False otherwise
        """
        m_objPath = getMDagPathFromNodeName( m_obj )
        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False        
        m_nodeMObj = m_objPath.node()
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute('_201102_objects')
            m_nodeName = m_objPath.fullPathName() 
            # ----------- unlcok --------------
            cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_type'    %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = False )
            # ----------- delete --------------            
            cmds.deleteAttr( '%s._201102_objects' %m_nodeName )
            return True
        except:
            self.errorHandle.raiseMessage( "Can't find %s._201102_objects attribute" %m_nodeName )
        return False     
    
    def count(self):
        """
        RETURN: count of nodes with '%s._201102_objects' attribute
        """
        m_selectionList = self.getNode()        
        return m_selectionList.length()

    def printObj(self):
        """
        read node and print the content of the dictionary, for debug purpose only
        """
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            m_listIt.getDagPath( m_objPath ) # get current Dag
            m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
            m_nodeName = m_objPath.fullPathName() # get full string path to node            

            try:
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_isSel' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_selTags' )
                m_nodeFn.attribute( '_201102_saveDir' ) 
                self.errorHandle.raiseMessage ( "KEY: %s <==> SEL: %s TYPE: %s TAG: %s DIR: %s" 
                         %( m_nodeName, 
                            cmds.getAttr( '%s._201102_isSel'     %m_nodeName ),
                            cmds.getAttr( '%s._201102_type'      %m_nodeName ),
                            cmds.getAttr( '%s._201102_selTags'   %m_nodeName ),
                            cmds.getAttr( '%s._201102_saveDir'   %m_nodeName ) )   )
            except:
                pass
            m_listIt.next()

    def renameVisCol(self):
        """
        read node and print the content of the dictionary, for debug purpose only
        """
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            m_listIt.getDagPath( m_objPath ) # get current Dag
            m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
            m_nodeName = m_objPath.fullPathName() # get full string path to node 
            m_nodeNameShort = getShortName( m_nodeName )
            m_lod0Name  =  "%s|%s_LOD0"  %( m_nodeName, m_nodeNameShort ) 
            m_clod0Name =  "%s|%s_CLOD0" %( m_nodeName, m_nodeNameShort ) 
            m_lod0MDagPath   = getMDagPathFromNodeName( m_lod0Name )  
            m_clod0MDagPath  = getMDagPathFromNodeName( m_clod0Name )

            if ( m_lod0MDagPath ):            
                m_visNode = cmds.group( empty = True, world = True )
                m_visNodeMDagPath = getMDagPathFromNodeName( m_visNode )
                try:
                    cmds.parent( m_visNodeMDagPath.fullPathName(), m_nodeName )
                except:
                    self.errorHandle.raiseMessage( "Can't parent %s to %s" %( m_visNodeMDagPath.fullPathName(), m_nodeName ) )
                    return False
                try:    
                    cmds.rename( m_visNodeMDagPath.fullPathName(), 'vis' )
                except:
                    self.errorHandle.raiseMessage( "Can't rename %s to vis" %m_visNodeMDagPath.fullPathName() ) 
                    return False
                try:       
                    cmds.parent( m_lod0MDagPath.fullPathName(), m_visNodeMDagPath.fullPathName() )
                except:
                    self.errorHandle.raiseMessage( "Can't parent %s to %s" %( m_lod0MDagPath.fullPathName(), m_visNodeMDagPath.fullPathName() ) )
                    return False

            if ( m_clod0MDagPath ):
                m_colNode = cmds.group( empty = True, world = True )
                m_colNodeMDagPath = getMDagPathFromNodeName( m_colNode )

                try:
                    cmds.parent( m_colNodeMDagPath.fullPathName(), m_nodeName )
                except:
                    self.errorHandle.raiseMessage( "Can't parent %s to %s" %( m_colNodeMDagPath.fullPathName(), m_nodeName ) )
                    return False

                try:    
                    cmds.rename( m_colNodeMDagPath.fullPathName(), 'col' )
                except:
                    self.errorHandle.raiseMessage( "Can't rename %s to col" %m_colNodeMDagPath.fullPathName() ) 
                    return False

                try:       
                    cmds.parent( m_clod0MDagPath.fullPathName(), m_colNodeMDagPath.fullPathName() )
                except:
                    self.errorHandle.raiseMessage( "Can't parent %s to %s" %( m_clod0MDagPath.fullPathName(), m_colNodeMDagPath.fullPathName() ) )
                    return False                                    

            m_listIt.next() 
        cmds.select( clear = True ) 
        return True                                

    def lockAllItems(self):
        """
        lock all items in the node
        """
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                            
            m_listIt.getDagPath( m_objPath ) # get current Dag
            m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
            m_nodeName = m_objPath.fullPathName() # get full string path to node            
            try:
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_isSel' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_selTags' )
                m_nodeFn.attribute( '_201102_saveDir' ) 
                # ------------ lock ---------------
                cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = True )
                cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = True )
                cmds.setAttr( '%s._201102_type'    %m_nodeName, lock = True )
                cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = True )
                cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = True )
            except:
                pass
            m_listIt.next()  
    
    def unlockAllItems(self):
        """
        unlock all items in the node
        """
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                            
            m_listIt.getDagPath( m_objPath ) # get current Dag
            m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
            m_nodeName = m_objPath.fullPathName() # get full string path to node            

            try:
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_isSel' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_selTags' )
                m_nodeFn.attribute( '_201102_saveDir' ) 
                # ----------- unlcok --------------
                cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = False )
                cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = False )
                cmds.setAttr( '%s._201102_type'    %m_nodeName, lock = False )
                cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = False )
                cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = False )
            except:
                pass
            m_listIt.next()

    def setObjTag(self, m_obj, m_objSelTags):
        """
        set all object tags from m_tag_list to existed object in the node
        INPUT:  m_obj - key value from list (example: '|group4|group3')
                m_tag - list of strings formated like this: ['Location|Industrial',
                                                             'Architectural|Building',
                                                             'Object|Garbage']
        RETURN: True if list of tags was added, False otherwise 
        """        
        m_objPath = getMDagPathFromNodeName( m_obj )
        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False        
        m_nodeMObj = m_objPath.node()
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute('_201102_objects')
        except:
            self.errorHandle.raiseMessage( "Can't find %s._201102_objects attribute" %m_nodeName )
            return False

        try:            
            m_nodeName = m_objPath.fullPathName() 
            # ----------- unlock --------------
            cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = False )
            # ----------- delete -------------- 
            cmds.setAttr( '%s._201102_selTags' %m_nodeName, type = 'stringArray', *( [len(m_objSelTags)] + m_objSelTags ) )
            # ------------- lock --------------
            cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = True )
            cmds.setAttr( '%s._201102_selTags' %m_nodeName, lock = True )
            return True
        except:
            self.errorHandle.raiseMessage( "Can't setAttr %s._201102_selTags" %m_nodeName )
        return False         

    def setObjType(self, m_obj, m_objType):
        """
        set type of object based on global array m_objectType ['Static', 'Dynamic']
        INPUT:  m_obj     - key value (example: '|group4|group3')
                m_objType - string value, type of object based on global array m_objectType 
        RETURN: True if object type was seted, False otherwise
        USAGE:  my_node.setObjType('|group4','Dynamic')
        """
        if (0 == m_objectType.count(m_objType)):
            # entered type don't specified in global array m_objectType
            self.errorHandle.raiseMessage("m_meshNode::setObjType Entered type %s don't specified in global array m_objectType" %m_objType)
            return False
        m_objPath = getMDagPathFromNodeName( m_obj )

        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False        

        m_nodeMObj = m_objPath.node()
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute('_201102_objects')
        except:
            self.errorHandle.raiseMessage( "Can't find %s._201102_objects attribute" %m_nodeName )
            return False            
        try:            
            m_nodeName = m_objPath.fullPathName() 
            # ----------- unlock --------------
            cmds.setAttr( '%s._201102_objects'              %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_type' %m_nodeName, lock = False )
            # ----------- delete -------------- 
            cmds.setAttr( '%s._201102_type' %m_nodeName,  m_objType, type = 'string' )
            # ------------- lock --------------
            cmds.setAttr( '%s._201102_objects'              %m_nodeName, lock = True )
            cmds.setAttr( '%s._201102_type' %m_nodeName, lock = True )
            return True
        except:
            self.errorHandle.raiseMessage( "Can't setAttr %s._201102_type" %m_nodeName )
        return False    

    def setSaveDir(self, m_obj, m_saveDir):
        """
        set save directory
        INPUT:  m_obj     - key value (example: '|group4|group3')
                m_saveDir - string value, path to folder for save current obj
        RETURN: True if saveDir was seted, False otherwise
        USAGE:  my_node.setSaveDir('|group4','XRAY_RESOURCES_PATH/models/vehicles')
        """
        m_objPath = getMDagPathFromNodeName( m_obj )

        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False        

        m_nodeMObj = m_objPath.node()
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute('_201102_objects')
        except:
            self.errorHandle.raiseMessage( "Can't find %s._201102_objects attribute" %m_nodeName )
            return False

        try:            
            m_nodeName = m_objPath.fullPathName() 
            # ----------- unlock --------------
            cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = False )
            # ----------- delete -------------- 
            cmds.setAttr( '%s._201102_saveDir' %m_nodeName, m_saveDir, type = 'string' )
            # ------------- lock --------------
            cmds.setAttr( '%s._201102_objects'                 %m_nodeName, lock = True )
            cmds.setAttr( '%s._201102_saveDir' %m_nodeName, lock = True )
            return True
        except:
            self.errorHandle.raiseMessage( "Can't setAttr %s._201102_saveDir" %m_nodeName )
        return False    

    def setSelectionState(self, m_obj, m_IsSelected = False):       
        """
        set to True if object in list selected
        INPUT:  m_obj        - key value from list self.obj (example: '|group4|group3')
                m_IsSelected - boolean value, True - selected, False - unselected 
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setSelectionState('|group4', False)
        """
        m_objPath = getMDagPathFromNodeName( m_obj )

        if ( not m_objPath ):
            self.errorHandle.raiseMessage( "Can't find object %s" %m_obj )
            return False

        m_nodeMObj = m_objPath.node()
        m_nodeFn   = OpenMaya.MFnDependencyNode()

        try:
            m_nodeFn.setObject( m_nodeMObj )
        except:
            self.errorHandle.raiseMessage( "Can't set MObject() to MFnDependencyNode()" )
            return False

        try:
            m_nodeFn.attribute( '_201102_objects' )
        except:
            self.errorHandle.raiseMessage( "Can't find %s._201102_objects attribute" %m_nodeName )
            return False

        try:            
            m_nodeName = m_objPath.fullPathName() 
            # ----------- unlock --------------
            cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = False )
            cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = False )
            # ----------- delete -------------- 
            cmds.setAttr( '%s._201102_isSel' %m_nodeName, bool( m_IsSelected ) )
            # ------------- lock --------------
            cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = True )
            cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = True )
            return True
        except:
            self.errorHandle.raiseMessage( "Can't setAttr %s._201102_isSel" %m_nodeName )
        return False
        
    def setAllSelectionStates(self, m_IsSelected = False):
        """
        set all selection states to m_IsSelected
        """
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                            
            m_listIt.getDagPath( m_objPath ) # get current Dag
            m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
            m_nodeName = m_objPath.fullPathName() # get full string path to node

            try:
                m_nodeFn.aAttribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_isSel' )
                # ----------- unlock --------------
                cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = False )
                cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = False )
                # ------------- set ---------------
                cmds.setAttr( '%s._201102_isSel'   %m_nodeName, bool( m_IsSelected ) )
                # ------------ lock ---------------
                cmds.setAttr( '%s._201102_objects' %m_nodeName, lock = True )
                cmds.setAttr( '%s._201102_isSel'   %m_nodeName, lock = True )
            except:
                pass
            m_listIt.next()
        
    def iterItems(self):
        """
        you can iterate in program using this method
        RETURN: hash all data stored in the node
        """
        # ------------------------------------------------------------
        if ( 0 == OpenMaya.MGlobal_mayaState() ): # MAIN PROGRESS BAR
            gMainProgressBar = mel.eval( '$tmp = $gMainProgressBar' )
            cmds.progressBar(   gMainProgressBar,
                                edit = True,
                                beginProgress = False,
                                isInterruptable = False,
                                maxValue=100 )
            cmds.progressBar( gMainProgressBar, edit = True, beginProgress = True ) # BEGIN PROGRESS
        # ------------------------------------------------------------
        m_returnHash = {}
        m_maxLenght = self.getMaxLenList() # [maxLen(getShortName(m_obj)), maxLen(m_obj), maxLen(m_objType), maxLen(m_saveDir)] 
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )           

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                  

            try:
                m_listIt.getDagPath( m_objPath ) # get current Dag
                m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_isSel' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_selTags' )
                m_nodeFn.attribute( '_201102_saveDir' ) 
                # -----------------------------
                m_nodeName        = m_objPath.fullPathName() # get full string path to node
                m_nodeNamePartial = getShortName( m_nodeName )
                m_IsSelected      = cmds.getAttr( '%s._201102_isSel'     %m_nodeName )
                m_objType         = cmds.getAttr( '%s._201102_type'      %m_nodeName )
                m_objSelTags      = cmds.getAttr( '%s._201102_selTags'   %m_nodeName )
                m_saveDir         = cmds.getAttr( '%s._201102_saveDir'   %m_nodeName )
                # m_xrayResourcesPathMeshes = 'XRAY_RESOURCES_PATH/models'
                m_saveDir = m_saveDir.replace(m_xrayResourcesPathMeshes, '' , 1)                 
                # -----------------------------
                m_list = []
                m_list.append( addSpacesToString( m_nodeNamePartial,   m_maxLenght[0] - len( m_nodeNamePartial ) ) )    # box_01
                m_list.append( addSpacesToString( m_nodeName,          m_maxLenght[1] - len( m_nodeName ) ) )           # |props|box|box_01
                m_list.append( addSpacesToString( m_objType,           m_maxLenght[2] - len( m_objType ) ) )            # Static 
                m_list.append( addSpacesToString( m_saveDir,           m_maxLenght[3] - len( m_saveDir ) ) )            # props                                 
                # 'box_01    |props|box|box_01    Static    props'
                m_returnHash[ m_nodeName ] = ( bool( m_IsSelected ), m_objType, m_objSelTags, m_saveDir, string.join( m_list,'    ' ) )
            except:
                self.errorHandle.raiseMessage( "m_meshNode.iterItems Error" )
                pass

            # ------------------------------------------------------------
            if ( 0 == OpenMaya.MGlobal_mayaState() ):     # INCREASE PROGRESS STEP
                cmds.progressBar( gMainProgressBar, edit=True, step=100/m_selectionList.length() )
            m_listIt.next()
        # ------------------------------------------------------------ 
        if ( 0 == OpenMaya.MGlobal_mayaState() ):
            cmds.progressBar( gMainProgressBar, edit = True, endProgress = True ) # END PROGRESS
        return m_returnHash

    def getObjLabelFromNode(self, m_objInput):
        """
        INPUT:  m_obj - key value (example: '|props|box|box_01')
        RETURN: string like 'box_01    |props|box|box_01    Static    props'
        USAGE:  my_node.getObjLabelFromNode('|props|box|box_01')        
        """
        m_maxLenght = self.getMaxLenList() 
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )   

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                  

            try:
                m_listIt.getDagPath( m_objPath ) # get current Dag
                m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj                     
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_saveDir' )
                m_nodeName        = m_objPath.fullPathName() # get full string path to node
                m_nodeNamePartial = getShortName( m_nodeName )
                m_objType  = cmds.getAttr( '%s._201102_type'      %m_nodeName )
                m_saveDir  = cmds.getAttr( '%s._201102_saveDir'   %m_nodeName )
                # m_xrayResourcesPathMeshes = 'XRAY_RESOURCES_PATH/models'
                m_saveDir = m_saveDir.replace(m_xrayResourcesPathMeshes, '' , 1)                 
                # -----------------------------
                if ( m_objInput == m_nodeName ):
                    m_list = []
                    m_list.append( addSpacesToString( m_nodeNamePartial,   m_maxLenght[0] - len( m_nodeNamePartial ) ) )    # box_01
                    m_list.append( addSpacesToString( m_nodeName,          m_maxLenght[1] - len( m_nodeName ) ) )           # |props|box|box_01
                    m_list.append( addSpacesToString( m_objType,           m_maxLenght[2] - len( m_objType ) ) )            # Static 
                    m_list.append( addSpacesToString( m_saveDir,           m_maxLenght[3] - len( m_saveDir ) ) )            # props                                 
                    return string.join(m_list,'    ')   # return string 'box_01    |props|box|box_01    Static    props'                    
            except:
                pass
            m_listIt.next()
        return None

    def getMaxLenList(self):
        """
        RETURN: [maxLen(getShortName(m_obj)), maxLen(m_obj), maxLen(m_objType), maxLen(m_saveDir)]
        """
        m_maxLenght =  [0,0,0,0]
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList )        

        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()                  
            try:
                m_listIt.getDagPath( m_objPath ) # get current Dag
                m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj                     
                m_nodeFn.attribute( '_201102_objects' )
                m_nodeFn.attribute( '_201102_type' )
                m_nodeFn.attribute( '_201102_saveDir' )
                m_nodeName        = m_objPath.fullPathName() # get full string path to node
                m_nodeNamePartial = getShortName( m_nodeName )
                m_objType  = cmds.getAttr( '%s._201102_type'      %m_nodeName )
                m_saveDir  = cmds.getAttr( '%s._201102_saveDir'   %m_nodeName )
                # m_xrayResourcesPathMeshes = 'XRAY_RESOURCES_PATH/models'
                m_saveDir = m_saveDir.replace(m_xrayResourcesPathMeshes, '' , 1)                 
                # -----------------------------
                # find maxLen strings
                if ( m_maxLenght[0] < len( m_nodeNamePartial ) ):
                    m_maxLenght[0] = len( m_nodeNamePartial )
                if ( m_maxLenght[1] < len( m_nodeName ) ):
                    m_maxLenght[1] = len( m_nodeName )
                if ( m_maxLenght[2] < len( m_objType ) ):
                    m_maxLenght[2] = len( m_objType )
                if ( m_maxLenght[3] < len( m_saveDir ) ):
                    m_maxLenght[3] = len( m_saveDir )                    
            except:
                pass
            m_listIt.next()
        return m_maxLenght 

    def exportDataNoDBSel( self ):
        self.exportData( True, True )
    
    def exportDataNoDBAll( self ):
        self.exportData( False, True )

    def exportDataAll( self ):
        self.exportData( False, False )
    
    def exportDataSel( self ):
        self.exportData( True, False )

    def exportData(self, m_onlySelected = False, m_ndb = False ):
        """
        INPUT:  m_onlySelected - boolean, if set to True export only selected objects from the list
                                          if set to False export all objects from the list
        RETURN: None if XRAY_RESOURCES_PATH directory don't exists
        """
        m_erroNumber = 0
        if (None == getRealXrayResourcesPath( 'models' )):
            self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            return None
        # ------------------------------------------------------------
        if ( 0 == OpenMaya.MGlobal_mayaState() ):  # MAIN PROGRESS BAR 
            gMainProgressBar = mel.eval('$tmp = $gMainProgressBar')
            cmds.progressBar(   gMainProgressBar,
                                edit=True,
                                beginProgress=False,
                                isInterruptable=False,
                                maxValue=100 )
            cmds.progressBar(gMainProgressBar, edit=True, beginProgress=True) # BEGIN PROGRESS
        # ------------------------------------------------------------
        m_storedDataStatic = {} # create dictionary for static objects
        m_storedDataDynamic = {} # create dictionary for dynamic objects
        # -------------------------
        #     READ CONFIG
        # -------------------------
        m_xrayResourcesPathMeshesConfig = getRealXrayResourcesPath( 'models' ) + '/categories.cfg'
        if (os.path.exists(m_xrayResourcesPathMeshesConfig)):
            m_configRead = ConfigParser.ConfigParser()
            try:
                m_configRead.read(m_xrayResourcesPathMeshesConfig) # check file for parsing errors
            except ConfigParser.ParsingError: # File contains parsing errors
                self.errorHandle.raiseMessage("m_meshNode::exportData categories.cfg file contains parsing errors")
            else:
                try: # Don't have parsing errors
                    m_configReadSectionStatic  = m_configRead.items('Static') # try to read 'static' section
                    m_configReadSectionDynamic = m_configRead.items('Dynamic') # try to read 'dynamic' section
                except ConfigParser.NoSectionError: # section don't exists
                    self.errorHandle.raiseMessage("m_meshNode::exportData categories.cfg section don't exists")
                else: # section exists lets read a file
                    for m_obj in m_configReadSectionStatic:
                        if ('' == m_obj[1] or 'null' == m_obj[1]):
                            m_storedDataStatic[m_obj[0]] = []
                        else:
                            m_storedDataStatic[m_obj[0]] = m_obj[1].rsplit()
                    for m_obj in m_configReadSectionDynamic:
                        if ('' == m_obj[1] or 'null' == m_obj[1]):
                            m_storedDataDynamic[m_obj[0]] = []
                        else:
                            m_storedDataDynamic[m_obj[0]] = m_obj[1].rsplit()
        # -------------------------
        #     READ NODE
        # -------------------------
        m_selectionList = self.getNode()         
        m_listIt = OpenMaya.MItSelectionList( m_selectionList ) 
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()   

            try:
                m_listIt.getDagPath( m_objPath ) # get current Dag
                m_nodeFn.setObject( m_objPath.node() ) # set MFn to MObj
                m_nodeFn.attribute( '_201102_objects' )    # '|boxes|box1'
                m_nodeFn.attribute( '_201102_isSel' )      # True
                m_nodeFn.attribute( '_201102_type' )       # 'Static'
                m_nodeFn.attribute( '_201102_selTags' )    # ['Location|Industrial', 'Architectural|Building']
                m_nodeFn.attribute( '_201102_saveDir' )    # 'XRAY_RESOURCES_PATH/models/props/boxes'
                m_obj             = m_objPath.fullPathName() # get full string path to node
                m_nodeName        = m_obj
                m_nodeNamePartial = getShortName( m_nodeName )
                m_IsSelected      = cmds.getAttr( '%s._201102_isSel'     %m_nodeName )
                m_objType         = cmds.getAttr( '%s._201102_type'      %m_nodeName )
                m_objSelTags      = cmds.getAttr( '%s._201102_selTags'   %m_nodeName )
                m_saveDir         = cmds.getAttr( '%s._201102_saveDir'   %m_nodeName )
                m_objNameHash  = m_saveDir.replace( m_xrayResourcesPathMeshes, '' , 1 ) + '/' + m_nodeNamePartial   # 'props/boxes/box1'
                m_objString    = self.getObjLabelFromNode( m_obj ) # 'box_01    |props|box|box_01    Static    props/boxes'
            except:
                self.errorHandle.raiseMessage( "m_meshNode.exportData %s item Error" %m_obj )
            else:
            # -------------------------------                    
                if ( not m_onlySelected ): # reload all objects
                    m_IsSelected = True
                if ( bool( m_IsSelected ) ):
                    if m_storedDataStatic.has_key( m_objNameHash ):
                        del m_storedDataStatic[m_objNameHash]
                    if m_storedDataDynamic.has_key( m_objNameHash ):
                        del m_storedDataDynamic[ m_objNameHash ]
                    if ( m_objType == 'Static' ):
                        # -----------------------
                        # export STATIC object
                        # -----------------------
                        # Check lod0
                        m_objLOD0visual = m_obj
                        m_objLOD0filename = getRealXrayResourcesPath( 'models' ) + m_objNameHash
                        m_objLOD0filename = m_objLOD0filename.lower()
                        # -------------------------------------------------------------------------------------------------------------------------
                        if (cmds.objExists(m_objLOD0visual)):
                            try:
                                cmds.xray_solid_visual_export( ndb = m_ndb,
                                                               root    = m_objLOD0visual,
                                                               outfilename    = m_objLOD0filename, tootle=True, thq=True)
                                m_storedDataStatic[m_objNameHash] = m_objSelTags # redefine tags
                                #self.errorHandle.raiseMessage("   %s  == > Successful %s " %( m_objString, m_objLOD0visual ) )
                            except:
                                #self.errorHandle.raiseMessage("ERROR   %s  == > Can't export %s" %( m_objString, m_objLOD0visual ) )
                                m_erroNumber = 4
                        # -------------------------------------------------------------------------------------------------------------------------
                        # None LODs
                        else:
                            self.errorHandle.raiseMessage("ERROR   %s  == > Can't find %s" %( m_objString, m_objLOD0visual ) )
                            m_erroNumber = 4
                    if (m_objType == 'Dynamic'):
                        #m_storedDataDynamic[m_objNameHash] = m_objSelTags # redefine tags
                        self.errorHandle.raiseMessage("DYNAMIC EXPORTER  %s  == > DON'T WORK YET" %m_objString)
                        # -----------------------
                        # export DYNAMIC object
                        # -----------------------                                   
            # ------------------------------------------------------------
            if ( 0 == OpenMaya.MGlobal_mayaState() ):  # INCREASE PROGRESS STEP
                cmds.progressBar(gMainProgressBar, edit=True, step=100/m_selectionList.length() )
            m_listIt.next()
        # -------------------------
        #     WRITE CONFIG
        # -------------------------
        m_xrayResourcesPathMeshesConfig = getRealXrayResourcesPath( 'models' ) + '/categories.cfg'
        m_configWrite = OrderedRawConfigParser()
        m_configWrite.add_section('Static')
        m_configWrite.add_section('Dynamic')
        for m_key in m_storedDataStatic.keys():
            m_configWrite.set('Static', m_key, string.join(m_storedDataStatic[m_key],' '))
        for m_key in m_storedDataDynamic.keys():
            m_configWrite.set('Dynamic', m_key, string.join(m_storedDataDynamic[m_key],' '))
        # with open(m_xrayResourcesPathMeshesConfig, 'wb') as m_configfile:
        #     m_configWrite.write(m_configfile) 
        # ------------------------------------------------------------                     
        if ( 0 == OpenMaya.MGlobal_mayaState() ):
            cmds.progressBar(gMainProgressBar, edit=True, endProgress=True) # END PROGRESS
        return m_erroNumber