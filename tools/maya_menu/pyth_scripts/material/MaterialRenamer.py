import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import os

def MaterialRenamer():
    m_result = False
    if ( renameFileNodes() ):
        m_result = True
    if ( renameBump2DNodes() ):
        m_result = True
    if ( renameMaterialNodes() ):
        m_result = True
    if ( renameShadingGroups() ):
        m_result = True                
    if ( renameMaterialInfoNodes() ):
        m_result = True    
    if ( renamePlace2dTextureNodes() ):
        m_result = True                
    return m_result

def renamePlace2dTextureNodes():
    m_result = False
    m_listPlace2dTexture = cmds.ls( type = "place2dTexture" )
    for m_place2dTexture in m_listPlace2dTexture:
        if ( not isObjFromReferencedFile( m_place2dTexture ) ):      
            m_connectedNodes = cmds.listConnections( m_place2dTexture, skipConversionNodes = True )
            if ( m_connectedNodes ):
                for m_node in m_connectedNodes:
                    if ( 'file' == cmds.nodeType( m_node ) ): 
                        try:
                            m_file = cmds.getAttr( "%s.fileTextureName" %m_node )
                        except:
                            print( "Can't getAttr %s.fileTextureName" %m_node )
                        else:
                            m_file = os.path.splitext( m_file )[0]
                            m_file = os.path.split( m_file )[1]
                            try:
                                cmds.rename( m_place2dTexture, "%s_place2dTexture" %m_file)
                                m_result = True
                            except:
                                print( "Can't rename %s to %s_place2dTexture" %( m_place2dTexture, m_file ) )
                            else:
                                print( "%s to %s_place2dTexture Renamed Successfully!" %( m_place2dTexture, m_file ) )  
                        break
    return m_result    

def renameMaterialInfoNodes():
    m_result = False
    m_listMI = cmds.ls( type = "materialInfo" )
    for m_mi in m_listMI:
        if ( not isObjFromReferencedFile( m_mi ) ):    
            m_mat = getMaterialFromMaterialInfoNode( m_mi )
            if ( m_mat ):
                m_node = getFirstFileNode( m_mat )
                if ( m_node ):
                    if ( 'file' == cmds.nodeType( m_node ) ):
                        try:
                            m_file = cmds.getAttr( "%s.fileTextureName" %m_node )
                        except:
                            print( "Can't getAttr %s.fileTextureName" %m_node )
                        else:
                            m_file = os.path.splitext( m_file )[0]
                            m_file = os.path.split( m_file )[1]    
                            try:
                                cmds.rename( m_mi, "%s_MatInfo" %m_file)
                                m_result = True
                            except:
                                print( "Can't rename %s to %s_MatInfo" %( m_mi, m_file ) )
                            else:
                                print( "%s to %s_MatInfo Renamed Successfully!" %( m_mi, m_file ) ) 
    return m_result    

def renameShadingGroups():
    m_result = False
    m_listSG = cmds.ls( type = "shadingEngine" )
    for m_sg in m_listSG:
        if ( not isObjFromReferencedFile( m_sg ) ):
            m_mat = getMaterialFromSG( m_sg )
            if ( m_mat ):
                m_node = getFirstFileNode( m_mat )
                if ( m_node ):
                    if ( 'file' == cmds.nodeType( m_node ) ):
                        try:
                            m_file = cmds.getAttr( "%s.fileTextureName" %m_node )
                        except:
                            print( "Can't getAttr %s.fileTextureName" %m_node )
                        else:
                            m_file = os.path.splitext( m_file )[0]
                            m_file = os.path.split( m_file )[1]    
                            try:
                                cmds.rename( m_sg, "%s_s" %m_file)
                                m_result = True
                            except:
                                print( "Can't rename %s to %s_s" %( m_sg, m_file ) )
                            else:
                                print( "%s to %s_s Renamed Successfully!" %( m_sg, m_file ) ) 
    return m_result    
    
def renameMaterialNodes():
    m_result = False
    m_listMat = cmds.ls( materials = True )
    for m_mat in m_listMat:
        if ( not isObjFromReferencedFile( m_mat ) ):
            m_node = getFirstFileNode( m_mat )
            if ( m_node ):
                if ( 'file' == cmds.nodeType( m_node ) ):
                    try:
                        m_file = cmds.getAttr( "%s.fileTextureName" %m_node )
                    except:
                        print( "Can't getAttr %s.fileTextureName" %m_node )
                    else:
                        m_file = os.path.splitext( m_file )[0]
                        m_file = os.path.split( m_file )[1]    
                        try:
                            cmds.rename( m_mat, "%s_m" %m_file)
                            m_result = True
                        except:
                            print( "Can't rename %s to %s_m" %( m_mat, m_file ) )
                        else:
                            print( "%s to %s_m Renamed Successfully!" %( m_mat, m_file ) ) 
    return m_result                                 

