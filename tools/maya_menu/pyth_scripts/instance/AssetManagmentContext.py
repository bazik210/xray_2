#-----------------------------------------------------------------------------------
#	SCRIPT			AssetManagmentContext.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#       Define tools for AssetManagment
#
#       USAGE:
#
#       from AssetManagmentContext import *     # common functions
#
#-----------------------------------------------------------------------------------
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import os
import re
#-----------------------------------------------------------------------------------
#	COMMON FUNCTIONS
#-----------------------------------------------------------------------------------
def selectObjectsByMask( m_maskStr, m_ignoreTransformsWithoutShape = True, m_ignoreInstances = False ):
    """
    select objects by mask, ignore - asset childs
    INPUT:
            m_maskStr                         - string     - mask for making selection (use mask like "border*" or use current selection )
            m_ignoreTransformsWithoutShape    - boolean    - indicate ignore transforms whithout shapes or not
            m_ignoreInstances                 - boolean    - indicate ignore instances or not            
    USAGE: 
            selectObjectsByMask( "border*", True, True )
    """
    m_selectionListFinal = OpenMaya.MSelectionList()
    m_dagPath = OpenMaya.MDagPath()
    # ----------------------------------------------------------------------------------
    # create Filters
    m_filterTransforms = cmds.itemFilter( byType = 'transform' )
    m_filterMask       = cmds.itemFilter( byName = m_maskStr )
    m_filterUnion      = cmds.itemFilter( intersect = ( m_filterTransforms, m_filterMask ) )
    # get list by Filter
    m_selList = cmds.lsThroughFilter( m_filterUnion )
    # ----------------------------------------------------------------------------------
    if ( not m_ignoreInstances ):
        if ( m_selList ):
            for m_obj in m_selList:
                m_shapeList = cmds.listRelatives( m_obj, fullPath = True )
                for m_shape in m_shapeList:
                    m_instList = cmds.listRelatives( m_shape, fullPath = True, allParents = True )
                    for m_inst in m_instList:
                        if ( 0 == m_selList.count( m_inst ) ):
                            m_selList.append( m_inst )
    # ----------------------------------------------------------------------------------
    # iterarte by list
    if ( m_selList ):
        for m_obj in m_selList:
            if ( not isObjInDagContainer( m_obj ) ):
                if ( m_ignoreTransformsWithoutShape ):
                # IGNORE transform whithout shapes
                    if ( isObjDagContainer( m_obj ) ):
                        # is Asset add to selectionList
                        m_dagPath = getMDagPathFromNodeName( m_obj )
                        if ( m_dagPath ):
                            if ( not m_selectionListFinal.hasItem( m_dagPath ) ):
                                m_selectionListFinal.add( m_dagPath )
                    elif ( isObjWhithShape( m_obj ) ):
                        m_dagPath = getMDagPathFromNodeName( m_obj )
                        if ( m_dagPath ):
                            if ( not m_selectionListFinal.hasItem( m_dagPath ) ):
                                m_selectionListFinal.add( m_dagPath )
                # DON'T ignore transform whithout shapes
                else:
                    m_dagPath = getMDagPathFromNodeName( m_obj )
                    if ( m_dagPath ):
                        if ( not m_selectionListFinal.hasItem( m_dagPath ) ):
                            m_selectionListFinal.add( m_dagPath )
    # delete Filters
    cmds.delete( m_filterTransforms, m_filterMask, m_filterUnion )
    # ----------------------------------------------------------------------------------
    m_resultString = 'select -clear;\n'
    m_finListIt = OpenMaya.MItSelectionList( m_selectionListFinal )
    while not m_finListIt.isDone():
        m_dagPathCurr = OpenMaya.MDagPath()
        m_finListIt.getDagPath( m_dagPathCurr )
        # ----------------------------------------------------------------------------------
        m_resultString += "select -add %s;\n" %m_dagPathCurr.fullPathName()
        # ----------------------------------------------------------------------------------
        m_finListIt.next()
    OpenMaya.MGlobal.executeCommand( m_resultString ) 

def isObjWhithShape( m_objName ):
    """
    Indicates whether or not this node has shape ( OpenMaya.MFn.kMesh )
    INPUT:  m_objName - string like: "|group1|grp1"
    RETURN: True  - if m_objName have kMesh shape
            False - if m_objName don't have kMesh shape
    """
    m_selectionList = OpenMaya.MSelectionList()     
    m_objPath = OpenMaya.MDagPath()
    try:                 
        m_selectionList.add( m_objName )               
        m_selectionList.getDagPath(0, m_objPath )
    except:
        return False
    return m_objPath.hasFn( OpenMaya.MFn.kMesh )

