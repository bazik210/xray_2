#-----------------------------------------------------------------------------------
#	SCRIPT			cleanPerFaceAssignment.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
# based on C++ cleanPerFaceAssignmentCmd.cpp
# Author: Hiroyuki Haga
#
#	DESCRIPTION:
#
#    1. It traces material connections
#    2. If multiple conections to one material is found, it reassign all materials
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select mesh with multiple connections to one material
#    3. Run from MEL or add to Shelf:
#       python("import cleanPerFaceAssignment; cleanPerFaceAssignment.cleanPerFaceAssignment()");
#
#-----------------------------------------------------------------------------------
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya            # work with maya API

def printObjectSGInfo():
    """
    display Shape information like:

    |group6|group5|pCube4|pCube4Shape
    -----------------------
    lambert3SG
    FaceCount: 4
     1 2 3 5
    -----------------------
    -----------------------
    lambert2SG
    FaceCount: 1
     0
    -----------------------
    
    """
    # Get the selection list and using the iterator to trace them
    m_list = OpenMaya.MSelectionList()     # create selectionList
    OpenMaya.MGlobal.getActiveSelectionList( m_list )
    m_listIt = OpenMaya.MItSelectionList( m_list )
    m_dagList = OpenMaya.MSelectionList() # temp list    
    while not m_listIt.isDone():
        # Get the DAG path of each element and extend the path to its shape node
        m_path = OpenMaya.MDagPath()
        m_listIt.getDagPath( m_path )
        #print m_path.fullPathName()
        #m_path.extendToShape()
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
        m_dagIterator.reset( m_path.node(), OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
        while not m_dagIterator.isDone(): # iterate
            m_objCurr = m_dagIterator.currentItem()    # current item        
            if ( m_objCurr.hasFn( OpenMaya.MFn.kMesh ) ):  # if current == kMesh
                m_objNameMObj = OpenMaya.MObject( m_objCurr )  # create MObject and connect with current item 
                if ( not m_dagList.hasItem( m_objCurr ) ):
                    m_dagList.add( m_objCurr )
                    m_tmp_path = OpenMaya.MDagPath() 
                    m_dagIterator.getPath( m_tmp_path )
                    # -----------------------------------------
                    printSGInfo( m_tmp_path )
                    # -----------------------------------------    
            m_dagIterator.next() # next item    
        m_listIt.next() 

def printSGInfo( m_path ):
    """
    INPUT:  OpenMaya.MDagPath() to OpenMaya.MFn.kMesh
    """
    m_path.extendToShape()
    print m_path.fullPathName()
    m_meshFn = OpenMaya.MFnMesh( m_path )
    m_sets  = OpenMaya.MObjectArray()
    m_comps = OpenMaya.MObjectArray()
    m_instanceNumber = m_path.instanceNumber()
    m_meshFn.getConnectedSetsAndMembers( m_instanceNumber, m_sets, m_comps, 1 )
    m_sets_len = m_sets.length()
    # if mesh have more then one ShadingGroup
    # last set contain all polygons, ignore it
    if ( 1 < m_sets_len ):
        m_sets_len -= 1
    for i in range( m_sets_len ):
        m_setFn = OpenMaya.MFnSet( m_sets[i] )
        m_faceIt = OpenMaya.MItMeshPolygon( m_path, m_comps[i] )
        print( "-----------------------" ) 
        print( m_setFn.name() )
        print( "FaceCount: %d" %m_faceIt.count() )
        m_tempStr = ''
        while not m_faceIt.isDone():
            m_tempStr += ' %d' %m_faceIt.index()
            m_faceIt.next()
        print( m_tempStr )
        print( "-----------------------" )

def cleanPerFaceAssignment( m_bNeedReconectSG = False ):
    """
    1. It traces material connections
    2. If multiple conections to one material is found, it reassign all materials
    """    
    # Get the selection list and using the iterator to trace them
    m_list = OpenMaya.MSelectionList()     # create selectionList
    OpenMaya.MGlobal.getActiveSelectionList( m_list )
    m_listIt = OpenMaya.MItSelectionList( m_list )
    m_dagList = OpenMaya.MSelectionList() # temp list
    m_resultString = 'select -clear;\n' # for Restoring current selection
    while not m_listIt.isDone():
        # Get the DAG path of each element and extend the path to its shape node
        m_path = OpenMaya.MDagPath()
        m_listIt.getDagPath( m_path )
        m_resultString += "select -add %s;\n" %m_path.fullPathName()
        #print m_path.fullPathName()
        #m_path.extendToShape()
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
        m_dagIterator.reset( m_path.node(), OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
        while not m_dagIterator.isDone(): # iterate
            m_objCurr = m_dagIterator.currentItem()    # current item        
            if ( m_objCurr.hasFn( OpenMaya.MFn.kMesh ) ):  # if current == kMesh
                m_objNameMObj = OpenMaya.MObject( m_objCurr )  # create MObject and connect with current item 
                if ( not m_dagList.hasItem( m_objCurr ) ):
                    m_dagList.add( m_objCurr )
                    m_tmp_path = OpenMaya.MDagPath() 
                    m_dagIterator.getPath( m_tmp_path )
                    # -----------------------------------------
                    cleanPerFaceAssignmentItem( m_tmp_path, m_bNeedReconectSG )
                    # -----------------------------------------
            m_dagIterator.next() # next item    
        m_listIt.next()
    OpenMaya.MGlobal.executeCommand( m_resultString ) # Restore Selection

def cleanPerFaceAssignmentItem( m_path, m_bNeedReconectSG = False ):
    """
    INPUT:  OpenMaya.MDagPath() to OpenMaya.MFn.kMesh
    """ 
    m_path.extendToShape()
    m_meshFn = OpenMaya.MFnMesh( m_path )
    m_sets  = OpenMaya.MObjectArray()
    m_comps = OpenMaya.MObjectArray()
    m_instanceNumber = m_path.instanceNumber()
    m_meshFn.getConnectedSetsAndMembers( m_instanceNumber, m_sets, m_comps, 1 )
    m_sets_len = m_sets.length()
    # if mesh have more then one ShadingGroup 
    # last set contain all polygons, ignore it
    if ( 1 < m_sets_len ):
        m_sets_len -= 1        
    # ------------------------------------------ 
    # Variable declaration 
    m_SGNames = []                              # Stores Shading Group name
    m_sameSGFaceCount = OpenMaya.MIntArray()    # storoes the number of faces included in each shading group
    m_memberFaceNames = []                      # stores names of faces included in each shading group
    m_sameSGFaceCount.clear()
    # ------------------------------------------------
    # 1    Initialization of variables
    # ------------------------------------------------       
    for i in range( m_sets_len ):
        # SGName Initialization
        # Append Shading Group name
        m_setFn = OpenMaya.MFnSet( m_sets[i] )
        m_SGNames.append( m_setFn.name() )
        # sameSGFaceCount Initialization
        # Set the number of faces included in a Shading Group
        m_tempFaceIt = OpenMaya.MItMeshPolygon( m_path, m_comps[i] )
        m_sameSGFaceCount.append( m_tempFaceIt.count() )
        # memberFaceNames Initialization
        m_aMemberFaceName = ''
        m_optimizeList = True
        if ( m_optimizeList ):
            m_lastIndices = [ -1, -1 ]
            m_haveFace = False
            while not m_tempFaceIt.isDone():
                if ( -1 == m_lastIndices[0] ):
                    m_lastIndices[0] = m_tempFaceIt.index()
                    m_lastIndices[1] = m_tempFaceIt.index()
                else:
                    m_currentIndex = m_tempFaceIt.index()
                    # Hit non-contiguous face #. split out a new string
                    if ( m_currentIndex > m_lastIndices[1] + 1 ):
                        m_aMemberFaceName += '%s.f[%d:%d] ' %( m_path.fullPathName(), m_lastIndices[0], m_lastIndices[1] )
                        m_lastIndices[0] = m_currentIndex
                        m_lastIndices[1] = m_currentIndex 
                    else:
                        m_lastIndices[1] = m_currentIndex
                m_haveFace = True
                m_tempFaceIt.next()
            # Only one member. Add it.
            if ( m_haveFace ):
                m_aMemberFaceName += '%s.f[%d:%d] ' %( m_path.fullPathName(), m_lastIndices[0], m_lastIndices[1] )
        else:
            while not m_tempFaceIt.isDone():
                m_aMemberFaceName += '%s.f[%d] ' %( m_path.fullPathName(), m_tempFaceIt.index() )
                m_tempFaceIt.next()
        m_memberFaceNames.append( m_aMemberFaceName )
    # ------------------------------------------------
    # 2    Finding redundant connections
    # ------------------------------------------------
    # Scan for multiple connections to a shading group and if it exists 
    # combine them
    for i in range( m_sets_len ):
        m_setFn = OpenMaya.MFnSet( m_sets[i] )
        for j in range( m_sets_len ):
            m_tempSetFn = OpenMaya.MFnSet( m_sets[j] )
            if ( m_setFn.name() == m_tempSetFn.name() ):
                if ( i != j ):
                    m_sameSGFaceCount[i] += m_sameSGFaceCount[j]
                    m_sameSGFaceCount[j] = 0
                    m_memberFaceNames[i] += m_memberFaceNames[j]
                    m_memberFaceNames[j] = ""
    # ------------------------------------------------
    # 4    Output
    # ------------------------------------------------
    #print m_SGNames;
    #print m_sameSGFaceCount;
    #print m_memberFaceNames;
    #m_bNeedReconectSG = False
    for m_sg in m_SGNames:
        if ( 1 != m_SGNames.count( m_sg ) ):
            m_bNeedReconectSG = True
            break     
    if ( m_bNeedReconectSG ):
        m_resultString = ''
        #print ("Try to fix multiple conections to one material in object %s" %m_path.fullPathName() )
        m_resultString += "select -r %s;\n" %m_path.fullPathName()
        m_resultString += "sets -e -forceElement initialShadingGroup;\n"                
        for i in range( m_sameSGFaceCount.length() ):
            if ( 0 != m_sameSGFaceCount[i] ):
                m_resultString += "select -r %s;\n" %m_memberFaceNames[i]
                m_resultString += "sets -e -forceElement %s;\n" %m_SGNames[i]
        m_resultString +=  "select -clear;\n"       
        OpenMaya.MGlobal.executeCommand( m_resultString )

def cleanPerFaceAssignmentEasy( ):
    """
    1. It traces material connections
    2. If multiple conections to one material is found, it reassign all materials
    """
    # Get the selection list and using the iterator to trace them
    m_list = OpenMaya.MSelectionList()     # create selectionList
    OpenMaya.MGlobal.getActiveSelectionList( m_list )
    m_listIt = OpenMaya.MItSelectionList( m_list )
    m_dagList = OpenMaya.MSelectionList() # temp list
    m_resultString = 'select -clear;\n' # for Restoring current selection
    while not m_listIt.isDone():
        # Get the DAG path of each element and extend the path to its shape node
        m_path = OpenMaya.MDagPath()
        m_listIt.getDagPath( m_path )
        m_resultString += "select -add %s;\n" %m_path.fullPathName()
        #print m_path.fullPathName()
        #m_path.extendToShape()
        m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )        # create NULL dagIterator
        m_dagIterator.reset( m_path.node(), OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )  # connect dagIterator with MDagPath             
        while not m_dagIterator.isDone(): # iterate
            m_objCurr = m_dagIterator.currentItem()    # current item        
            if ( m_objCurr.hasFn( OpenMaya.MFn.kMesh ) ):  # if current == kMesh
                m_objNameMObj = OpenMaya.MObject( m_objCurr )  # create MObject and connect with current item 
                if ( not m_dagList.hasItem( m_objCurr ) ):
                    m_dagList.add( m_objCurr )
                    m_tmp_path = OpenMaya.MDagPath() 
                    m_dagIterator.getPath( m_tmp_path )
                    # -----------------------------------------
                    cleanPerFaceAssignmentItemEasy( m_tmp_path.fullPathName() )
                    # -----------------------------------------
            m_dagIterator.next() # next item    
        m_listIt.next()
    OpenMaya.MGlobal.executeCommand( m_resultString ) # Restore Selection

def cleanPerFaceAssignmentItemEasy( m_node_name ):
    """
    INPUT:  m_node_name - string like: "crimppannel_storage_LOD0" 
    """
    # Stores Shading Group name
    m_SGNames = []                              
    m_selectionList = OpenMaya.MSelectionList()
    m_path = OpenMaya.MDagPath()
    try:            
        m_selectionList.add( m_node_name )
        m_selectionList.getDagPath( 0, m_path )
    except:
        return None
    #-----------------------------
    m_path.extendToShape()
    m_SGNames = cmds.listConnections( m_path.fullPathName(), destination = True, source = False, plugs = False, type = "shadingEngine" )
    #-----------------------------
    # Remove duplicates
    if ( m_SGNames ):
        for m_SG in m_SGNames:
            while ( 1 != m_SGNames.count( m_SG ) ):
                m_SGNames.remove( m_SG )
    #-----------------------------
    m_resultString = ''
    m_resultString += "select -r %s;\n" %m_path.fullPathName()
    m_resultString += "sets -e -forceElement initialShadingGroup;\n"     
    if ( m_SGNames ):
        for m_SG in m_SGNames:
            m_tempStr = '' 
            m_tempFaceIt = OpenMaya.MItMeshPolygon( m_path ) 
            while not m_tempFaceIt.isDone():       
                if ( cmds.sets( "%s.f[%s]" %( m_path.fullPathName(), m_tempFaceIt.index() ),  isMember = m_SG ) ):
                    m_resultString += "select -add %s.f[%s];\n" %( m_path.fullPathName(), m_tempFaceIt.index() )
                m_tempFaceIt.next()
            m_resultString += "sets -e -forceElement %s;\n" %m_SG
            m_resultString += "select -clear;\n"
        try:
            OpenMaya.MGlobal.executeCommand( m_resultString )
        except:
            return False
    return True