#-----------------------------------------------------------------------------------
#	SCRIPT			isInstanced.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#    if a node is instanced - Select all instances
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select objects
#    3. Run from MEL or add to Shelf:
#       python("import isInstanced; isInstanced.isInstanced()");
#
#-----------------------------------------------------------------------------------
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds

def isInstanced():
    """
    if a node is instanced - Select all instances 
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_selectionListFinal = OpenMaya.MSelectionList() # selection list for instanced objects
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    m_selListIt = OpenMaya.MItSelectionList( m_selectionList )
    
    while not m_selListIt.isDone():
        m_dagPath = OpenMaya.MDagPath()
        m_selListIt.getDagPath( m_dagPath )
        try:
            m_dagPath.extendToShape() # don't ignore transform without shapes
        except:
            m_selectionListFinal.add( m_dagPath.fullPathName() )
            print( "WITHOUT SHAPES: %s" %m_dagPath.fullPathName() )           
        else:
            if ( m_dagPath.isInstanced() ): # object already instanced
                # get all instances
                m_allParents = cmds.listRelatives( m_dagPath.fullPathName(), fullPath = True, allParents = True )
                print( "INSTANCES: %s" %m_allParents )
                for m_dagString in m_allParents:
                    m_selectionListFinal.add( m_dagString )
            else:
                m_dagPath = OpenMaya.MDagPath()
                m_selListIt.getDagPath( m_dagPath )
                m_selectionListFinal.add( m_dagPath.fullPathName() )
                print( "NOT INSTANCED: %s" %m_dagPath.fullPathName() )     
        m_selListIt.next()
    
    m_resultString = 'select -clear;\n'    
    m_finListIt = OpenMaya.MItSelectionList( m_selectionListFinal )
    while not m_finListIt.isDone():
        m_dagPath = OpenMaya.MDagPath()
        m_finListIt.getDagPath( m_dagPath )
        m_resultString += "select -add %s;\n" %m_dagPath.fullPathName()
        m_finListIt.next()
        
    OpenMaya.MGlobal.executeCommand( m_resultString )