import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import os, sys 
m_debug = False

def changeMaterialTypeAll( m_sourceType, m_destType ):
    m_listOfMaterials = cmds.ls( type = m_sourceType )
    for m_mat in m_listOfMaterials:
        if ( not isObjFromReferencedFile( m_mat ) ):
            if ( changeMaterialType( m_mat, m_destType ) ):
                sys.stdout.write( "%s successfully changed FROM %s TO %s\n" %( m_mat, m_sourceType, m_destType ) )
            else:
                return False
    return True

def changeMaterialType( m_oldMatName, m_newMatType ):
    """
    m_newMatType - string, like: lambert, blinn, phong
    """
    m_matTypes = [ 'lambert', 'blinn', 'phong', 'xray_shader' ]
    if ( not m_matTypes.count( m_newMatType ) ):
        if ( m_debug ): sys.stderr.write( "Specify material type, like: lambert\n" )
        return False
    try:
        m_newMatName = cmds.shadingNode( m_newMatType , asShader = True )
        if ( m_debug ): sys.stdout.write( "new material %s successfully created\n" %m_newMatName )
    except:
        if ( m_debug ): sys.stderr.write( "Can't create new material\n" )
        return False
    copyAllShaderParameters( m_oldMatName, m_newMatName )
    copyAllShaderConnections( m_oldMatName, m_newMatName )
    m_oldSGNames = getSGFromMaterial( m_oldMatName )
    m_newSGNames = getSGFromMaterial( m_newMatName )
    for m_oldSG in m_oldSGNames:
        try:
            cmds.connectAttr( "%s.outColor" %m_newMatName, "%s.surfaceShader" %m_oldSG, force = True ) 
            if ( m_debug ): sys.stdout.write( "Successful connectAttr from %s.outColor to %s.surfaceShader\n" %( m_newMatName, m_oldSG ) )
        except:
            if ( m_debug ): sys.stderr.write( "Can't connectAttr from %s.outColor to %s.surfaceShader\n" %( m_newMatName, m_oldSG ) )
            return False 
    try:
        cmds.delete( m_oldMatName )
        if ( m_debug ): sys.stdout.write( "%s successfully deleted\n" %m_oldMatName )
    except:
        if ( m_debug ): sys.stderr.write( "Can't delete %s\n" %m_oldMatName )
        return False
    try:
        cmds.rename( m_newMatName, m_oldMatName )
        if ( m_debug ): sys.stdout.write( "%s successfully renamed to %s\n" %( m_newMatName, m_oldMatName ) )
    except:
        if ( m_debug ): sys.stderr.write( "Can't rename %s to %s\n" %( m_newMatName, m_oldMatName ) )
        return False
    return True

def copyAllShaderParameters( m_oldMatName, m_newMatName ):
    m_commonShaderAttributes = [ "colorR", "colorG", "colorB", "transparencyR", "transparencyG", "transparencyB" ]
    for m_attr in m_commonShaderAttributes:
        m_oldAttr = cmds.getAttr( "%s.%s" %( m_oldMatName, m_attr ) )
        try:
            cmds.setAttr( "%s.%s" %( m_newMatName, m_attr ), m_oldAttr )
            if ( m_debug ): sys.stdout.write( "Successful setAttr %s.%s to %s\n" %( m_newMatName, m_attr, m_oldAttr )  )
        except:
            if ( m_debug ): sys.stderr.write( "Can't setAttr %s.%s to %s\n" %( m_newMatName, m_attr, m_oldAttr ) )

def copyAllShaderConnections( m_oldMatName, m_newMatName ):
    m_commonShaderConnections =  [ "color", "transparency" ]
    for m_attr in m_commonShaderConnections:
        m_oldConnection = cmds.connectionInfo( "%s.%s" %( m_oldMatName, m_attr ), sourceFromDestination = True )
        if ( m_oldConnection ):            
            try:
                cmds.connectAttr( m_oldConnection, "%s.%s" %( m_newMatName, m_attr ) )
                if ( m_debug ): sys.stdout.write( "Successful connectAttr from %s to %s.%s \n" %( m_oldConnection, m_newMatName, m_attr ) )
            except:
                if ( m_debug ): sys.stderr.write( "Can't connectAttr from %s to %s.%s\n" %( m_oldConnection, m_newMatName, m_attr ) )
                
def getSGFromMaterial( m_material ):
    """
    Returns the Shading Group set whose '.surfaceShader'
    attribute is being fed by the specified Material.
    Returns an list of all Shading Groups using the Material.
    """
    m_return = []
    m_classifications = cmds.getClassification( cmds.nodeType( m_material ) )
    m_classifications = m_classifications[0]
    if ( "shader/surface" == m_classifications ):
        if ( cmds.connectionInfo( "%s.outColor" %m_material, isSource = True ) ):    
            m_destSGs = cmds.connectionInfo( "%s.outColor" %m_material, destinationFromSource = True )
            for m_dest in m_destSGs:
                if ( "shadingEngine" == cmds.nodeType( m_dest ) ):
                    if ( -1 != m_dest.find( ".surfaceShader" ) ):
                        m_return.append( m_dest.replace( ".surfaceShader", "", 1 ) )
    return m_return

def isObjFromReferencedFile( m_objName ):
    """
    Indicates whether or not this node came from a referenced file.
    INPUT:  m_objName - string like: "|group1|grp1"
    RETURN: True - if m_objName from Reference file, False otherwise      
    """   
    m_selectionList = OpenMaya.MSelectionList()     
    m_objMObj = OpenMaya.MObject()                
    try:
        m_selectionList.add( m_objName )               
    except:
        return False
    try:
        m_selectionList.getDependNode(0, m_objMObj )
        m_fnDepNode = OpenMaya.MFnDependencyNode( m_objMObj )          
    except:
        return False     
    return m_fnDepNode.isFromReferencedFile()