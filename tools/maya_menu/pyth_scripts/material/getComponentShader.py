#-----------------------------------------------------------------------------------
#	SCRIPT		getComponentShader.py
#	AUTHOR		Zaitsev Evgeniy
#                       ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#	
#     if keysPressed == "" 	- select the shader assigned to the active poly face
#     if keysPressed == "Ctrl" 	- assign shader selected shader
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Run from MEL or add to Shelf:
#       python("import getComponentShader; getComponentShader.getComponentShader()");
#
#-----------------------------------------------------------------------------------
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
#-----------------------------------------------------------------------------------
#	global variables
#-----------------------------------------------------------------------------------
global m_globalShadingGroup

def getComponentShader():
    """
    main function
    """
    m_keys = keysPressed()
    if   ( ''     == m_keys ):
        m_tempShadingGroup = getShadingGroupName()
        if ( m_tempShadingGroup ):
            global m_globalShadingGroup      
            m_globalShadingGroup = m_tempShadingGroup
            OpenMaya.MGlobal.displayInfo( 'Get %s' %m_globalShadingGroup )
    elif ( 'Ctrl' == m_keys ):
        try:
            m_tempShadingGroup = m_globalShadingGroup
        except:
            m_tempShadingGroup = 'initialShadingGroup'
        if ( setShadingGroupByName( m_tempShadingGroup ) ):
            OpenMaya.MGlobal.displayInfo( 'Set %s' %m_tempShadingGroup )
        else:
            OpenMaya.MGlobal.displayWarning( "Can't set shading group" )
 
def keysPressed():
    m_mods = cmds.getModifiers()
    m_keys = ''
    if (m_mods & 1) > 0: 
        m_keys = 'Shift'
    if (m_mods & 2) > 0: 
        m_keys = 'CapsLock'
    if (m_mods & 4) > 0: 
        m_keys = 'Ctrl'
    if (m_mods & 8) > 0: 
        m_keys = 'Alt'
    return m_keys     
            
def getShadingGroupName():
    """
    RETURN: string - ShadingGroupName, name of assigned shading group to current polygon
            None - if can't find any Shading group, or have some errors, see declaration
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_objPath = OpenMaya.MDagPath() 
    m_stat = True
    m_sel_string = []
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    m_selectionList.getSelectionStrings( m_sel_string )
    try:
        m_selectionList.getDagPath( 0, m_objPath )
    except:
        m_stat = False
        OpenMaya.MGlobal.displayWarning( "Select polygonal mesh" )
        return None 
    if ( 1 < len( m_sel_string ) ):
        OpenMaya.MGlobal.displayWarning( "Select only one polygon" )
        m_stat = False
        return None
    if ( 1 < m_selectionList.length() ):
        OpenMaya.MGlobal.displayWarning( "Select only one polygonal mesh" )
        m_stat = False
        return None    
    try:
        m_face_name = m_sel_string[0]
        m_idx1 = m_face_name.index('[')
        m_idx2 = m_face_name.index(']')
        # get selected polygon index
        m_face_index = int( m_face_name[ m_idx1 + 1 : m_idx2 ] )
    except:
        OpenMaya.MGlobal.displayWarning( "Select only one polygon" )
        m_stat = False
        return None     
    if ( m_stat ):
        m_objPath.extendToShape()
        if ( m_objPath.apiType() == OpenMaya.MFn.kMesh ):
            m_meshFn = OpenMaya.MFnMesh( m_objPath )
            m_sets  = OpenMaya.MObjectArray()
            m_comps = OpenMaya.MObjectArray()
            m_instanceNumber = m_objPath.instanceNumber()
            m_meshFn.getConnectedSetsAndMembers( m_instanceNumber, m_sets, m_comps, 1 )
            m_sets_len = m_sets.length()
            # if mesh have more then one ShadingGroup 
            # last set contain all polygons, ignore it
            if ( 1 < m_sets_len ):
                m_sets_len -= 1               
            m_bfound = False
            m_ind = 0
            while ( m_ind < m_sets_len and not m_bfound ):
                m_sefFn = OpenMaya.MFnSet( m_sets[ m_ind ] )
                m_tempFaceIt = OpenMaya.MItMeshPolygon( m_objPath, m_comps[ m_ind ] )
                while not m_tempFaceIt.isDone(): # iterate
                    if ( m_tempFaceIt.index() == m_face_index ):
                        return m_sefFn.name()
                        #print " ASSIGNED set name = " + m_sefFn.name()
                        #m_bfound = True
                        #break
                    m_tempFaceIt.next()
                m_ind += 1
    return None 

def setShadingGroupByName( m_SGselected ):
    """
    RETURN: True  - if shading group set to current selection
            False - otherwise
    """
    if ( m_SGselected ): 
        cmds.sets( edit  = True, forceElement = m_SGselected )
        return True
    return False