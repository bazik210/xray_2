#-----------------------------------------------------------------
#	SCRIPT			exportXRayContextAnim.py
#	AUTHOR			Zaitsev Evgeniy
#                   ev.zaitsev@gmail.com
#	DATE:			15 July 2010
#
#	DESCRIPTION:	Define tools for animation exporter
#                   S.T.A.L.K.E.R. 2 project
#
#  VERSION:	        201101
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
m_mode = 'release' # debug, release
#-----------------------------------------------------------------------------------
#	ANIMATIONS NODE CLASS
#-----------------------------------------------------------------------------------
class m_animNode(object):
    """
    store a dictionary of animations, their DAG data in the script Node
    """
    def __init__(self):
        """
        initialize the default attributes
        """
        self.errorHandle = m_meshTrace() # error tracer
    
    def getNode(self):
        """
        connect with node if node exists in the scene,
        if node don't exists - check mayaState variable,
        if mayaState set to 0 - create new node
        if mayaState set to 2 - do nothing
        RETURN: MObject - if node exists connect with node, 
                          if node don't exists and mayaState == 0 create node and connect
                False   - if node don't exists amd mayaState != 2
        """
        # ----------------------------------------------------------------------------------------------
        # CONVERT OLD DATA v2011.01 to v2011.02
        if ( 0 == OpenMaya.MGlobal_mayaState() ):
          if ( not self.convertOld( ) ):
              self.errorHandle.raiseMessage( "Data convert error %s" %m_animNodeName )
        # ----------------------------------------------------------------------------------------------
        m_animNode = self.addAttr()
        if ( m_animNode ):
            self.lockAllItems()
            return m_animNode
        else:
            return False

    def addAttr( self ):
        """
        Add needed attributes to m_animNodeName
        """
        m_selectionList = OpenMaya.MSelectionList()    # create selection list
        m_animNode      = OpenMaya.MObject()           # create MObject
        m_animNodeFn    = OpenMaya.MFnDependencyNode() # create a function set     
        try:
            m_selectionList.add( m_animNodeName )   # add node with name 'xray_anim_lister'
        except:
            if ( 0 == OpenMaya.MGlobal_mayaState() ):
                #------------------
                #    CREATE NODE
                #------------------            
                cmds.createNode( 'script', name = m_animNodeName )
            else:
                return False
        m_selectionList.add( m_animNodeName )          
        m_selectionList.getDependNode(0, m_animNode)  # connect with MObject first element in the selection list
        m_animNodeFn.setObject( m_animNode )          # connect MObject with function set    
        # ----------------------------------------------------------------------------------------------            
        try:
            m_animNodeFn.attribute('_201102_skins')  # find attribute by name 'skins'
        except:
            if (0 == OpenMaya.MGlobal_mayaState()):
                #------------------
                #    ADD ATTR
                #------------------
                cmds.addAttr(m_animNodeName, longName         = '_201102_skins',
                                             niceName         = 'Skinned Meshes',
                                             attributeType    = 'compound',
                                             numberOfChildren = 2,
                                             multi = True)
                cmds.addAttr(m_animNodeName, parent        = '_201102_skins',
                                                                                longName      = '_201102_skinNameSkins',
                                                                                niceName      = 'Mesh Name',
                                                                                dataType      = 'string' )
                cmds.addAttr(m_animNodeName, parent        = '_201102_skins',
                                                                                longName      = '_201102_isSkinSel',
                                                                                niceName      = 'Is Mesh Selected',
                                                                                attributeType = 'bool')
            else:
                self.errorHandle.raiseMessage( "%s" %OpenMaya.MFileIO.currentFile() )
                self.errorHandle.raiseMessage( "Found old version of data stored in %s node, convert it please" %m_animNodeName )
                return False
        # ---------------------------------------------------------------------------------------------- 
        try:         
            m_animNodeFn.attribute('_201102_animLayersMessage')  # find attribute by name 'animLayersMessage'
        except:
            if (0 == OpenMaya.MGlobal_mayaState()):
                cmds.addAttr( m_animNodeName, longName = '_201102_animLayersMessage',
                                              niceName = 'Anim Layer Message',
                                              attributeType = 'message',
                                              multi = True ) 
            else:
                self.errorHandle.raiseMessage( "%s" %OpenMaya.MFileIO.currentFile() )
                self.errorHandle.raiseMessage( "Found old version of data stored in %s node, convert it please" %m_animNodeName )
                return False        
        # ----------------------------------------------------------------------------------------------           
        try:
            m_animNodeFn.attribute('_201102_clips')  # find attribute by name 'clips'
        except:
            if (0 == OpenMaya.MGlobal_mayaState()):                    
                cmds.addAttr(m_animNodeName, longName          = '_201102_clips',
                                             niceName          = 'Animation Clips',
                                             attributeType     = 'compound', 
                                             numberOfChildren  = 15, 
                                             multi             = True)
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_skinNameClips',
                                                                             niceName      = 'Mesh Name',        
                                                                             dataType      = 'string' )
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_animName',
                                                                             niceName      = 'Animation Name',
                                                                             dataType      = 'string' )
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_isAnimSel',
                                                                             niceName      = 'Is Animation Selected',
                                                                             attributeType = 'bool')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',
                                                                             longName      = '_201102_startFrame',
                                                                             niceName      = 'Start Frame',
                                                                             attributeType = 'float')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',
                                                                             longName      = '_201102_endFrame',
                                                                             niceName      = 'End Frame',
                                                                             attributeType = 'float')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_LCC',
                                                                             niceName      = 'Local Coordinat Center',
                                                                             dataType      = 'string')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_saveDir',
                                                                             niceName      = 'Save to',
                                                                             dataType      = 'string')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_saveDirIsDefault',
                                                                             niceName      = 'Save to Is Default',
                                                                             attributeType = 'bool')                                         
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',
                                                                             longName      = '_201102_FPS',
                                                                             niceName      = 'Time',
                                                                             attributeType = 'long')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_epsilon',
                                                                             niceName      = 'Epsilon',
                                                                             attributeType = 'float')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_epsilonIsDefault',
                                                                             niceName      = 'Epsilon Is Default',
                                                                             attributeType = 'bool')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_isAdditive',
                                                                             niceName      = 'Is Additive',
                                                                             attributeType = 'bool')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_baseAnimation',
                                                                             niceName      = 'Base Animation',
                                                                             dataType      = 'string')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',   
                                                                             longName      = '_201102_baseAnimationIsDefault',
                                                                             niceName      = 'Base Animation Is Default',
                                                                             attributeType = 'bool')
                cmds.addAttr(m_animNodeName, parent        = '_201102_clips',
                                                                             longName      = '_201102_animLayers',
                                                                             niceName      = 'Anim Layers List',
                                                                             attributeType = 'bool',
                                                                             multi = True )
            else:
                self.errorHandle.raiseMessage( "%s" %OpenMaya.MFileIO.currentFile() )
                self.errorHandle.raiseMessage( "Found old version of data stored in %s node, convert it please" %m_animNodeName )
                return False 
        return m_animNode       

    def convertOld( self ):
        """
        RETURN: True  - if old Data converted 
                False - otherwise
        INPUT:  m_version - string like 'from201101to201102'
        """
        if ( 0 != OpenMaya.MGlobal_mayaState() ):
            return False
        m_skins = {} # list of entries "_201101_skins"
        m_clips = {} # list of entries "_201101_clips"
        #------------------------------------------
        m_animNode = OpenMaya.MObject()
        m_animNode = getMObjectFromNodeName( m_animNodeName ) 
        if ( m_animNode ):
            m_animNodeFn = OpenMaya.MFnDependencyNode( m_animNode )
        else:
            return True # can't find connection with node                 
        try:
            #------------------------------------------
            #  from201101to201102 
            #------------------------------------------          
            m_animNodeFn.attribute('_201101_skins') 
            m_animNodeFn.attribute('_201101_clips')   
        except:
            return True # can't find old data
        else:                     
            #------------------------------------------
            #  SKINS
            #------------------------------------------             
            try:
                m_indexesSkins = OpenMaya.MIntArray()
                m_animNodeAttrSkins = m_animNodeFn.attribute('_201101_skins')                
                m_animNodePlugSkins = OpenMaya.MPlug( m_animNode, m_animNodeAttrSkins )
                m_animNodePlugSkins.getExistingArrayAttributeIndices( m_indexesSkins )
            except:
                return False              
            for m_index in m_indexesSkins:
                # GET
                m_skinName     = cmds.getAttr( "%s._201101_skins[%s]._201101_skinNameSkins"    %( m_animNodeName, m_index ) )
                m_isSkinSel    = cmds.getAttr( "%s._201101_skins[%s]._201101_isSkinSel"        %( m_animNodeName, m_index ) )
                # SET
                m_skins[ m_index ] = ( m_skinName, m_isSkinSel )
            #------------------------------------------
            #  CLIPS 
            #------------------------------------------                                 
            try:
                m_indexesClips = OpenMaya.MIntArray()
                m_animNodeAttrClips = m_animNodeFn.attribute( '_201101_clips' )                
                m_animNodePlugClips = OpenMaya.MPlug( m_animNode, m_animNodeAttrClips )
                m_animNodePlugClips.getExistingArrayAttributeIndices( m_indexesClips )
            except:
                return False
            for m_index in m_indexesClips:
                # GET                
                m_skinName         = cmds.getAttr( "%s._201101_clips[%s]._201101_skinNameClips"    %( m_animNodeName, m_index ) )
                m_animName         = cmds.getAttr( "%s._201101_clips[%s]._201101_animName"         %( m_animNodeName, m_index ) )    
                m_isAnimSel        = cmds.getAttr( "%s._201101_clips[%s]._201101_isAnimSel"        %( m_animNodeName, m_index ) )
                m_startFrame       = cmds.getAttr( "%s._201101_clips[%s]._201101_startFrame"       %( m_animNodeName, m_index ) )
                m_endFrame         = cmds.getAttr( "%s._201101_clips[%s]._201101_endFrame"         %( m_animNodeName, m_index ) )
                m_localCoordCenter = cmds.getAttr( "%s._201101_clips[%s]._201101_LCC"              %( m_animNodeName, m_index ) )
                m_FPS              = cmds.getAttr( "%s._201101_clips[%s]._201101_FPS"              %( m_animNodeName, m_index ) )
                m_saveDirIsDefault = cmds.getAttr( "%s._201101_clips[%s]._201101_saveDirIsDefault" %( m_animNodeName, m_index ) )
                m_epsilonIsDefault = cmds.getAttr( "%s._201101_clips[%s]._201101_epsilonIsDefault" %( m_animNodeName, m_index ) )
                if ( m_saveDirIsDefault ):
                    m_saveDir      = m_anim_saveDirDefault
                else:                    
                    m_saveDir      = cmds.getAttr( "%s._201101_clips[%s]._201101_saveDir"          %( m_animNodeName, m_index ) )
                if ( m_epsilonIsDefault ):
                    m_epsilon      = m_anim_epsilonDefault
                else:                    
                    m_epsilon      = cmds.getAttr( "%s._201101_clips[%s]._201101_epsilon"          %( m_animNodeName, m_index ) )
                if ( m_epsilon > m_anim_epsilonMax ):
                    m_epsilon = m_anim_epsilonMax
                if ( m_epsilon < m_anim_epsilonMin ):
                    m_epsilon = m_anim_epsilonMin                 
                # SET
                m_clips[ m_index ] = ( m_skinName, m_animName, m_isAnimSel, m_startFrame, m_endFrame, m_localCoordCenter, 
                                        m_saveDir, m_saveDirIsDefault, m_FPS, m_epsilon, m_epsilonIsDefault )
            #------------------------------------------
            #  Let's go delete the node
            #------------------------------------------                                                      
            cmds.delete( m_animNodeName )
            m_animNode = self.addAttr()
            if ( not m_animNode ):
                return False            
            if ( 0 != len( m_skins ) ):
                for obj,data in m_skins.iteritems():
                    m_index     = obj
                    m_skinName  = data[0]
                    m_isSkinSel = bool( data[1] )              
                    cmds.setAttr( '%s._201102_skins[%s]._201102_skinNameSkins' %( m_animNodeName, m_index ),  m_skinName,  type = 'string' )
                    cmds.setAttr( '%s._201102_skins[%s]._201102_isSkinSel'     %( m_animNodeName, m_index ),  m_isSkinSel )                      
            if ( 0 != len( m_clips ) ):
                for obj,data in m_clips.iteritems():
                    m_index                    = obj
                    m_skinName                 = data[0]
                    m_animName                 = data[1]
                    m_isAnimSel                = bool( data[2] )
                    m_startFrame               = data[3]
                    m_endFrame                 = data[4]
                    m_localCoordCenter         = data[5]
                    m_saveDir                  = data[6]
                    m_saveDirIsDefault         = bool( data[7] )
                    m_FPS                      = data[8]
                    m_epsilon                  = data[9]
                    m_epsilonIsDefault         = bool( data[10] )
                    m_isAdditive               = False
                    m_baseAnimation            = m_anim_baseAnimationDefault
                    m_baseAnimationIsDefault   = True
                    cmds.setAttr( '%s._201102_clips[%s]._201102_skinNameClips'          %( m_animNodeName, m_index ), m_skinName, type = 'string' )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_animName'               %( m_animNodeName, m_index ), m_animName, type = 'string' )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_isAnimSel'              %( m_animNodeName, m_index ), m_isAnimSel )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_startFrame'             %( m_animNodeName, m_index ), m_startFrame )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_endFrame'               %( m_animNodeName, m_index ), m_endFrame )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_LCC'                    %( m_animNodeName, m_index ), m_localCoordCenter, type = 'string' )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_saveDir'                %( m_animNodeName, m_index ), m_saveDir,   type = 'string')
                    cmds.setAttr( '%s._201102_clips[%s]._201102_saveDirIsDefault'       %( m_animNodeName, m_index ), m_saveDirIsDefault )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_FPS'                    %( m_animNodeName, m_index ), m_FPS )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_epsilon'                %( m_animNodeName, m_index ), m_epsilon )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_epsilonIsDefault'       %( m_animNodeName, m_index ), m_epsilonIsDefault )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_isAdditive'             %( m_animNodeName, m_index ), m_isAdditive )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_baseAnimation'          %( m_animNodeName, m_index ), m_baseAnimation, type = 'string' )
                    cmds.setAttr( '%s._201102_clips[%s]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ), m_baseAnimationIsDefault ) 
        return True                               

    def getIndexes(self):
        """
        RETURN: tuple ( skins_count , [skins_indexes], clips_count, [clips_indexes] )
                ( 2, [1, 2], 3, [2, 5, 6] )
                ( 
                  2            - amount of 'skins'
                  [1 ,2]       - array of 'skins' indexes
                  3            - amount of 'clips'
                  [2, 5, 6]    - array of 'clips' indexes
                ) 
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_animNodeFn = OpenMaya.MFnDependencyNode(m_animNode) # create a function set 
            m_indexesSkins = OpenMaya.MIntArray() # Result: [1, 2]
            m_indexesClips = OpenMaya.MIntArray() # Result: [2, 5, 6]
            m_animNodeAttrSkins = m_animNodeFn.attribute('_201102_skins')             
            m_animNodeAttrClips = m_animNodeFn.attribute('_201102_clips')
            m_animNodePlugSkins = OpenMaya.MPlug(m_animNode, m_animNodeAttrSkins) # create a MPlug
            m_animNodePlugClips = OpenMaya.MPlug(m_animNode, m_animNodeAttrClips) # create a MPlug
            m_animNodePlugSkins.getExistingArrayAttributeIndices(m_indexesSkins)
            m_animNodePlugClips.getExistingArrayAttributeIndices(m_indexesClips) 
        return  ( m_indexesSkins.length(), m_indexesSkins, m_indexesClips.length(), m_indexesClips)

    def getConnectedAnimLayers( self ):
        """
        RETURN: tuple( m_obj_array, m_indexes )
                m_obj_array  - MObjectArray() of connected Animation layers stored in node,
                m_indexes    - MIntArray() of connected Animation layers stored in node
                None - if node don't exists or have an error          
        """
        m_node = OpenMaya.MObject()
        m_nodeFn = OpenMaya.MFnDependencyNode() # create a function set
        m_nodePlug = OpenMaya.MPlug()        
        m_indexes = OpenMaya.MIntArray()        
        m_obj_array = OpenMaya.MObjectArray() # array of MObjects                
        try:
            m_node = getMObjectFromNodeName( m_animNodeName )
            m_nodeFn.setObject( m_node )            # connect MObject with function set
            m_nodeAttr = m_nodeFn.attribute('_201102_animLayersMessage')
            m_nodePlug = OpenMaya.MPlug( m_node, m_nodeAttr )       
            m_nodePlug.getExistingArrayAttributeIndices( m_indexes )        
            for m_ind in m_indexes:
                m_plug_children_layers = m_nodePlug.elementByLogicalIndex( m_ind )    
                connectedPlugs = OpenMaya.MPlugArray()
                if (m_plug_children_layers.connectedTo(connectedPlugs, True, False)):      
                    m_plug_destination = connectedPlugs[0]
                    m_obj_array.append(m_plug_destination.node()) # append MObject to array
                else:
                    cmds.removeMultiInstance( '%s._201102_animLayersMessage[%d]' %( m_animNodeName, m_ind ) )
        except:
            return None
        m_output = ( m_obj_array, m_indexes )
        return m_output

    def isAnimLayerAlreadyConnected( self, m_obj ):
        """
        INPUT:  m_obj - MObject to the Animation layer node
        RETURN: True  - is Animation Layer Already connected to the node
                False - otherwise        
        """
        m_obj_tuple = self.getConnectedAnimLayers()
        if ( m_obj_tuple ):            
            m_obj_array = m_obj_tuple[ 0 ] 
            if ( m_obj_array ):
                for m_ind in range( m_obj_array.length() ):
                    try:
                        if ( m_obj == m_obj_array[ m_ind ] ):
                            return True
                    except:
                        pass
        return False

    def animLayerNodesConnect( self ):
        """
        try to connect existing Animation Layer nodes to 'xray_anim_lister' node
            source:      AnimLayerNode.message 
            destination: xray_anim_lister._201102_animLayersMessage[%d]        
        RETURN: True  - if connection done
                False - if have some errors       
        """
        m_anim_layers = m_animLayers() # class for Animation Layer nodes
        m_obj_array = m_anim_layers.get() # get all animation layers currently stored in maya file 
        for m_ind in range( m_obj_array.length() ):
            if ( not self.isAnimLayerAlreadyConnected( m_obj_array[ m_ind ] ) ):
                m_nodeFn = OpenMaya.MFnDependencyNode()
                m_nodePlug = OpenMaya.MPlug()
                m_node = getMObjectFromNodeName( m_animNodeName )
                try:                    
                    m_nodeFn.setObject( m_node ) # error - if node don't exists
                except:
                    self.errorHandle.raiseMessage("can't find %s" %m_animNodeName)
                    return False
                try:
                    m_nodeAttr = m_nodeFn.attribute( '_201102_animLayersMessage' )
                    m_nodePlug = OpenMaya.MPlug( m_node, m_nodeAttr )
                except:
                    self.errorHandle.raiseMessage("can't find %s._201102_animLayersMessage" %m_animNodeName )
                    return False
                m_indexes = OpenMaya.MIntArray()
                try:
                    m_nodePlug.getExistingArrayAttributeIndices( m_indexes )
                except:
                    self.errorHandle.raiseMessage("wrong type %s._201102_animLayersMessage should be 'message' array" %m_animNodeName )
                    return False                   
                if ( 0 == m_indexes.length() ):
                    m_nextAvailable = 0
                else:
                    m_nextAvailable = m_indexes[ m_indexes.length() - 1 ] + 1
                src = "%s.message" % getNodeNameFromeMObject( m_obj_array[ m_ind ] )
                dst = "%s._201102_animLayersMessage[%d]" %( m_animNodeName, m_nextAvailable )
                #print ("connect %s to %s " %( src, dst ) )
                try:
                    cmds.connectAttr( src, dst )
                except:
                    self.errorHandle.raiseMessage("can't connect attr %s to %s " %( src, dst )  )
                    return False           
        return True
        
    def animLayerNodesSynchronize( self ):
        """
        try to synchronize array of indexes 
            source:      xray_anim_lister._201102_animLayersMessage[%d] 
            destination: xray_anim_lister._201102_clips[%d]._201102_animLayers[%d]
        RETURN: True  - if synchronization done
                False - if have some errors   
        """
        if ( not self.animLayerNodesConnect() ):
            return False
        m_node = OpenMaya.MObject()
        m_nodeFn = OpenMaya.MFnDependencyNode() 
        m_nodePlug = OpenMaya.MPlug()        
        m_indexes = OpenMaya.MIntArray()
        m_indexesClips = OpenMaya.MIntArray()                
        try:
            m_node = getMObjectFromNodeName( m_animNodeName )
            m_nodeFn.setObject( m_node )            # connect MObject with function set
        except:
            self.errorHandle.raiseMessage("can't find %s" %m_animNodeName)
            return False
        try:            
            m_nodeAttr = m_nodeFn.attribute('_201102_animLayersMessage')
            m_nodePlug = OpenMaya.MPlug( m_node, m_nodeAttr )
            m_nodePlug.getExistingArrayAttributeIndices( m_indexes )
        except:
            self.errorHandle.raiseMessage("can't find %s._201102_animLayersMessage or attr has wrong type (should be array of 'message')" %m_animNodeName )
            return False                
        try:
            m_animNodeAttrClips = m_nodeFn.attribute( '_201102_clips' )
            m_animNodePlugClips = OpenMaya.MPlug( m_node, m_animNodeAttrClips )
            m_animNodePlugClips.getExistingArrayAttributeIndices( m_indexesClips )
        except:
            self.errorHandle.raiseMessage("can't find %s._201102_clips or attr has wrong type" %m_animNodeName )
            return False
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
        for m_indAL in m_indexesClips:
            m_indexesAnimLayers = OpenMaya.MIntArray() # indexes of _201102_animLayers attribute for current _201102_clips item
            # child(14) = 14 is the index of _201102_animLayers attribute
            m_animNodePlugClips.elementByLogicalIndex(m_indAL).child(14).getExistingArrayAttributeIndices( m_indexesAnimLayers )
            # print "%d %s needed %s" %( m_indAL, m_indexesAnimLayers, m_indexes )
            # iterate trought all indexes
            if ( 0 != m_indexes.length() ):
                for m_ind in range( m_indexes[ m_indexes.length() - 1 ] + 1 ):
                    m_a = findMIntArray( m_indexes, m_ind ) # source xray_anim_lister._201102_animLayersMessage[%d] 
                    m_b = findMIntArray( m_indexesAnimLayers, m_ind )  # destination xray_anim_lister._201102_clips[%d]._201102_animLayers[%d]             
                    if   ( m_a and m_b ):
                        pass
                        #print "Ok"
                    elif ( m_a and not m_b ):
                        cmds.getAttr( '%s._201102_clips[%d]._201102_animLayers[%d]' %( m_animNodeName, m_indAL, m_ind ) )
                        #print "Need create index %d" %m_ind
                    elif ( not m_a and m_b ):
                        self.unlockAllItems()
                        cmds.removeMultiInstance( '%s._201102_clips[%d]._201102_animLayers[%d]' %( m_animNodeName, m_indAL, m_ind ) ) 
                        self.lockAllItems()
                        #print "Need delete index %d" %m_ind
                # ------------------------------------------------------------
                if ( 0 == OpenMaya.MGlobal_mayaState() ):     # INCREASE PROGRESS STEP
                    cmds.progressBar(gMainProgressBar, edit=True, step = 100/m_indexesClips.length() )
        # ------------------------------------------------------------ 
        if ( 0 == OpenMaya.MGlobal_mayaState() ):  
            cmds.progressBar(gMainProgressBar, edit = True, endProgress = True) # END PROGRESS
        return True

    def animLayerNodesFindIndex( self, m_find_node_name ):
        """
        INPUT:  m_find_node_name - string, name of the node ( example: 'AnimLayer1' ) 
        RETURN: tuple ( index1, index2 )
                index1 - calculation index
                index2 - int, index of xray_anim_lister._201102_animLayersMessage[%d], if node found by string name
                None   - if have some errors   
        """ 
        m_obj_tuple = self.getConnectedAnimLayers()
        if ( m_obj_tuple ):
            for m_ind in range( m_obj_tuple[1].length() ):
                #print "%s %s" %( m_obj_tuple[1][m_ind], getNodeNameFromeMObject( m_obj_tuple[0][m_ind] ) )
                if ( m_find_node_name == getNodeNameFromeMObject( m_obj_tuple[0][m_ind] ) ):
                    return ( m_ind, m_obj_tuple[1][m_ind] )
        return None                           
        
    def alreadyExistsSkin( self, m_skinName ):
        """
        check if m_skinName already exists in dictionary of the node
        INPUT:  m_skinName - object name, like '|group1|stalker' 
        RETURN: True if object exits in dictionary, False otherwise
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count , [skins_indexes], clips_count, [clips_indexes] ) 
            for m_index in m_indexes[1]:
                if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')):
                    return True
        return False   
                       
    def addSkin(self, m_skinName, m_isSkinSel = False):
        """
        append item to the multi attribute 'skins'
        INPUT:  m_skinName    - skin name       
                m_isSkinSel   - set to True if skin in list selected

        STORED: m_obj         - '|group1|stalker' 
                m_isSkinSel   - False
        RETURN: True if skin was added properly, False otherwise
        """
        m_animNode = self.getNode() # MObject
        if (self.alreadyExistsSkin(m_skinName)):    # check if skin alreadyExists
            return False
        else:
            if (bool(m_animNode)):
                # Find the next available index
                m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                              #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
                if (0 == m_indexes[0]):
                    m_index = 0
                else:
                    m_index = m_indexes[1][m_indexes[0]-1] + 1           
                                                      #--------------------------------
                cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']', type = True) # Create a plug by accessing iter
                                                      #---------------------------------
                # ----------- unlcok --------------
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = False)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = False)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      lock = False)
                # ------------- set ---------------
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  m_skinName,  type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      bool(m_isSkinSel))         
                # ------------ lock ---------------
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = True)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = True)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      lock = True)           
                return True
        return False

    def addClip(self, m_skinName):
        """
        append item to the multi attribute 'clips'
        
        INPUT:  m_skinName                 - skin name
                m_animName                 - name of animation clip
                m_isAnimSel                - set to True if clip selected in list
                m_startFrame               - start frame for animation
                m_endFrame                 - end frame for animation
                m_localCoordCenter         - for cat-scene animations with locked offsets (can be any kTransform)
                m_saveDir                  - location of exported animations
                m_saveDirIsDefault         - is m_saveDir has Default value
                m_FPS                      - current OpenMaya.MTime_uiUnit() (example: NTSC 30 fps)
                m_epsilon                  - compression quality (0.001 - best quality, 0.1 - low quality)
                m_epsilonIsDefault         - is m_epsilon has Default value
                m_isAdditive               - is animation Additive to the base animation
                m_baseAnimation            - location of Base Animation
                m_baseAnimationIsDefault   - is m_baseAnimation has Default value

        STORED: m_skinName                 - '|group1|stalker' 
                m_animName                 - 'run_turn_left'
                m_isAnimSel                - False
                m_startFrame               - 0
                m_endFrame                 - 24
                m_localCoordCenter         - 'pCube1'
                m_saveDir                  - 'XRAY_RESOURCES_PATH/animations/actors/stalker/run'
                m_FPS                      - 8
                m_FPSisDefault             - True
                m_epsilon                  - 0.005
                m_epsilonIsDefault         - True
                m_isAdditive               - False
                m_baseAnimation            - 'XRAY_RESOURCES_PATH/animations/default.clip'
                m_baseAnimationIsDefault   - True
                m_animLayers               - [ True, False, ... ]
                                
        RETURN: True if Animation Clip was added properly, False otherwise
        """       
        m_animName                 = ''
        m_isAnimSel                = False
        m_startFrame               = float(cmds.playbackOptions( query = True, min = True ))
        m_endFrame                 = float(cmds.playbackOptions( query = True, max = True )) 
        m_localCoordCenter         = ''
        m_saveDir                  = m_anim_saveDirDefault
        m_saveDirIsDefault         = True
        m_FPS                      = OpenMaya.MTime_uiUnit()
        m_epsilon                  = m_anim_epsilonDefault
        m_epsilonIsDefault         = True
        m_isAdditive               = False
        m_baseAnimation            = m_anim_baseAnimationDefault
        m_baseAnimationIsDefault   = True                  
        # -----------------------------------------        
        m_animNode = self.getNode() # MObject        
        if (bool(m_animNode)):
            if (self.alreadyExistsSkin(m_skinName)):    # check if skin Exists
                m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                              #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
                if (0 == m_indexes[2]):
                    m_index = 0
                else:                
                    m_index = m_indexes[3][m_indexes[2]-1] + 1           
                                                      #--------------------------------
                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']', type = True) # Create a plug by accessing iter
                                                      #---------------------------------
                # ----------- unlcok --------------
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',          lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',               lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',              lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',             lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',               lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                    lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',       lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                    lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',       lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = False)
                # ------------- set ---------------
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',    m_skinName,  type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',         m_animName,  type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',        bool(m_isAnimSel))
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',       m_startFrame)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',         m_endFrame)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',              m_localCoordCenter,  type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',          m_saveDir,   type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault', bool(m_saveDirIsDefault))
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',              m_FPS)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',          m_epsilon)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault', bool(m_epsilonIsDefault))
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',               bool(m_isAdditive))
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',            m_baseAnimation,   type = 'string')
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault',   bool(m_baseAnimationIsDefault))                   
                # ------------ lock ---------------
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',      lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',           lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',          lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',         lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',           lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',   lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',   lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = True)
                return True
        return False
        
    def delClip(self, m_skinName, m_clipID):
        """
        delete Animation Clip from the node based on m_clipID
        INPUT:  m_skinName - string, example '|group4|stalker'
                m_clipID   - integer, example: 4
        RETURN: True if Animation Clip was deleted from the node, False otherwise        
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            if (self.alreadyExistsSkin(m_skinName)):    # check if skin Exists
                m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                              #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
                for m_index in m_indexes[3]:
                    if (m_index == m_clipID):
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',          lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',               lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',              lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',             lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',               lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                    lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',       lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                    lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',       lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = False)     
                        # ----------- delete --------------
                        cmds.removeMultiInstance(m_animNodeName + '._201102_clips[' + str(m_index) + ']')                                                                  
                        return True
        return False

    def delSkin(self, m_skinName):
        """
        delete item from multi attribute 'skins' stored in the node AND
        delete All Animation Clip from the node based on m_skinName        
        INPUT:  m_skinName - string, example '|group4|stalker'
        RETURN: True if Object was deleted from the node (from 'skins','clips' attributes),
                False otherwise        
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            if (self.alreadyExistsSkin(m_skinName)):    # check if skin Exists            
                m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                              #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
                # ---------------------------------------------------------------------------------------------                 
                #  delete all animation clips (from multi attr 'clips') associated with input skin m_skinName
                # ---------------------------------------------------------------------------------------------
                for m_index in m_indexes[3]:
                    if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips')):
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',          lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',               lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',              lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',             lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',               lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                    lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',       lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                    lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',                lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',       lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = False)
                        # ----------- delete --------------
                        cmds.removeMultiInstance(m_animNodeName + '._201102_clips[' + str(m_index) + ']')                
                # --------------------------------------------------------------------------------                 
                #       delete m_skinName from multi attr 'skins'
                # --------------------------------------------------------------------------------
                for m_index in m_indexes[1]:
                    if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')):
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      lock = False)
                        # ----------- delete --------------
                        cmds.removeMultiInstance(m_animNodeName + '._201102_skins[' + str(m_index) + ']') 
                return True                       
        return False

    def printAnims(self):
        """
        read node and print the content of the dictionary, for debug purpose only
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            for m_index in m_indexes[1]:
                self.errorHandle.raiseMessage("SKIN: %s <==> SEL: %s" 
                            % ( cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins'),
                                cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel')))
            for m_index in m_indexes[3]:
                self.errorHandle.raiseMessage("SKIN: %s <==> ANIM: %s SEL: %s MIN: %s MAX: %s LCC: %s DIR: %s %s FPS: %s EPS: %s %s %s %s %s"
                            % ( cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation'),
                                cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault')))

    def lockAllItems(self):
        """
        lock all items in the node
        """
        m_animNode = OpenMaya.MObject()
        m_animNode = getMObjectFromNodeName( m_animNodeName )        
	print m_animNodeName
        if ( m_animNode ):
            m_animNodeFn = OpenMaya.MFnDependencyNode(m_animNode) # create a function set 
            m_indexesSkins = OpenMaya.MIntArray() # Result: [1, 2]
            m_indexesClips = OpenMaya.MIntArray() # Result: [2, 5, 6]
            m_animNodeAttrSkins = m_animNodeFn.attribute('_201102_skins')             
            m_animNodeAttrClips = m_animNodeFn.attribute('_201102_clips')
            m_animNodePlugSkins = OpenMaya.MPlug(m_animNode, m_animNodeAttrSkins) # create a MPlug
            m_animNodePlugClips = OpenMaya.MPlug(m_animNode, m_animNodeAttrClips) # create a MPlug
            m_animNodePlugSkins.getExistingArrayAttributeIndices(m_indexesSkins)
            m_animNodePlugClips.getExistingArrayAttributeIndices(m_indexesClips) 
            for m_index in m_indexesSkins:
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = True)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = True)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      lock = True)                                                                   
            for m_index in m_indexesClips:
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',      lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',           lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',          lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',         lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',           lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',   lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',            lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',   lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = True)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = True)

    def unlockAllItems(self):
        """
        unlock all items in the node
        """
        m_animNode = OpenMaya.MObject()
        m_animNode = getMObjectFromNodeName( m_animNodeName )        
        if ( m_animNode ):
            m_animNodeFn = OpenMaya.MFnDependencyNode(m_animNode) # create a function set 
            m_indexesSkins = OpenMaya.MIntArray() # Result: [1, 2]
            m_indexesClips = OpenMaya.MIntArray() # Result: [2, 5, 6]
            m_animNodeAttrSkins = m_animNodeFn.attribute('_201102_skins')             
            m_animNodeAttrClips = m_animNodeFn.attribute('_201102_clips')
            m_animNodePlugSkins = OpenMaya.MPlug(m_animNode, m_animNodeAttrSkins) # create a MPlug
            m_animNodePlugClips = OpenMaya.MPlug(m_animNode, m_animNodeAttrClips) # create a MPlug
            m_animNodePlugSkins.getExistingArrayAttributeIndices(m_indexesSkins)
            m_animNodePlugClips.getExistingArrayAttributeIndices(m_indexesClips) 
            for m_index in m_indexesSkins:
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = False)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = False)
                cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',      lock = False)
            for m_index in m_indexesClips:
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',          lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',               lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',              lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',             lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',               lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',                    lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',       lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',                    lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',                lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',       lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAdditive',             lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimation',          lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_baseAnimationIsDefault', lock = False)
                cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animLayers',             lock = False)
                
    def setSkinSelectionState(self, m_skinName, m_isSkinSel = False):
        """
        set to True if object in list selected
        INPUT:  m_skinName    - key value from list self.obj (example: '|group1|stalker')
                m_isSkinSel   - boolean value, True - selected, False - unselected 
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setSkinSelectionState('|group1|stalker', False)        
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[0]):
                return False
            for m_index in m_indexes[1]:
                if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                    lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',  lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',  bool(m_isSkinSel)) 
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                    lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel',  lock = True)
                    return True
        return False
        
    def setClipSelectionState(self, m_clipID, m_isAnimSel = False):
        """
        set to True if object in list selected
        INPUT:  m_clipID      - int value (example: 3), animation id
                m_isAnimSel   - boolean value, True - selected, False - unselected 
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setClipSelectionState(4, False)        
        """                    
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                        lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',      lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',      bool(m_isAnimSel))                    
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                        lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel',      lock = True)
                    return True
        return False
        
    def setClipIsAdditive( self, m_clipID, m_isAdditive ):
        """
        set animation is Additive
        INPUT:  m_clipID      - int value (example: 3), animation id
                m_isAdditive   - boolean value, True - selected, False - unselected 
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setClipIsAdditive( 4, False )        
        """ 
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr( '%s._201102_clips[%d]'                    %( m_animNodeName, m_index ), lock = False )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_isAdditive' %( m_animNodeName, m_index ), lock = False )
                    # ------------- set ---------------
                    cmds.setAttr( '%s._201102_clips[%d]._201102_isAdditive' %( m_animNodeName, m_index ), bool( m_isAdditive ) )
                    # ------------ lock ---------------
                    cmds.setAttr( '%s._201102_clips[%d]'                    %( m_animNodeName, m_index ), lock = True )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_isAdditive' %( m_animNodeName, m_index ), lock = True )
                    return True
        return False 
        
    def setClipBaseAnimation( self, m_clipID, m_baseAnimation = m_xrayResourcesPathAnims ):
        """
        set BaseAnimation for current Additive animation clip
        INPUT:  m_clipID         - int value (example: 3), animation id
                m_baseAnimation  - string value, path to file stored in HDD with already exported BaseAnimation
        RETURN: True if file changed, False otherwise
        USAGE:  my_node.setClipBaseAnimation( 4, 'XRAY_RESOURCES_PATH/animations/actors/BaseRun.clip' )       
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr( '%s._201102_clips[%d]'                                %( m_animNodeName, m_index ), lock = False )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimation'          %( m_animNodeName, m_index ), lock = False )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ), lock = False )
                    # ------------- set ---------------
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimation'          %( m_animNodeName, m_index ), m_baseAnimation, type = 'string' )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ), False )
                    # ------------ lock ---------------
                    cmds.setAttr( '%s._201102_clips[%d]'                                %( m_animNodeName, m_index ), lock = True )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimation'          %( m_animNodeName, m_index ), lock = True )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ), lock = True )
                    return True
        return False

    def setClipAnimLayers( self, m_clipID, m_checkboxStates ):
        """
        set selection states of AnimLayers for current animation clip
        INPUT:  m_clipID          - int value (example: 3), animation id
                m_checkboxStates  - hash, like this:
                                    {u'AnimLayer4': False, u'AnimLayer5': False, u'AnimLayer1': True}
        RETURN: True if data changed, False otherwise     
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr( '%s._201102_clips[%d]'                             %( m_animNodeName, m_index ), lock = False )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers'          %( m_animNodeName, m_index ), lock = False )
                    # ------------- set ---------------
                    for obj, data in m_checkboxStates.iteritems():
                        m_current_anim_layer_index = self.animLayerNodesFindIndex( obj )
                        m_ind = m_current_anim_layer_index[1]
                        cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers[%d]'      %( m_animNodeName, m_index, m_ind ), lock = False )                        
                        cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers[%d]'      %( m_animNodeName, m_index, m_ind ), data )                                        
                    # ------------ lock ---------------
                    cmds.setAttr( '%s._201102_clips[%d]'                             %( m_animNodeName, m_index ), lock = True )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers'          %( m_animNodeName, m_index ), lock = True )
                    return True
        return False                                               

    def setClipName(self, m_clipID, m_animName = ''):
        """
        set name of animation clip
        INPUT:  m_clipID      - int value (example: 3), animation id
                m_animName    - name of animation
        RETURN: True if animation name changed, False otherwise
        USAGE:  my_node.setClipName(4, 'idle')       
        """
        m_animName = makeName( m_animName, 'filename' )      # format string for filename
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                     lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',    lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',    m_animName,  type = 'string')                
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                     lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName',    lock = True)
                    return True
        return False

    def setClipStartFrame(self, m_clipID, m_startFrame = 0):
        """
        set Start Frame for animation clip
        INPUT:  m_clipID      - int value (example: 3), animation id
                m_startFrame  - int value
        RETURN: True if Start Frame changed, False otherwise
        USAGE:  my_node.setClipStartFrame( 4, 0 )               
        """            
        try:
            m_startFrame = float(m_startFrame)
        except:
            m_startFrame = float(cmds.playbackOptions( query = True, min = True ))
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    m_endFrame = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame')
                    if (m_startFrame > m_endFrame):
                        m_startFrame = m_endFrame
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                       lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',    lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',    m_startFrame)             
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                       lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',    lock = True)
                    return True
        return False 

    def setClipEndFrame(self, m_clipID, m_endFrame = 100):
        """
        set End Frame for animation clip
        INPUT:  m_clipID      - int value (example: 3), animation id
                m_endFrame    - int value
        RETURN: True if End Frame changed, False otherwise
        USAGE:  my_node.setClipEndFrame( 4, 24 )               
        """            
        try:
            m_endFrame = float(m_endFrame)
        except:
            m_endFrame = float(cmds.playbackOptions( query = True, max = True ))
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    m_startFrame = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame')
                    if (m_endFrame < m_startFrame):
                        m_endFrame = m_startFrame
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                     lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',    lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',    m_endFrame)             
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                     lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',    lock = True)
                    return True
        return False

    def setClipLocalCoordCenter(self, m_clipID, m_localCoordCenter = ''):
        """
        set Local Coordinate Center for animation clip
        INPUT:  m_clipID              - int value (example: 3), animation id
                m_localCoordCenter    - string (example: 'pCube1'), any kTransform
        RETURN: True if LCC changed, False otherwise
        USAGE:  my_node.setClipLocalCoordCenter(4, 'pCube1')       
        """
        #m_localCoordCenter = makeName( m_localCoordCenter, 'nodename' )      # format string for nodename
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',    lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',    m_localCoordCenter,  type = 'string')           
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC',    lock = True)
                    return True
        return False
        
    def setClipSaveDir(self, m_clipID, m_saveDir = m_xrayResourcesPathAnims):
        """
        set Save Directory for animation clip
        INPUT:  m_clipID     - int value (example: 3), animation id
                m_saveDir    - string value, path to folder for save current animation
        RETURN: True if Save Directory changed, False otherwise
        USAGE:  my_node.setClipSaveDir(4, 'XRAY_RESOURCES_PATH/animations/actors/stalker/run')       
        """
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',            lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',   lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',            m_saveDir,  type = 'string')
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',   False)           
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir',            lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault',   lock = True)
                    return True
        return False
    
    def setClipFPS(self, m_clipID, m_FPS):
        """
        set Frame Range for current clip
        INPUT:  m_clipID     - int value (example: 3), animation id
                m_FPS        - int value (OpenMaya.MTime.kNTSCFrame == 8)
        RETURN: True if Clip Frame Range changed, False otherwise
        USAGE:  my_node.setClipFPS(4, 8)        
        """
        m_FPSOriginal = OpenMaya.MTime_uiUnit()
        m_return = False
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                m_return = False
            else:    
                for m_index in m_indexes[3]:
                    if (m_index == m_clipID):
                        m_FPSOld        = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS')
                        m_startFrameOld = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame')
                        m_endFrameOld   = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame') 
                        OpenMaya.MTime.setUIUnit(m_FPSOld)
                        m_startFrame = m_startFrameOld*OpenMaya.MTime.asUnits(OpenMaya.MTime(), m_FPS)
                        m_endFrame   = m_endFrameOld*OpenMaya.MTime.asUnits(OpenMaya.MTime(), m_FPS)
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                     lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',         lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',  lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',    lock = False)
                        # ------------- set ---------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',          m_FPS)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',   m_startFrame)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',     m_endFrame)
                        # ------------ lock ---------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                      lock = True)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS',          lock = True)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame',   lock = True)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame',     lock = True)
                        m_return = True      
        OpenMaya.MTime.setUIUnit(m_FPSOriginal) 
        return m_return
    
    def setClipEpsilon(self, m_clipID, m_epsilon):
        """
        set Epsilon for exporting animations
        INPUT:  m_clipID     - int value (example: 3), animation id
                m_epsilon    - float value (example: 0.005)
        RETURN: True if Epsilon changed, False otherwise
        USAGE:  my_node.setClipEpsilon(4, 0.1)             
        """
        if (m_epsilon > m_anim_epsilonMax):
            m_epsilon = m_anim_epsilonMax
        if (m_epsilon < m_anim_epsilonMin):
            m_epsilon = m_anim_epsilonMin  
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            if (0 == m_indexes[2]):
                return False        
            for m_index in m_indexes[3]:
                if (m_index == m_clipID):
                    # ----------- unlcok --------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',            lock = False)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',   lock = False)
                    # ------------- set ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',            m_epsilon)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',   False)           
                    # ------------ lock ---------------
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                            lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon',            lock = True)
                    cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault',   lock = True)
                    return True
        return False

    def setSkinName(self, m_skinName, m_skinNameNew):
        """
        set Skin Name for attr 'skins', 'clips'
        INPUT:  m_skinName        - key value (example: '|group4|stalker')
                m_skinNameNew     - like m_obj
        RETURN: True if skin name changed, False otherwise
        USAGE:  my_node.setSkinName('|group4', '|group4|group3')        
        """
        m_animNode = self.getNode() # MObject
        m_skinNameNew = makeName(m_skinNameNew, 'nodename')
        if (self.alreadyExistsSkin(m_skinNameNew)):    # check if new skin alreadyExists
            return False
        if (bool(m_animNode)):
            if (self.alreadyExistsSkin(m_skinName)):    # check if old skin Exists
                m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                              #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3] 
                # ---------------------------------------------------------------------------------------------                 
                #      rename first all from attr 'clips'
                # ---------------------------------------------------------------------------------------------
                for m_index in m_indexes[3]:
                    if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips')):
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                        lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',  lock = False)
                        # ----------- rename --------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',  m_skinNameNew,  type = 'string') 
                        # ----------- lock ----------------
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']',                        lock = True)
                        cmds.setAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips',  lock = True)
                # --------------------------------------------------------------------------------                 
                #       rename in attr 'skins'
                # --------------------------------------------------------------------------------
                for m_index in m_indexes[1]:
                    if (m_skinName == cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')):
                        # ----------- unlcok --------------
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = False)
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = False)
                        # ----------- rename --------------
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  m_skinNameNew,  type = 'string')
                        # ----------- lcok ----------------
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']',                        lock = True)
                        cmds.setAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins',  lock = True)
                return True
        return False                                                                                                                  

    def iterItemsSkin(self):
        """
        you can iterate in program using this method
        RETURN: hash, all data stored in the node (attr 'skins')
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
        m_maxLenght = self.getMaxLenListSkin() # [ maxLen(getShortName(m_skinName)), maxLen(m_skinName) ] 
        m_animNode  = self.getNode() # MObject 
        if (bool(m_animNode)):        
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            for m_index in m_indexes[1]:
                m_skinName     = cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')
                m_isSkinSel    = cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_isSkinSel') 
                # -----------------------------                
                m_list = []
                m_list.append(addSpacesToString(getShortName(m_skinName), m_maxLenght[0] - len(getShortName(m_skinName))))  # stalker
                m_list.append(addSpacesToString(m_skinName,               m_maxLenght[1] - len(m_skinName)))                # |group1|stalker                    
                # 'stalker    |group1|stalker'            
                # -------------------------------                 
                m_returnHash[m_skinName] = (bool(m_isSkinSel), string.join(m_list,'    '))
                # ------------------------------------------------------------
                if ( 0 == OpenMaya.MGlobal_mayaState() ):     # INCREASE PROGRESS STEP               
                    cmds.progressBar(gMainProgressBar, edit=True, step = 100/m_indexes[0] )
        # ------------------------------------------------------------ 
        if ( 0 == OpenMaya.MGlobal_mayaState() ):  
            cmds.progressBar(gMainProgressBar, edit = True, endProgress = True) # END PROGRESS
        return m_returnHash

    def iterItemsClip(self, m_stat = 'all', m_selSkinName = None):
        """
        you can iterate in program using this method
        INPUT:  m_stat         - 'all' - return all stored clips
                                 'sel' - return clips for selected mesh
                m_selSkinName  - '|group1|stalker'
        RETURN: hash, all data stored in the node (attr 'clips')
                 
                 obj      - 3                     - m_clipID
                 data[0]  - '|group1|stalker'     - m_skinName 
                 data[1]  - False                 - m_isAnimSel
                 data[2]  - 'run_turn_left'       - m_animName
                 data[3]  - 0                     - m_startFrame
                 data[4]  - 24                    - m_endFrame 
                 data[5]  - 'pCube1'              - m_localCoordCenter 
                 data[6]  - '/actors/stalker/run' - m_saveDir
                 data[7]  - '3    run_turn_left    0    24    /actors/stalker/run    pCube1    NTSC (30 fps) 0.005'
                 data[8]  - False                 - m_saveDirIsDefault 
                 data[9]  - True                  - m_epsilonIsDefault
                 data[10] - 8                     - m_FPS (OpenMaya.MTime.kNTSCFrame)
                 data[11] - 0.005                 - m_epsilon
                 data[12] - False                 - m_isAdditive 
                 data[13] - '/default'            - m_baseAnimation
                 data[14] - True                  - m_baseAnimationIsDefault  
                 data[15] - [ True, False, ... ]  - m_animLayers  
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
        m_animNode  = self.getNode() # MObject
        m_maxLenght = self.getMaxLenListClip()         
        if ( None != m_selSkinName ):
            if ( not self.alreadyExistsSkin( m_selSkinName ) ):
                m_selSkinName = None
        if (bool(m_animNode)):        
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3] 
            for m_index in m_indexes[3]:
                m_skinName         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips')
                m_animName         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName')    
                m_isAnimSel        = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel')
                m_startFrame       = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame')
                m_endFrame         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame')
                m_localCoordCenter = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC')
                m_saveDirIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault')
                m_epsilonIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault')
                m_FPS              = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS')           
                if ( m_saveDirIsDefault ):
                    m_saveDir      = m_anim_saveDirDefault
                else:
                    m_saveDir      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir')
                if ( m_epsilonIsDefault ):
                    m_epsilon      = m_anim_epsilonDefault
                else:
                    m_epsilon      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon')
                m_FPSStr           = m_FPSlist[m_FPS  - 1]    
                m_startFrameStr    = str( '%.2f' %( m_startFrame ) )
                m_endFrameStr      = str( '%.2f' %( m_endFrame   ) )
                m_epsilonStr       = str( '%.3f' %( m_epsilon    ) )
                m_clipID           = m_index
                # -------------------------------
                m_isAdditive             = cmds.getAttr( '%s._201102_clips[%d]._201102_isAdditive'             %( m_animNodeName, m_index ) )
                m_baseAnimationIsDefault = cmds.getAttr( '%s._201102_clips[%d]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ) )                
                if ( m_baseAnimationIsDefault ):
                    m_baseAnimation      = m_anim_baseAnimationDefault
                else:
                    m_baseAnimation      = cmds.getAttr( '%s._201102_clips[%d]._201102_baseAnimation'          %( m_animNodeName, m_index ) )
                m_animLayers             = cmds.getAttr( '%s._201102_clips[%d]._201102_animLayers'             %( m_animNodeName, m_index ) )
                # m_xrayResourcesPathAnims = 'XRAY_RESOURCES_PATH/animations'
                m_saveDir             = m_saveDir.replace( 'XRAY_RESOURCES_PATH', '' , 1)
                m_baseAnimation       = m_baseAnimation.replace( 'XRAY_RESOURCES_PATH', '' , 1 )
                m_animFilename        = "%s/%s.clip" %( m_saveDir, m_animName )
                m_baseAnimFilename    = "%s.clip" %m_baseAnimation
                # -------------------------------------------------------------
                m_list = []
                m_list.append(addSpacesToString(str(m_clipID),            m_maxLenght[7] - len(str(m_clipID))))             # 3
                m_list.append(addSpacesToString(m_animName,               m_maxLenght[2] - len(m_animName)))                # run_turn_left
                m_list.append(addSpacesToString(m_startFrameStr,          m_maxLenght[3] - len(m_startFrameStr)))           # 0
                m_list.append(addSpacesToString(m_endFrameStr,            m_maxLenght[4] - len(m_endFrameStr)))             # 24
                m_list.append(addSpacesToString(m_animFilename,           m_maxLenght[6] - len(m_animFilename)))            # /animations/actors/stalker/run
                m_list.append(addSpacesToString(m_localCoordCenter,       m_maxLenght[5] - len(m_localCoordCenter)))        # pCube1
                m_list.append(addSpacesToString(m_FPSStr,                 m_maxLenght[0] - len(m_FPSStr)))                  # NTSC (30 fps)
                m_list.append(addSpacesToString(m_epsilonStr,             m_maxLenght[1] - len(m_epsilonStr)))              # 0.005
                if ( m_isAdditive ):
                    m_list.append( 'ADD to %s' %m_baseAnimFilename )
                # '3    run_turn_left    0    24    /animations/actors/stalker/run    pCube1    NTSC (30 fps) 0.005    Additive for /default.clip'
                # ------------------------------
                if   ('all' == m_stat):
                    m_returnHash[m_clipID]     = (m_skinName, bool(m_isAnimSel), m_animName, m_startFrame, m_endFrame, 
                                                  m_localCoordCenter, m_saveDir, string.join(m_list,'    '),
                                                  m_saveDirIsDefault, m_epsilonIsDefault, m_FPS, m_epsilon,
                                                  m_isAdditive, m_baseAnimFilename, m_baseAnimationIsDefault, m_animLayers )
                elif ('sel' == m_stat):
                    if (None != m_selSkinName and m_skinName == m_selSkinName):
                        m_returnHash[m_clipID] = (m_skinName, bool(m_isAnimSel), m_animName, m_startFrame, m_endFrame,
                                                  m_localCoordCenter, m_saveDir, string.join(m_list,'    '),
                                                  m_saveDirIsDefault, m_epsilonIsDefault, m_FPS, m_epsilon,
                                                  m_isAdditive, m_baseAnimFilename, m_baseAnimationIsDefault, m_animLayers )
                # ------------------------------------------------------------
                if ( 0 == OpenMaya.MGlobal_mayaState() ):     # INCREASE PROGRESS STEP               
                    cmds.progressBar(gMainProgressBar, edit=True, step = 100/m_indexes[2])        
        # ------------------------------------------------------------ 
        if ( 0 == OpenMaya.MGlobal_mayaState() ):  
            cmds.progressBar(gMainProgressBar, edit = True, endProgress = True) # END PROGRESS             
        return m_returnHash               

    def getMaxLenListSkin(self):
        """
        RETURN: [ maxLen(getShortName(m_skinName)), 
                  maxLen(m_skinName) ]
        """
        m_maxLenght =  [0,0]
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):        
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            for m_index in m_indexes[1]:
                m_skinName     = cmds.getAttr(m_animNodeName + '._201102_skins[' + str(m_index) + ']._201102_skinNameSkins')
                # find maxLen strings
                if (m_maxLenght[0] < len(getShortName(m_skinName))):
                    m_maxLenght[0] = len(getShortName(m_skinName))
                if (m_maxLenght[1] < len(m_skinName)):
                    m_maxLenght[1] = len(m_skinName)
            # -------------------------------------------------
        return m_maxLenght

    def getMaxLenListClip(self):
        """
        RETURN: [ maxLen(m_FPS), 
                  maxLen(m_epsilon),
                  maxLen(m_animName),
                  maxLen(m_startFrame),
                  maxLen(m_endFrame),
                  maxLen(m_localCoordCenter),
                  maxLen(m_saveDir), 
                  maxLen(m_clipID) ]
        """
        m_maxLenght =  [ 0, 0, 0, 0, 0, 0, 0, 0 ]
        m_animNode = self.getNode() # MObject
        if (bool(m_animNode)):        
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3]
            for m_index in m_indexes[3]:
                m_FPS              = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS')
                m_animName         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName')
                m_startFrame       = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame')
                m_endFrame         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame')
                m_localCoordCenter = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC')
                m_saveDirIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault')
                m_epsilonIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault')
                if (m_saveDirIsDefault):
                    m_saveDir      = m_anim_saveDirDefault
                else:
                    m_saveDir      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir')
                if (m_epsilonIsDefault):
                    m_epsilon      = m_anim_epsilonDefault
                else:
                    m_epsilon      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon')
                m_FPSStr           = m_FPSlist[m_FPS  - 1]
                m_startFrameStr    = str( '%.2f'%(m_startFrame) )
                m_endFrameStr      = str( '%.2f'%(m_endFrame) )
                m_epsilonStr       = str( '%.3f'%(m_epsilon) )
                m_clipID           = str(m_index)
                # m_xrayResourcesPathAnims = 'XRAY_RESOURCES_PATH'
                m_saveDir             = m_saveDir.replace( 'XRAY_RESOURCES_PATH', '' , 1)
                m_animFilename     = "%s/%s.clip" %( m_saveDir, m_animName )
                # find maxLen strings
                if (m_maxLenght[0] < len(m_FPSStr)):
                    m_maxLenght[0] = len(m_FPSStr)
                if (m_maxLenght[1] < len(m_epsilonStr)):
                    m_maxLenght[1] = len(m_epsilonStr)
                if (m_maxLenght[2] < len(m_animName)):
                    m_maxLenght[2] = len(m_animName)
                if (m_maxLenght[3] < len(m_startFrameStr)):
                    m_maxLenght[3] = len(m_startFrameStr)
                if (m_maxLenght[4] < len(m_endFrameStr)):
                    m_maxLenght[4] = len(m_endFrameStr)
                if (m_maxLenght[5] < len(m_localCoordCenter)):
                    m_maxLenght[5] = len(m_localCoordCenter)
                if (m_maxLenght[6] < len(m_animFilename)):
                    m_maxLenght[6] = len(m_animFilename)
                if (m_maxLenght[7] < len(m_clipID)):
                    m_maxLenght[7] = len(m_clipID)
            # -------------------------------------------------
        return m_maxLenght

    def exportDataALL( self ):
        animLayerArrayNormalize()
        self.exportData()

    def exportDataIgnoreADD( self ):
        animLayerArrayNormalize()
        self.exportData( False, None, True )

    def exportDataOnlySEL( self, m_skinNameSelected ):
        animLayerArrayNormalize()
        self.exportData( True, m_skinNameSelected )

    def exportData( self, m_onlySelected = False, m_skinNameSelected = None, m_ignoreAdditive = False ):
        """
        ignoreADD            - ( False, None, True )
        exportOnlySelected   - ( True, "SkinName" )
        exportALL            - ()  or ( False, None, False )
        INPUT:  m_onlySelected   - boolean, if set to True, export only selected animations from the list
                                            for specified skinned mesh m_skinNameSelected
                                            if set to False export all animations from the list
                m_ignoreAdditive - boolean, if set to True, export all animations without additive animations
                                            if set ot False, export all
        RETURN: None if XRAY_RESOURCES_PATH directory don't exists
        """
        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Init section" )
        m_erroNumber = 0
        if (None == getRealXrayResourcesPath( 'animations/single' )):
            self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            return None
        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Snapshot section" )        
        #-------------------------------------------------------------------------------
        # Snapshot original
        #-------------------------------------------------------------------------------
        if ( None != m_skinNameSelected ):
            if ( not self.alreadyExistsSkin( m_skinNameSelected ) ):
                m_skinNameSelected = None
        m_startFrameOriginal = float(cmds.playbackOptions( query = True, min = True ))
        m_endFrameOriginal   = float(cmds.playbackOptions( query = True, max = True )) 
        m_FPSOriginal        = OpenMaya.MTime_uiUnit()
        m_anim_layers = m_animLayers() # class for Animation Layer nodes        
        m_anim_layers.animLayersMuteSnapshot() # get snapshot of .Mute attr from all animLayer nodes
        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "ProgressBar section" )             
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
        if (bool(m_animNode)):                     
            m_indexes = self.getIndexes() # Result: ( skins_count ,  [skins_indexes],  clips_count,   [clips_indexes] )
                                          #           m_indexes[0],  m_indexes[1],     m_indexes[2],  m_indexes[3] 
            for m_index in m_indexes[3]:
                if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Get Attributes" )
                m_skinName         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_skinNameClips')
                m_animName         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_animName')
                m_isAnimSel        = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_isAnimSel')
                m_startFrame       = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_startFrame')
                m_endFrame         = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_endFrame')
                m_localCoordCenter = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_LCC')
                m_FPS              = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_FPS')                  
                m_saveDirIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDirIsDefault')
                m_epsilonIsDefault = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilonIsDefault')
                if (m_saveDirIsDefault):
                    m_saveDir      = m_anim_saveDirDefault
                else:
                    m_saveDir      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_saveDir')
                if (m_epsilonIsDefault):
                    m_epsilon      = m_anim_epsilonDefault
                else:
                    m_epsilon      = cmds.getAttr(m_animNodeName + '._201102_clips[' + str(m_index) + ']._201102_epsilon')
                if (m_epsilon > m_anim_epsilonMax):
                    m_epsilon = m_anim_epsilonMax
                if (m_epsilon < m_anim_epsilonMin):
                    m_epsilon = m_anim_epsilonMin
                m_clipID           = m_index
                # m_xrayResourcesPathMeshes = 'XRAY_RESOURCES_PATH/animations'
                m_saveDir = m_saveDir.replace(m_xrayResourcesPathAnims, '' , 1)
                # -------------------------------
                m_isAdditive             = cmds.getAttr( '%s._201102_clips[%d]._201102_isAdditive'             %( m_animNodeName, m_index ) )
                m_baseAnimationIsDefault = cmds.getAttr( '%s._201102_clips[%d]._201102_baseAnimationIsDefault' %( m_animNodeName, m_index ) )
                if ( m_baseAnimationIsDefault ):
                    m_baseAnimation = m_anim_baseAnimationDefault
                else:
                    m_baseAnimation      = cmds.getAttr( '%s._201102_clips[%d]._201102_baseAnimation'          %( m_animNodeName, m_index ) )               
                m_animLayersAttr         = cmds.getAttr( '%s._201102_clips[%d]._201102_animLayers'             %( m_animNodeName, m_index ) )
                m_animLayersAttr         = m_animLayersAttr[ 0 ]               
                m_baseAnimation = m_baseAnimation.replace( m_xrayResourcesPathAnims, '' , 1 )
                if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Create directories" )
                # try to create directories
                if ( not createDirectories( getRealXrayResourcesPath( 'animations/single' ), m_saveDir ) ):
                    self.errorHandle.raiseMessage("ERROR   == > Can't create directory %s" %m_saveDir)
                    break # stop processing export current animation item
                # ---------------------------------------------------------------------------------------------
                if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Get filenames" )    
                m_expFilename      = getRealXrayResourcesPath( 'animations/single' ) + m_saveDir + '/' + m_animName
                m_expFilename      = m_expFilename.lower()
                m_baseAnimFilename = getRealXrayResourcesPath( 'animations/single' ) + m_baseAnimation
                # ---------------------------------------------------------------------------------------------
                m_ruleSel = False                
                if ( m_onlySelected ): # export only selected
                    if ( None != m_skinNameSelected and m_skinNameSelected == m_skinName and m_isAnimSel ):
                        m_ruleSel = True
                else:
                    m_ruleSel = True
                m_ruleAdd = True
                if ( m_ignoreAdditive ):
                   if ( m_isAdditive ): # ignore additive animations
                        m_ruleAdd = False
                else:
                    m_ruleAdd = True
                # ---------------------------------------------------------------------------------------------
                if ( m_ruleSel and m_ruleAdd ):
                    if   ( not cmds.objExists( m_skinName ) ):
                        self.errorHandle.raiseMessage("ERROR   == > Can't find object %s in scene" %m_skinName)
                    elif ( not  isMeshHasSkinCluster( m_skinName ) ):
                        self.errorHandle.raiseMessage("ERROR   == > Can't find skinCluster %s" %m_skinName)
                    else:
                        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Reset Scene" )
                        OpenMaya.MTime.setUIUnit( m_FPS )
                        cmds.select( clear = True )
                        cmds.select( m_skinName, replace = True )
                        cmds.playbackOptions( edit = True, min = m_startFrame )
                        cmds.playbackOptions( edit = True, max = m_endFrame )
                        m_anim_layers.animLayersMuteAll() # set to True .Mute attr in all animLayer nodes
                        m_obj_tuple = self.getConnectedAnimLayers()                                             
                        if ( m_obj_tuple ):
                            m_obj_array = m_obj_tuple[ 0 ]
                            if ( m_obj_array ):
                                for m_ind in range( m_obj_array.length() ):                                    
                                    if ( m_animLayersAttr ):
                                        m_mute_state = not m_animLayersAttr[ m_ind ]
                                    else:
                                        m_mute_state = False                                        
                                        self.errorHandle.raiseMessage("WARNING   == > may contain incorrect animation %s" %m_expFilename )
                                    cmds.animLayer( getNodeNameFromeMObject( m_obj_array[ m_ind ] ), edit = True, mute = m_mute_state )
                        # ---------------------
                        #      ADDITIVE
                        # ---------------------                                                
                        if ( m_isAdditive ):
                            try:
                                cmds.xray_anim_export( outfilename = m_expFilename, additiveToAnimation = m_baseAnimFilename, precision_t = m_epsilon, precision_r = m_epsilon, precision_s = m_epsilon )
                                #self.errorHandle.raiseMessage("   == > Successful %s additive to %s" %( m_expFilename, m_baseAnimFilename ) )
                            except:
                                #self.errorHandle.raiseMessage("ERROR   == > Can't export animation %s" %m_expFilename )
                                m_erroNumber = 4
                        # ---------------------
                        #     LOCAL CENTER
                        # ---------------------
                        elif ( "" != m_localCoordCenter ):
                            if ( cmds.objExists( m_localCoordCenter ) ):
                                try:
                                    cmds.xray_anim_export( outfilename = m_expFilename, localCoordinatCenterNode = m_localCoordCenter, precision_t = m_epsilon, precision_r = m_epsilon, precision_s = m_epsilon )
                                    #self.errorHandle.raiseMessage("   == > Successful %s local_center %s" %( m_expFilename, m_localCoordCenter ) )
                                except:
                                    #self.errorHandle.raiseMessage("ERROR   == > Can't export animation %s" %m_expFilename )
                                    m_erroNumber = 4
                            else:
                                self.errorHandle.raiseMessage("ERROR   == > Can't find local_center %s in %s" %( m_localCoordCenter, m_expFilename ) )
                                m_erroNumber = 4
                        # ---------------------
                        #      ANIM
                        # ---------------------
                        else:
                            try:
                                cmds.xray_anim_export( outfilename = m_expFilename, precision_t = m_epsilon, precision_r = m_epsilon, precision_s = m_epsilon )
                                #self.errorHandle.raiseMessage("   == > Successful %s" %m_expFilename )
                            except:
                                #self.errorHandle.raiseMessage("ERROR   == > Can't export animation %s" %m_expFilename )
                                m_erroNumber = 4
                # ------------------------------------------------------------
                if ( 0 == OpenMaya.MGlobal_mayaState() ):     # INCREASE PROGRESS STEP               
                    cmds.progressBar(gMainProgressBar, edit=True, step = 100/m_indexes[2])        
        # ------------------------------------------------------------ 
        if ( 0 == OpenMaya.MGlobal_mayaState() ):  
            cmds.progressBar(gMainProgressBar, edit = True, endProgress = True) # END PROGRESS
        #-------------------------------------------------------------------------------
        # Restore original
        #-------------------------------------------------------------------------------
        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Restore Shapshoot" )             
        m_anim_layers.animLayersMuteRestore() # restore from snapshot all animLayer nodes .Mute attribute    
        OpenMaya.MTime.setUIUnit(m_FPSOriginal)      
        cmds.playbackOptions( edit = True, min = m_startFrameOriginal )
        cmds.playbackOptions( edit = True, max = m_endFrameOriginal )
        if ( 'debug' == m_mode ): self.errorHandle.raiseMessage( "Exit Current Export" ) 
        return m_erroNumber