def renameBump2DNodes():
    m_result = False
    m_bump2dList = cmds.ls( type = 'bump2d' )
    for m_bump2d in  m_bump2dList:
        if ( not isObjFromReferencedFile( m_bump2d ) ):
            m_connectedNodes = cmds.listConnections( m_bump2d, destination = False, source = True )
            if ( m_connectedNodes ):
                for m_node in m_connectedNodes:
                    if ( 'file' == cmds.nodeType( m_node ) ):
                        try:
                            m_file = cmds.getAttr( "%s.fileTextureName" %m_node )
                        except:
                            print( "Can't getAttr %s.fileTextureName" %m_node )
                        else:
                            m_file = os.path.splitext( m_file )[0]
                            m_file = os.path.split( m_file )[1]
                            try:
                                cmds.rename( m_bump2d, "%s_bump2d" %m_file)
                                m_result = True
                            except:
                                print( "Can't rename %s to %s_bump2d" %( m_bump2d, m_file ) )
                            else:
                                print( "%s to %s_bump2d Renamed Successfully!" %( m_bump2d, m_file ) )  
    return m_result                         

def renameFileNodes( ):
    m_result = False
    m_fileList = cmds.ls( type = 'file' )
    for m_fileNode in m_fileList:
        if ( not isObjFromReferencedFile( m_fileNode ) ):
            try:
                m_file = cmds.getAttr( "%s.fileTextureName" %m_fileNode )
            except:
                print( "Can't getAttr %s.fileTextureName" %m_fileNode )
            else:
                # get
                m_fileExt      = os.path.splitext( m_file )[1] 
                m_fileName     = os.path.splitext( m_file )[0]
                # cut
                if ( -1 != m_fileExt.find( '.' ) ):
                    m_fileExt  = m_fileExt.replace( '.' , '' )
                m_fileName     = os.path.split( m_fileName )[1]
                try:
                    cmds.rename( m_fileNode, "%s_%s" %( m_fileName, m_fileExt ) )
                    m_result = True
                except:
                    print( "Can't rename %s to %s_%s" %( m_fileNode, m_fileName, m_fileExt ) )
                else:
                    print( "%s to %s_%s Renamed Successfully!" %( m_fileNode, m_fileName, m_fileExt ) )
    return m_result  

def getFirstFileNode( m_nodeInput ):
    m_connectedNodes = cmds.listConnections( m_nodeInput, destination = False, source = True )
    if ( m_connectedNodes ):
        for m_node in m_connectedNodes: 
            if    ( 'file' == cmds.nodeType( m_node ) ):
                return m_node
            elif  ( 'bump2d' == cmds.nodeType( m_node ) ):
                getFirstFileNode( m_node )
    return None 

def getMaterialFromSG( m_sg ):
    m_material = None
    if ( "shadingEngine" == cmds.nodeType( m_sg ) and 
         cmds.connectionInfo( "%s.surfaceShader" %m_sg, isDestination = True ) ):
        m_material = cmds.connectionInfo( "%s.surfaceShader" %m_sg,  sourceFromDestination = True )  
        if ( -1 != m_material.find( ".outColor" ) ):
            m_material = m_material.replace( ".outColor", "", 1 )
    return m_material    

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

def getMaterialFromMaterialInfoNode( m_materialInfo ):
    m_material = None
    m_node = OpenMaya.MObject()
    m_nodeFn = OpenMaya.MFnDependencyNode() # create a function set
    m_nodePlug = OpenMaya.MPlug()      
    try:
        m_node = getMObjectFromNodeName( m_materialInfo )
        m_nodeFn.setObject( m_node )            # connect MObject with function set
        m_nodeAttr = m_nodeFn.attribute( 'material' )
        m_nodePlug = OpenMaya.MPlug( m_node, m_nodeAttr ) 
        m_connectedPlugs = OpenMaya.MPlugArray()
        if ( m_nodePlug.connectedTo( m_connectedPlugs, True, False ) ):
            m_plug_destination = m_connectedPlugs[0]
            m_material = getNodeNameFromeMObject( m_plug_destination.node() )
    except:
        return m_material
    return m_material
    
def getNodeNameFromeMObject( m_node ):
    """
    INPUT:  m_node - MObject
    RETURN: string like 'BaseAnimation'
            None - have an error
    """
    m_nodeFn = OpenMaya.MFnDependencyNode()
    try: 
        m_nodeFn.setObject(m_node)
    except:
        return None
    return m_nodeFn.name()       

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