#-----------------------------------------------------------------------------------
#	SCRIPT			referenceCopyDel.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#       copyTransformWithReferences()   - create copy of selected transforms, understand referenced files
#       deleteTransformWithReferences() - remove selected transform, understand referenced files
#       SelectAssetsBySelection()       - if a selected node is a Asset and from referenced file - Select all nodes like this
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select objects
#    3. Run from MEL or add to Shelf:
#       python("import referenceCopyDel; referenceCopyDel.copyTransformWithReferences()");
#       python("import referenceCopyDel; referenceCopyDel.deleteTransformWithReferences()");
#       python("import referenceCopyDel; referenceCopyDel.SelectAssetsBySelection()");
#
#-----------------------------------------------------------------------------------
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
import os
from AssetManagmentContext import *     # common functions
global m_globalCopiedTransform
#-----------------------------------------------------------------------------------
#	SELECT
#-----------------------------------------------------------------------------------
def SelectAssetsBySelection( ):
    """
    if a selected node is a Asset and from referenced file - Select all nodes like this
    """
    m_selList = cmds.ls( selection = True )
    if ( m_selList ):
        m_selObj = m_selList[ len( m_selList ) - 1 ]
        if ( not isObjFromReferencedFile( m_selObj ) ):
            OpenMaya.MGlobal.displayWarning( "%s not from Referenced file" %m_selObj )
            return 
        if ( not isObjDagContainer( m_selObj ) ):
            OpenMaya.MGlobal.displayWarning( "%s not a Asset" %m_selObj )
            return 
    else:
        OpenMaya.MGlobal.displayWarning( "Select at list one object" )
        return 

    m_dagPathSel = getMDagPathFromNodeName( m_selObj )
    
    m_filename = cmds.reference( m_dagPathSel.fullPathName(), query = True, filename = True, shortName = True )
    m_filename = os.path.splitext( m_filename )[0]
    
    m_resultString = 'select -clear;\n' 
    m_dagIterator = OpenMaya.MItDag( OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid )
    while not m_dagIterator.isDone(): 
        m_objCurr = m_dagIterator.currentItem()    
        m_dagCurr = OpenMaya.MFnDagNode( m_objCurr )
        if ( m_dagCurr.isFromReferencedFile() and m_objCurr.hasFn( OpenMaya.MFn.kDagContainer ) ):
            m_filenameCurr = cmds.reference( m_dagCurr.fullPathName(), query = True, filename = True, shortName = True )
            m_filenameCurr = os.path.splitext( m_filenameCurr )[0]        
            if ( m_filename == m_filenameCurr ):
                m_resultString += "select -add %s;\n" %m_dagCurr.fullPathName()
        m_dagIterator.next() # next item   
    OpenMaya.MGlobal.executeCommand( m_resultString )   

    return
#-----------------------------------------------------------------------------------
#	DELETE
#-----------------------------------------------------------------------------------
def deleteTransformWithReferences():
    m_selList = cmds.ls( selection = True )
    if ( 0 != len( m_selList ) ):
        for m_selObj in m_selList:
            goTroughtTreeRemoveAllReferences( m_selObj )
            try:
                cmds.delete( m_selObj )
            except:
                pass
        cmds.select( clear = True )
    setBlackBoxAll()

def goTroughtTreeRemoveAllReferences( m_objStr ):
    if ( isObjFromReferencedFile( m_objStr ) ):
        m_filename  = cmds.reference( m_objStr, query = True, filename = True )
        cmds.file( m_filename, removeReference = True )    
    else:
        if ( not isObjDagContainer( m_objStr ) ):
            try:
                m_list = cmds.listRelatives( m_objStr, children = True )
                if ( m_list ):
                    for m_item in m_list:
                        goTroughtTreeRemoveAllReferences( m_item )
            except:
                pass            
#-----------------------------------------------------------------------------------
#	COPY
#-----------------------------------------------------------------------------------
def copyTransformWithReferences():
    m_selList = cmds.ls( selection = True )
    if ( 0 != len( m_selList ) ):
        m_selObj = m_selList[ len( m_selList ) - 1 ]
        goTroughtTree( m_selObj )
        cmds.select( clear = True )
        cmds.select( m_globalCopiedTransform, replace = True )
    setBlackBoxAll()

def goTroughtTree( m_objStrChild, m_objStrParent = 'world', m_objStrNewParent = 'world' ):
    #print "Debug Info: %s child of %s" %( m_objStrChild, m_objStrParent )
    # CREATE 
    m_newGrp = copyEntity( m_objStrChild )
    # PARENT
    #print "Debug Info: %s child of %s" %( m_newGrp, m_objStrNewParent )    
    if ( 'world' != m_objStrNewParent ):
        cmds.parent( m_newGrp, m_objStrNewParent )
    else:
        global m_globalCopiedTransform
        m_globalCopiedTransform = m_newGrp
    # MOVE
    TRSPPSPfromTo( m_objStrChild, m_newGrp )
    # FIND CHILDS
    # ignore DagContainer childs
    if ( not isObjDagContainer( m_objStrChild ) ):
        m_list = cmds.listRelatives( m_objStrChild, children = True )        
        if ( None != m_list ):
            if ( 0 != len( m_list ) ):
                for m_item in m_list:
                    goTroughtTree( m_item, m_objStrChild, m_newGrp )             

def copyEntity( m_objStr ): 
    """
    create copy of input m_objStr, if m_objStr is from referenced file - create new reference
    INPUT:  m_objStr        - string like: "|group1|grp1"
    RETURN: String (m_lasAdded) - name of the last added (copied) object
    """
    # IF reference load from file
    if ( isObjFromReferencedFile( m_objStr ) ):
        m_filename  = cmds.reference( m_objStr, query = True, filename = True )
        m_namespace = os.path.splitext( os.path.split( m_filename )[1] )[0]
        cmds.file( m_filename,
                           reference = True, 
                           type = "mayaBinary",
                           sharedNodes = ( "displayLayers", "shadingNetworks" ),
                           namespace = m_namespace,
                           options = "v=0" )
        cmds.select( replace = True, allDagObjects = True )
        m_lasAdded = cmds.ls( selection = True, tail = 1 )
        m_lasAdded = m_lasAdded[0]
        cmds.select( clear = True )
        return m_lasAdded
    else:      
    # IF not from ref file
        m_lasAdded = cmds.group( empty = True, world = True )
        cmds.select( clear = True )
        return m_lasAdded