def TRSPPSPfromTo( m_source, m_destination ):
    """
    copy translation, rotation, scale, rotatePivot, scalePivot from m_source to m_destination 
    T  - translation
    R  - rotation
    S  - scale
    RP - rotatePivot
    SP - scalePivot
    INPUT:  m_source        - string like: "|group1|grp1"
            m_destination   - string like: "|group1|grp2"
    RETURN: True if done properly, False otherwise
    """
    try:
        m_translation = cmds.xform( m_source, query = True, worldSpace = True, translation = True )
        m_rotation    = cmds.xform( m_source, query = True, worldSpace = True, rotation = True )
        # Warning: Cannot query scale in worldspace.  Defaulting to local space.
        m_scale       = cmds.xform( m_source, query = True, relative = True , scale = True )
        m_rotatePivot = cmds.xform( m_source, query = True, worldSpace = True, rotatePivot = True )
        m_scalePivot  = cmds.xform( m_source, query = True, worldSpace = True, scalePivot = True )            
        cmds.xform( m_destination, worldSpace = True, translation = m_translation )
        cmds.xform( m_destination, worldSpace = True, rotation = m_rotation )
        cmds.xform( m_destination, scale = m_scale )
        cmds.xform( m_destination, worldSpace = True, rotatePivot = m_rotatePivot )
        cmds.xform( m_destination, worldSpace = True, scalePivot = m_scalePivot )
        return True
    except:
        return False

def isObjFromReferencedFile( m_objName ):
    """
    Indicates whether or not this node came from a referenced file.
    INPUT:  m_objName - string like: "|group1|grp1"
    RETURN: True - if m_objName from Reference file, False otherwise      
    """
    m_selectionList = OpenMaya.MSelectionList()     
    m_objPath = OpenMaya.MDagPath()                 
    try:
        m_selectionList.add( m_objName )               
    except:
        return False
    try:
        m_selectionList.getDagPath(0, m_objPath )
        m_fnDagNode = OpenMaya.MFnDagNode( m_objPath )        
    except:
        return False     
    return m_fnDagNode.isFromReferencedFile()

def isObjInDagContainer( m_objName, m_result = False ):
    """
    Indicates whether or not this node child of any Asset
    INPUT:  m_objName - string like: "|group1|grp1"
    RETURN: True - if m_objName is child of OpenMaya.MFn.kDagContainer, False otherwise     
    """ 
    m_selectionList = OpenMaya.MSelectionList()     
    m_objPath = OpenMaya.MDagPath() 
    try:
        m_selectionList.add( m_objName )                  
    except:
        return m_result 
    try:
        m_selectionList.getDagPath( 0, m_objPath )        
    except:
        return m_result 
    m_fnDagNode       = OpenMaya.MFnDagNode( m_objPath )
    m_objParent       = m_fnDagNode.parent(0)
    m_fnDagNodeParent = OpenMaya.MFnDagNode( m_objParent )
    if ( m_objParent.hasFn( OpenMaya.MFn.kDagContainer ) ):
        m_result = True
    if ( not m_objParent.hasFn( OpenMaya.MFn.kWorld ) ):
        m_result = isObjInDagContainer( m_fnDagNodeParent.fullPathName(), m_result )
    #print "obj = %s     parent = %s     world = %s    res = %s" %( m_objName, m_fnDagNodeParent.fullPathName() , m_objParent.hasFn( OpenMaya.MFn.kWorld ), m_result )        
    return m_result

def isObjDagContainer( m_objName ):
    """
    Indicates whether or not this node came from a referenced file.
    INPUT:  m_objName - string like: "|group1|grp1"
    RETURN: True - if m_objName is OpenMaya.MFn.kDagContainer, False otherwise     
    """  
    m_selectionList = OpenMaya.MSelectionList()     
    m_objPath = OpenMaya.MDagPath()
    try:
        m_selectionList.add( m_objName )                  
    except:
        return False 
    try:
        m_selectionList.getDagPath(0, m_objPath)        
    except:
        return False 
    if ( m_objPath.hasFn( OpenMaya.MFn.kDagContainer ) ):
        return True
    return False

def getMDagPathFromNodeName( m_node_name ):
    """
    INPUT:  m_node_name = '|group6|group5'
    RETURN: MDagPath if m_node_name exists,
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_objPath = OpenMaya.MDagPath()
    try:            
        m_selectionList.add( m_node_name )
        m_selectionList.getDagPath( 0, m_objPath )
    except:
        return None
    return m_objPath

def getShortName( m_string ):
    """
    INPUT:  string like this: '|props|box|box_01'
    RETURN: 'box_01'
    """
    m_list = []
    m_looklike = r'[^|]*$' # r - raw, init string as is
    m_matcher = re.compile(m_looklike,re.IGNORECASE)
    m_list = m_matcher.findall(m_string)
    return m_list[0]

def createInstanceDir():
    m_filepath = OpenMaya.MFileIO.currentFile()
    m_filename = os.path.splitext( os.path.split( m_filepath )[1] )[0]
    m_currDir  = os.path.split( m_filepath )[0]
    m_createDir = "%s/%s_instance" %( m_currDir, m_filename )
    if ( not os.path.exists( m_createDir ) ):
        try:
            os.mkdir( m_createDir )
        except:
            OpenMaya.MGlobal.displayWarning( "Can't create directory %s" %m_createDir )
            return False
    return m_createDir
    
def setBlackBoxAll():
    m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )
    while not m_dagIterator.isDone(): 
        m_objCurr = m_dagIterator.currentItem()    
        m_dagCurr = OpenMaya.MFnDagNode( m_objCurr )
        if ( m_objCurr.hasFn( OpenMaya.MFn.kDagContainer ) ):
            cmds.setAttr( "%s.blackBox" %m_dagCurr.fullPathName(), False )
            cmds.setAttr( "%s.blackBox" %m_dagCurr.fullPathName(), True )
        m_dagIterator.next() # next item    
