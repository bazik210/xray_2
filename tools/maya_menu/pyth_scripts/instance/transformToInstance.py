#-----------------------------------------------------------------------------------
#	SCRIPT			transformToInstance.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#    This command replaces selected objects with the lead object. And makes them as a instance of lead object.
#    Lead object - last selected
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select objects
#    3. Run from MEL or add to Shelf:
#       python("import transformToInstance; transformToInstance.transformToInstanceMain()");
#
#-----------------------------------------------------------------------------------
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.mel as mel
import re                                   # regular expression

def transformToInstanceMain():
    """
    main function
    """
    m_keys = mel.eval('keysPressed()')
    if   ( ''     == m_keys ):
        transformToInstance( False )
    elif ( 'Ctrl' == m_keys ):
        transformToInstance( True )

def duplicateInstance():
    """
    RETURN: tuple( OpenMaya.MDagPath(), string )
                   OpenMaya.MDagPath() - to cleared instance node,
                   string              - the name of original object 
            None - if have some errors
    """
    m_selectionList = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    # get last selected item
    m_dagPath = OpenMaya.MDagPath()
    if ( 0 == m_selectionList.length() ):
        print( "Select a least one Object" )
        return ( None, None )
    try:
        m_selectionList.getDagPath( m_selectionList.length() - 1 , m_dagPath )
        m_resultName = getShortName( m_dagPath.fullPathName() )
        m_dagPath.extendToShape()
    except:
        print( "Can't get DagPath" )
        return ( None, None )   
    # duplicate it
    try:
        m_instObj = cmds.instance( m_dagPath.fullPathName() )
    except:
        print( "Can't duplicate %s "  %m_dagPath.fullPathName() )
        return ( None, None )
    m_selectionList = OpenMaya.MSelectionList()
    m_instPath = OpenMaya.MDagPath()
    m_selectionList.add( m_instObj[0] )
    try:
        m_selectionList.getDagPath( 0, m_instPath )  
    except:
        print( "Can't get DagPath %s " %m_instObj[0] )
        return ( None, None )
    try:
        m_fnTransrom = OpenMaya.MFnTransform( m_instPath )
        # reset rotation!
        #
        m_rotMEulerNew = OpenMaya.MEulerRotation( 0, 0, 0 )           
        m_fnTransrom.setRotation( m_rotMEulerNew )
        # reset scale!
        #
        m_doubleArrayNew = OpenMaya.MScriptUtil()
        m_doubleArrayNew.createFromDouble ( 1.0, 1.0, 1.0 )
        m_doubleArrayNewPtr = m_doubleArrayNew.asDoublePtr ()        
        m_fnTransrom.setScale( m_doubleArrayNewPtr )
        # Freeze transfrom
        #cmds.makeIdentity( m_instPath.fullPathName() , apply = True, translate = True, rotate = True, scale = True )
        # move to zero    
        cmds.move( 0, 0, 0, m_instPath.fullPathName(), rotatePivotRelative = True )
        # Freeze transfrom again
        #cmds.makeIdentity( m_instPath.fullPathName() , apply = True, translate = True, rotate = True, scale = True )
    except:
        print( "Can't makeIdentity or move for %s " %m_instPath.fullPathName() )
        return ( None, None )
    return ( m_instPath, m_resultName )

