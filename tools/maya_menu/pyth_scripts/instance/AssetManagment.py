#-----------------------------------------------------------------------------------
#	SCRIPT			AssetManagment.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#       createAssetFromSelection - create Asset from selection
#       exportSelectedAssets()   - export assets from selection
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select objects
#    3. Run from MEL or add to Shelf:
#       python("import AssetManagment; AssetManagment.createAssetFromSelection()");
#       python("import AssetManagment; AssetManagment.exportSelectedAssets()");
#
#-----------------------------------------------------------------------------------
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya    
import os
import re
from AssetManagmentContext import *     # common functions
import cleanPerFaceAssignment
#-----------------------------------------------------------------------------------
#	ASSET CREATION
#-----------------------------------------------------------------------------------
def createAssetFromSelection():
    m_assetName = None
    m_selList = cmds.ls( selection = True )
    if ( 0 != len( m_selList ) ):
        for m_selObj in m_selList:
            if ( -1 != m_selObj.find("LOD0") ):
                m_assetName = m_selObj.replace( "_LOD0", "" )
    if ( None == m_assetName ):
        m_container = cmds.container(addNode = m_selList,
                                        type = 'dagContainer',
                                        includeShaders = True,
                                        includeHierarchyBelow = True, 
                                        includeTransform = True, 
                                        force = True)
    else:                            
        m_container = cmds.container(addNode = m_selList,
                                        type = 'dagContainer', 
                                        includeShaders = True,
                                        includeHierarchyBelow = True, 
                                        includeTransform = True, 
                                        force = True, 
                                        name = m_assetName)
    cmds.xform( m_container, worldSpace = True, rotatePivot = ( 0, 0, 0 ) )
    cmds.xform( m_container, worldSpace = True, scalePivot = ( 0, 0, 0 ) )
#-----------------------------------------------------------------------------------
#	ASSET EXPORTING
#-----------------------------------------------------------------------------------
def exportSelectedAssets():
    m_filepath = OpenMaya.MFileIO.currentFile()
    m_filename = os.path.splitext( os.path.split( m_filepath )[1] )[0]
    m_tr = m_transformHideRestore( ) 
                  
    if ( 'untitled' == m_filename ):
        OpenMaya.MGlobal.displayWarning( 'please, save current scene' )
    else:
        m_selList = cmds.ls( selection = True )
        m_selectionList = OpenMaya.MSelectionList()   
        m_selectionListFinal = OpenMaya.MSelectionList()
        m_dagContainerList = OpenMaya.MSelectionList()
        OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
        m_selListIt = OpenMaya.MItSelectionList( m_selectionList )
        if ( 0 != m_selectionList.length() ):
            m_instDir = createInstanceDir()
            #----------------- add all objects to m_selectionListFinal -----------------
            while not m_selListIt.isDone():
                m_dagPath = OpenMaya.MDagPath()
                m_selListIt.getDagPath( m_dagPath )    
                m_selectionListFinal.add( m_dagPath )
                m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )
                m_dagIterator.reset( m_dagPath.node(), OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )
                while not m_dagIterator.isDone():
                    m_objCurr = m_dagIterator.currentItem()
                    m_fnDagCurr = OpenMaya.MFnDagNode( m_objCurr )
                    m_dagCurr = OpenMaya.MDagPath()        
                    m_fnDagCurr.getPath( m_dagCurr )
                    m_selectionListFinal.add( m_dagCurr )
                    m_dagIterator.next()    
                m_selListIt.next()
            #----------------- remove duplicates from  m_selectionListFinal and non Assets -----------------
            m_finListIt = OpenMaya.MItSelectionList( m_selectionListFinal )
            while not m_finListIt.isDone():
                m_dagPathCurr = OpenMaya.MDagPath() 
                m_finListIt.getDagPath( m_dagPathCurr )
                m_objCurr = m_dagPathCurr.node()
                if ( m_objCurr.hasFn( OpenMaya.MFn.kDagContainer ) ):
                     if ( not m_dagContainerList.hasItem( m_dagPathCurr ) ):
                         m_dagContainerList.add( m_dagPathCurr )
                m_finListIt.next()        
            #----------------- iterate only by unique items -----------------
            if ( m_instDir ):
                m_dagContainerIt = OpenMaya.MItSelectionList( m_dagContainerList )
                while not m_dagContainerIt.isDone():
                    m_dagPathCurr = OpenMaya.MDagPath()
                    m_dagContainerIt.getDagPath( m_dagPathCurr )
                    # ----------------------------------------------------------------------------------
                    if ( not isObjFromReferencedFile( m_dagPathCurr.fullPathName() ) and isObjDagContainer( m_dagPathCurr.fullPathName() ) ):
                        # select current Asset
                        cmds.select( m_dagPathCurr.fullPathName(), replace = True )
                        # create filename
                        m_filename = "%s/%s.mb" %( m_instDir, getShortName( m_dagPathCurr.fullPathName() ) )
                        m_result = m_tr.setAsset( m_dagPathCurr.fullPathName() )
                        m_tr.fixShadigGroup()
                        m_tr.transfromsHide()
                        cmds.setAttr( "%s.blackBox" %m_dagPathCurr.fullPathName(), True )
                        try:
                            cmds.file( m_filename,
                                                force = True,
                                                options = "v=0",
                                                type = "mayaBinary",
                                                preserveReferences = True,
                                                exportSelected = True )
                        except:
                            OpenMaya.MGlobal.displayWarning( "Could not save file %s" %m_filename )
                        else:
                            OpenMaya.MGlobal.displayInfo( "Successful %s" %m_filename )
                        cmds.setAttr( "%s.blackBox" %m_dagPathCurr.fullPathName(), False )
                        m_tr.transfromsRestore()
                        # ----------------------------------------------------------------------------------
                    m_dagContainerIt.next() 
            cmds.select( m_selList, replace = True )                