def animLayerArrayNormalize():
    m_selectionList = OpenMaya.MSelectionList()    # create selection list
    m_animNode      = OpenMaya.MObject()           # create MObject
    m_animNodeFn    = OpenMaya.MFnDependencyNode() # create a function set    
    m_selectionList.add( m_animNodeName )          
    m_selectionList.getDependNode(0, m_animNode)  # connect with MObject first element in the selection list
    m_animNodeFn.setObject( m_animNode )          # connect MObject with function set    
    if (bool(m_animNode)):
        m_animNodeFn = OpenMaya.MFnDependencyNode(m_animNode) # create a function set 
        m_indexesClips = OpenMaya.MIntArray() # Result: [2, 5, 6]
        m_indexesMessages = OpenMaya.MIntArray() # Result: [2, 5, 6]
        m_animNodeAttrClips = m_animNodeFn.attribute('_201102_clips')
        m_animNodeAttrMessages = m_animNodeFn.attribute('_201102_animLayersMessage')
        m_animNodePlugMessages = OpenMaya.MPlug( m_animNode, m_animNodeAttrMessages )        
        m_animNodePlugClips = OpenMaya.MPlug( m_animNode, m_animNodeAttrClips) # create a MPlug
        m_animNodePlugClips.getExistingArrayAttributeIndices(m_indexesClips)
        m_animNodePlugMessages.getExistingArrayAttributeIndices( m_indexesMessages )
    for m_index in m_indexesClips:
        if ( m_indexesMessages ):
            for m_ind in m_indexesMessages:
                m_currentState = cmds.getAttr( '%s._201102_clips[%d]._201102_animLayers[%d]' %( m_animNodeName, m_index, m_ind ) ) 
                if ( not m_currentState ):
                    # ----------- unlcok --------------
                    cmds.setAttr( '%s._201102_clips[%d]'                             %( m_animNodeName, m_index ), lock = False )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers'          %( m_animNodeName, m_index ), lock = False )  
                    # ------------- set ---------------          
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers[%d]'      %( m_animNodeName, m_index, m_ind ), lock = False ) 
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers[%d]'      %( m_animNodeName, m_index, m_ind ), False )
                    # ------------ lock ---------------
                    cmds.setAttr( '%s._201102_clips[%d]'                             %( m_animNodeName, m_index ), lock = True )
                    cmds.setAttr( '%s._201102_clips[%d]._201102_animLayers'          %( m_animNodeName, m_index ), lock = True )        