def transformToInstance( m_stat = False ):
    """
    Replace selected transforms by instances based on last object in selection
    INPUT:  m_stat = True if you need to replace and existing instances    
    RETURN: True - if all done properly,
            False - otherwise   
    """ 
    m_lastObjStr = getLastSelectedObject()
    if ( not m_lastObjStr ):
        print( "Some Errors with root instance" )
        return False      
    #------------------------------------------
    m_selectionList = OpenMaya.MSelectionList()
    m_selectionListFinal = OpenMaya.MSelectionList() # selection list for instanced objects
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    m_selListIt = OpenMaya.MItSelectionList( m_selectionList )
    #----------------- add all instances to selection list -----------------  
    while not m_selListIt.isDone():
        m_dagPath = OpenMaya.MDagPath()
        m_selListIt.getDagPath( m_dagPath )
        try:
            m_dagPath.extendToShape() # don't ignore transform without shapes
        except:
            m_selectionListFinal.add( m_dagPath.fullPathName() )   
        else:
            if ( m_dagPath.isInstanced() ): # object already instanced
                # get all instances
                m_allParents = cmds.listRelatives( m_dagPath.fullPathName(), fullPath = True, allParents = True )
                for m_dagString in m_allParents:
                    m_selectionListFinal.add( m_dagString )
            else:
                m_dagPath = OpenMaya.MDagPath()
                m_selListIt.getDagPath( m_dagPath )            
                m_selectionListFinal.add( m_dagPath.fullPathName() )  # fullname without Shape 
        m_selListIt.next()
    #---------------------- create iterator ------------------------------
    if ( m_stat ):        
        m_finListIt = OpenMaya.MItSelectionList( m_selectionListFinal )
    else:
        m_finListIt = OpenMaya.MItSelectionList( m_selectionList )
    #------------------ select all needed objects -------------------------    
    m_resultString = 'select -clear;\n'
    while not m_finListIt.isDone():
        m_dagPath = OpenMaya.MDagPath()
        m_finListIt.getDagPath( m_dagPath )
        m_resultString += "select -add %s;\n" %m_dagPath.fullPathName()                
        m_finListIt.next()
    m_resultString += "select -add %s;\n" %m_lastObjStr
    OpenMaya.MGlobal.executeCommand( m_resultString )
    #---------------------- create iterator ------------------------------
    if ( m_stat ):        
        m_finListIt = OpenMaya.MItSelectionList( m_selectionListFinal )
    else:
        m_finListIt = OpenMaya.MItSelectionList( m_selectionList )    
    #------------------- create root instance -----------------------------            
    m_instPath, m_resultName = duplicateInstance() # prepare root instanced object  
    if ( not m_instPath ):
        print( "Some Errors with root instance" )
        return False                                
    #----------------------------------------------------------------------      
    cmds.select( clear = True )
    m_resultString = 'select -clear;\n'
    while not m_finListIt.isDone():
        m_dagPath = OpenMaya.MDagPath()
        m_finListIt.getDagPath( m_dagPath )
        m_fnTransrom = OpenMaya.MFnTransform( m_dagPath )
        # get Translation!
        #
        m_pointPivot = m_fnTransrom.rotatePivot( OpenMaya.MSpace.kWorld )
        # get Rotation!
        #    
        m_rotMEuler = OpenMaya.MEulerRotation( 0, 0, 0 )
        m_fnTransrom.getRotation( m_rotMEuler )         
        # get Scale!
        #
        m_doubleArray = OpenMaya.MScriptUtil()
        m_doubleArray.createFromDouble ( 1.0, 1.0, 1.0 )
        m_doubleArrayPtr = m_doubleArray.asDoublePtr ()
        m_fnTransrom.getScale( m_doubleArrayPtr )         
        # Let's instance    
        try:
            m_tmpObj = cmds.instance( m_instPath.fullPathName() )
            print ( "create instance %s" %m_tmpObj )
        except:
            print( "Can't instance %s" %m_instPath.fullPathName() )
            break 
        m_tmpObjDagNode = getDagNodeFromNodeName( m_tmpObj[0] )            
        if ( not m_tmpObjDagNode ):
            print( "Can't get DagNode from name %s" %m_tmpObj[0] )
            break
        try:           
            cmds.rename( m_tmpObjDagNode.fullPathName() , m_resultName )
        except:
            print( "Can't rename %s to %s " %( m_tmpObjDagNode.fullPathName(), m_resultName ) )    
        m_tmpObjMObject = getMObjectFromNodeName( m_tmpObjDagNode.fullPathName() )
        if ( not m_tmpObjMObject ):
            print( "Can't get MObject from name %s" %m_tmpObjDagNode.fullPathName() )
            break  
        try:
            m_tmpFnTransform = OpenMaya.MFnTransform( m_tmpObjMObject )
        except:
            print( "Can't connect MFnTransform with MObject %s " %m_tmpObjDagNode.fullPathName() )
            break
        m_tmpObjParentDest = getParentDagNodeFromNodeName( m_dagPath.fullPathName() )
        m_tmpObjParentSrc  = getParentDagNodeFromNodeName( m_tmpObjDagNode.fullPathName() )
        if ( not m_tmpObjParentDest):
            print( "Can't get parent DagNode from name %s" %m_dagPath.fullPathName() )
            break
        if ( not m_tmpObjParentSrc):
            print( "Can't get parent DagNode from name %s" %m_tmpObjDagNode.fullPathName() )
            break
        if ( m_tmpObjParentDest.isDefaultNode() ): # parent to world
            if ( not m_tmpObjParentSrc.isDefaultNode() ):
                try:
                    cmds.parent( m_tmpObjDagNode.fullPathName(), world = True )
                except:
                    print ( "Can't parent %s to world " %m_tmpObjDagNode.fullPathName() )
        else:
            if ( not m_tmpObjParentSrc.isDefaultNode() ):
                try:
                    cmds.parent( m_tmpObjDagNode.fullPathName(), world = True )
                except:
                    print ( "Can't parent %s to world " %m_tmpObjDagNode.fullPathName() )
            try:
                cmds.parent( m_tmpObjDagNode.fullPathName(), m_tmpObjParentDest.fullPathName() )
            except:
                print( "Can't parent %s to %s "  %( m_tmpObjDagNode.fullPathName(), m_tmpObjParentDest.fullPathName() ) ) 
        # set Translation!
        #
        try:
            cmds.move( m_pointPivot.x , m_pointPivot.y, m_pointPivot.z, m_tmpObjDagNode.fullPathName(), rotatePivotRelative = True )
        except:
            print ( "Can't move %s" %m_tmpObjDagNode.fullPathName() )
        # set Rotation
        #
        try:
            m_tmpFnTransform.setRotation( m_rotMEuler )
        except:
            print( "Can't rotate %s" %m_tmpObjDagNode.fullPathName() )
        # set Scale
        #
        try:
            m_tmpFnTransform.setScale( m_doubleArrayPtr )
        except:
            print( "Can't scale %s" %m_tmpObjDagNode.fullPathName() )
        m_resultString += "select -add %s;\n" %m_tmpObjDagNode.fullPathName()
        try:
            print ( "delete 1 %s" %m_dagPath.fullPathName() )
            cmds.delete( m_dagPath.fullPathName() )            
        except:
            print( "Can't delete %s" %m_dagPath.fullPathName() )
        m_finListIt.next()
    #------------------------------------------
    try:
        print ( "delete 2 %s" %m_instPath.fullPathName() )    
        cmds.delete( m_instPath.fullPathName() )         
    except: 
        print( "Can't delete %s " %m_instPath.fullPathName() )
    OpenMaya.MGlobal.executeCommand( m_resultString )  
    renameLastSelectedObject( m_resultName )
    return True