#-----------------------------------------------------------------------------------
#	m_transformHideRestore CLASS
#-----------------------------------------------------------------------------------
class m_transformHideRestore( object ):
    """
    class used for hide (lock) and unhide (unlock) all transfroms - childs of transform m_assetName (string)
    ignore '*_LOD0' transform
    USAGE:  
    """
    def __init__( self ):
        """
        initialize the default values
        """
        self.m_obj_array = OpenMaya.MObjectArray()
        self.m_mobjAsset = OpenMaya.MObject()
        self.m_mobjLOD0 = OpenMaya.MObject()
        self.m_mobjParent = OpenMaya.MObject()
    
    def setAsset( self, m_assetName ):
        """
        INPUT  m_assetName ( String ) - name of needed asset     
        """
        m_selectionList = OpenMaya.MSelectionList()     # create selectionList
        m_objPath = OpenMaya.MDagPath()        
        try:
            m_selectionList.add( m_assetName )              # add object to selectionList     
        except:
            return False
        try:
            m_selectionList.getDagPath( 0, m_objPath )        # get first element in the selection list and connect with MDagPath
        except:
            return False
        self.m_mobjAsset = m_objPath.node()
        return True

    def fixShadigGroup( self ):
        """
        1. remember all assigned Shading Groups
        2. assign initialShadingGroup 
        3. reassign all Shading groups
        """ 
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
        m_dagIterator.reset( self.m_mobjAsset, OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
        while not m_dagIterator.isDone(): 
            m_objCurr = m_dagIterator.currentItem()
            m_dagPath = OpenMaya.MDagPath( ) 
            m_dagCurr = OpenMaya.MFnDagNode( m_objCurr )
            m_dagCurr.getPath( m_dagPath )       
            if ( m_objCurr.hasFn( OpenMaya.MFn.kMesh ) ):
                cleanPerFaceAssignment.cleanPerFaceAssignmentItemEasy( m_dagPath.fullPathName() )
            m_dagIterator.next() # next item                     

    def getLOD0( self ):
        """
        get MObject to '*_LOD0' object 
        """
        self.m_mobjLOD0 = OpenMaya.MObject()
        
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
        m_dagIterator.reset( self.m_mobjAsset, OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
        while not m_dagIterator.isDone():
            m_objCurr = m_dagIterator.currentItem()    
            m_dagCurr = OpenMaya.MFnDagNode( m_objCurr )
            if ( m_objCurr.hasFn( OpenMaya.MFn.kTransform ) ):  
                m_objLOD0 = getShortName( m_dagCurr.fullPathName() )
                if ( -1 != m_objLOD0.find("LOD0") ):
                    self.m_mobjLOD0 = m_objCurr
                    return True            
            m_dagIterator.next() # next item
        return False

    def getTransforms( self ):
        """
        get all childs of transform m_assetName (string) ignore '*_LOD0' transform
        """ 
        if ( not self.m_mobjLOD0.isNull() ):
            # dag to LOD0 object
            m_fnDagLOD0 = OpenMaya.MFnDagNode( self.m_mobjLOD0 )
            # lock throught Asset m_objPath.node()
            m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
            m_dagIterator.reset( self.m_mobjAsset, OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
            while not m_dagIterator.isDone(): 
                m_objCurr = m_dagIterator.currentItem()    
                m_dagCurr = OpenMaya.MFnDagNode( m_objCurr )
                if ( m_objCurr.hasFn( OpenMaya.MFn.kTransform ) ):
                    if ( not m_dagCurr.hasChild( self.m_mobjLOD0 ) and  self.m_mobjLOD0 != m_objCurr  ):
                        if ( not m_dagCurr.hasParent( self.m_mobjLOD0 ) ):                 
                            self.m_obj_array.append( m_objCurr )
                m_dagIterator.next() # next item
            
    def transfromsHide( self ):
        """
        hide and lock visibility
        """
        m_result = self.getLOD0()
        self.getTransforms()
        if ( m_result ):
            for m_ind in range( self.m_obj_array.length() ):
                m_dagCurr = OpenMaya.MFnDagNode( self.m_obj_array[ m_ind ])
                cmds.setAttr( "%s.visibility" %m_dagCurr.fullPathName(), lock = False ) 
                cmds.setAttr( "%s.visibility" %m_dagCurr.fullPathName(), False )
                cmds.setAttr( "%s.visibility" %m_dagCurr.fullPathName(), lock = True )
        # PARENT current Asset to WORLD
        m_fnDagAsset = OpenMaya.MFnDagNode( self.m_mobjAsset )
        self.m_mobjParent = m_fnDagAsset.parent( 0 )
        if ( not self.m_mobjParent.hasFn( OpenMaya.MFn.kWorld ) ):
            m_fnDagParent = OpenMaya.MFnDagNode( self.m_mobjParent )
            cmds.parent( m_fnDagAsset.fullPathName(), world = True )
    
    def transfromsRestore( self ):
        """
        unhide and unlock visibility
        """
        m_result = self.getLOD0()                                   
        self.getTransforms()
        if ( m_result ):
            for m_ind in range( self.m_obj_array.length() ):
                m_dagCurr = OpenMaya.MFnDagNode( self.m_obj_array[ m_ind ]) 
                cmds.setAttr( "%s.visibility" %m_dagCurr.fullPathName(), lock = False ) 
                cmds.setAttr( "%s.visibility" %m_dagCurr.fullPathName(), True )
        else:
            m_dagCurr = OpenMaya.MFnDagNode( self.m_mobjAsset )
            print "    >>> %s_LOD0 don't exists in Asset " %getShortName( m_dagCurr.fullPathName() )
        # PARENT current Asset to old Parent
        if ( not self.m_mobjParent.hasFn( OpenMaya.MFn.kWorld ) ):
            m_fnDagAsset = OpenMaya.MFnDagNode( self.m_mobjAsset )
            m_fnDagParent = OpenMaya.MFnDagNode( self.m_mobjParent )
            cmds.parent( m_fnDagAsset.fullPathName(), m_fnDagParent.fullPathName() )