def renameLastSelectedObject( m_inputStr ):
    """
    INPUT:  m_inputStr = 'panel_inn_corner'
    """
    m_selectionList = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    # get last selected item
    m_dagPath = OpenMaya.MDagPath()
    if ( 0 != m_selectionList.length() ):
        try:
            m_selectionList.getDagPath( m_selectionList.length() - 1 , m_dagPath )
        except:
            print( "Can't get DagPath" )
        else:
            try:
                cmds.rename( m_dagPath.fullPathName(),  m_inputStr )
            except: 
                print( "Can't rename %s to %s " %( m_dagPath.fullPathName(),  m_inputStr ) )

def getLastSelectedObject( ):
    """
    RETURN: last object in active selection list
            string like: "|pSphere1|lod0|v0|group11|group10|group7|group1|root27"
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    # get last selected item
    m_dagPath = OpenMaya.MDagPath()
    if ( 0 == m_selectionList.length() ):
        print( "Select a least one Object" )
        return None
    if ( 0 != m_selectionList.length() ):
        try:
            m_selectionList.getDagPath( m_selectionList.length() - 1 , m_dagPath )
        except:
            print( "Can't get DagPath" )
            return None
        else:
            return m_dagPath.fullPathName()
    return None 

def getParentDagNodeFromNodeName( m_node_name ):
    """
    INPUT:  m_node_name = '|group6|group5'
    RETURN: MDagNode of parent for m_node_name, if m_node_name exists,
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_objPath = OpenMaya.MDagPath() 
    try:          
        m_selectionList.add( m_node_name )
        m_selectionList.getDagPath( 0, m_objPath )
        m_fnDagNode = OpenMaya.MFnDagNode( m_objPath )
        m_obj = m_fnDagNode.parent(0)
        m_fnDagNode = OpenMaya.MFnDagNode( m_obj )
    except:
        return None
    return m_fnDagNode 
    
def getDagNodeFromNodeName( m_node_name ):
    """
    INPUT:  m_node_name = '|group6|group5'
    RETURN: MDagNode of m_node_name, if m_node_name exists,
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_objPath = OpenMaya.MDagPath() 
    try:          
        m_selectionList.add( m_node_name )
        m_selectionList.getDagPath( 0, m_objPath )
        m_fnDagNode = OpenMaya.MFnDagNode( m_objPath )
    except:
        return None
    return m_fnDagNode  
    
def getMObjectFromNodeName( m_node_name ):
    """
    INPUT:  m_node_name = 'BaseAnimation'
    RETURN: MObject if m_node_name exists,
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_node = OpenMaya.MObject() 
    try:            
        m_selectionList.add(m_node_name)               
        m_selectionList.getDependNode(0, m_node)
    except:
        return None
    return m_node
    
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