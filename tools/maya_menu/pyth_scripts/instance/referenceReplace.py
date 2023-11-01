#-----------------------------------------------------------------------------------
#	SCRIPT			referenceReplace.py
#	AUTHOR			Zaitsev Evgeniy 
#                               ev.zaitsev@gmail.com
#
#	DESCRIPTION:
#
#       replaceObjToRef()   - replace selected objects to reference object
#
#       USAGE:
#
#    1. Make sure the PYTHONPATH environment variable is set properly in the maya.env file or the script may not run properly.
#    2. Select objects
#    3. Run from MEL or add to Shelf:
#       python("import referenceReplace; m_replaceRefWindow = referenceReplace.m_replaceRefWin()"); 
#
#-----------------------------------------------------------------------------------
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya 
import os
import re
from AssetManagmentContext import *     # common functions

m_replaceReferencesGuiID = 'm_replaceReferencesGuiID'
#------------------------------------------------------------------------------------------------------
#     INTERFACE
#------------------------------------------------------------------------------------------------------
class m_replaceRefWin( object ):
    """
    Main UI window class defination
    """
    def __init__( self ):
        """
        Initialize window
        """
        self.m_dagPathSelectedAsset = None
        # ----------------------------------------------------------------------
        if cmds.window( m_replaceReferencesGuiID, exists = True ):
            cmds.deleteUI( m_replaceReferencesGuiID )
        # ----------------------------------------------------------------------
        self.m_window = cmds.window( m_replaceReferencesGuiID, title = "Replace Reference", width = 300, height = 200 )    
        self.m_formLayout = cmds.formLayout( parent = self.m_window )
        # ----------------------------------------------------------------------
        self.m_textLoadAsset = cmds.text( parent = self.m_formLayout, label = 'Load a ASSET from referenced file to use as a SOURCE for replacing', align = 'left' )
        self.m_textFieldAssetName = cmds.textFieldButtonGrp( parent = self.m_formLayout, 
                                 label = 'Asset:', 
                                 editable = False,
                                 buttonLabel = 'Load Asset',
                                 columnWidth3 = ( 40 , 220, 80 ),
                                 buttonCommand = lambda *args:self.uiCallback( "m_textFieldAssetNameBtn", args ),
                                 annotation = "Select DagContainer and press 'Load Asset' button" )
        self.m_separator1 = cmds.separator( parent = self.m_formLayout,
                                       height = 5, style='in' )
        self.m_textSelectByMask = cmds.text( parent =self. m_formLayout, label = 'Select objects by mask', align = 'left' )
        self.m_textFieldFilter = cmds.textFieldButtonGrp( parent = self.m_formLayout,
                                 label = 'Filter:', 
                                 editable = True,
                                 buttonLabel = 'Select',
                                 text = "*",
                                 columnWidth3 = ( 40 , 220, 80 ),
                                 buttonCommand = lambda *args:self.uiCallback( "m_textFieldFilterBtn", args ),
                                 annotation = "Enter filter and press 'Select' button" )
        self.m_checkBoxAllowInstances = cmds.checkBox( parent = self.m_formLayout,
                                                  label = 'Allow Instances',
                                                  value = False,
                                                  changeCommand = lambda *args: self.uiCallback( "m_checkBoxAllowInstances", args ),
                                                  annotation = "Allow Instances when press 'Select' button" )  
        self.m_checkBoxIgnoreTrWithoutShapes = cmds.checkBox( parent = self.m_formLayout,
                                                  label = 'Ignore Transforms without shapes',
                                                  value = True,
                                                  changeCommand = lambda *args: self.uiCallback( "m_checkBoxIgnoreTrWithoutShapes", args ),
                                                  annotation = "Ignore Transforms without shapes when press 'Select' button" )                                                                                            
        self.m_separator2 = cmds.separator( parent = self.m_formLayout,
                                       height = 5, style='in' )  
        self.m_textReplaceSelection = cmds.text( parent = self.m_formLayout, label = 'Replace SELECTION to SOURCE', align = 'left' )                                                                
        self.m_buttonReplace = cmds.button( parent = self.m_formLayout,
                                                label = 'Replace', 
                                                width = 80,
                                                command = lambda *args:self.uiCallback( "m_buttonReplace", args ),
                                                annotation = 'Replace selected transforms using Source' )
        # ----------------------------------------------------------------------        
        cmds.formLayout( self.m_formLayout, edit = True,
                    attachForm =    [    
                                         ( self.m_textLoadAsset,                'top',   5 ), 
                                         ( self.m_textLoadAsset,                'left',  5 ),
                                         ( self.m_textFieldAssetName,           'left',  5 ), 
                                         ( self.m_textFieldAssetName,           'right', 5 ),
                                         ( self.m_separator1,                   'left',  5 ), 
                                         ( self.m_separator1,                   'right', 5 ),
                                         ( self.m_textSelectByMask,             'left',  5 ), 
                                         ( self.m_textSelectByMask,             'right', 5 ),
                                         ( self.m_textFieldFilter,              'left',  5 ), 
                                         ( self.m_textFieldFilter,              'right', 5 ), 
                                         ( self.m_checkBoxAllowInstances,       'left',  5 ),
                                         ( self.m_checkBoxIgnoreTrWithoutShapes,'left',  5 ),
                                         ( self.m_separator2,                   'left',  5 ), 
                                         ( self.m_separator2,                   'right', 5 ),
                                         ( self.m_textReplaceSelection,         'left',  5 ), 
                                         ( self.m_textReplaceSelection,         'right', 5 ),
                                         ( self.m_buttonReplace,                'left',  5 ), 
                                    ], 
                    attachControl = [
                                         ( self.m_textFieldAssetName,              'top', 5, self.m_textLoadAsset ), 
                                         ( self.m_separator1,                      'top', 5, self.m_textFieldAssetName ),  
                                         ( self.m_textSelectByMask,                'top', 5, self.m_separator1 ),  
                                         ( self.m_textFieldFilter,                 'top', 5, self.m_textSelectByMask ),
                                         ( self.m_checkBoxAllowInstances,          'top', 5, self.m_textFieldFilter ), 
                                         ( self.m_checkBoxIgnoreTrWithoutShapes,   'top', 5, self.m_checkBoxAllowInstances ), 
                                         ( self.m_separator2,                      'top', 5, self.m_checkBoxIgnoreTrWithoutShapes ), 
                                         ( self.m_textReplaceSelection,            'top', 5, self.m_separator2 ), 
                                         ( self.m_buttonReplace,                   'top', 5, self. m_textReplaceSelection ),  
                                    ],
                    attachNone =    [    
                                         ( self.m_textLoadAsset,                   'right'  ),
                                         ( self.m_textLoadAsset,                   'bottom' ),
                                         ( self.m_textFieldAssetName,              'bottom' ),  
                                         ( self.m_separator1,                      'bottom' ), 
                                         ( self.m_textSelectByMask,                'bottom' ), 
                                         ( self.m_textFieldFilter,                 'bottom' ),
                                         ( self.m_checkBoxAllowInstances,          'right'  ),
                                         ( self.m_checkBoxAllowInstances,          'bottom' ),
                                         ( self.m_checkBoxIgnoreTrWithoutShapes,   'right'  ),
                                         ( self.m_checkBoxIgnoreTrWithoutShapes,   'bottom' ), 
                                         ( self.m_separator2,                      'bottom' ),  
                                         ( self.m_textReplaceSelection,            'bottom' ),
                                         ( self.m_buttonReplace,                   'right'  ),
                                         ( self.m_buttonReplace,                   'bottom' ),
                                    ])        
        cmds.showWindow( m_replaceReferencesGuiID )
    #------------------------------------------------------------------------------------------------------
    #     uiCallback
    #------------------------------------------------------------------------------------------------------
    def uiCallback( self, *args ):
        """
        Callback for UI
        """   
        m_input = args[0]
        #-------------------------------
        #    m_textFieldAssetNameBtn
        #-------------------------------
        if ( 'm_textFieldAssetNameBtn' == m_input ):
            m_temp = getAssetFromSelection()
            if ( m_temp ):
                self.m_dagPathSelectedAsset = m_temp
                cmds.textFieldButtonGrp( self.m_textFieldAssetName, edit = True, fileName = self.m_dagPathSelectedAsset.fullPathName() ) 
        #-------------------------------
        #    m_textFieldFilterBtn
        #-------------------------------
        if ( 'm_textFieldFilterBtn' == m_input ): 
            m_maskStr               = cmds.textFieldButtonGrp( self.m_textFieldFilter, query = True, fileName = True )
            m_allowInstances        = cmds.checkBox( self.m_checkBoxAllowInstances, query = True, value = True ) 
            m_ignoreTrWithoutShapes = cmds.checkBox( self.m_checkBoxIgnoreTrWithoutShapes, query = True, value = True )
            selectObjectsByMask( m_maskStr, m_ignoreTrWithoutShapes, not m_allowInstances )          
        #-------------------------------
        #    m_buttonReplace
        #-------------------------------
        if ( 'm_buttonReplace' == m_input ): 
            if ( self.m_dagPathSelectedAsset ):           
                replaceObjToRef( self.m_dagPathSelectedAsset.fullPathName() )
#------------------------------------------------------------------------------------------------------
#     getAssetFromSelection
#------------------------------------------------------------------------------------------------------ 
def getAssetFromSelection():
    """
    RETURN: MDagPath if last node from selection is DagContainer,
            None - otherwise
    """
    m_selList = cmds.ls( selection = True )
    if ( m_selList ):
        m_selObj = m_selList[ len( m_selList ) - 1 ]
        if ( not isObjFromReferencedFile( m_selObj ) ):
            OpenMaya.MGlobal.displayWarning( "%s not from Referenced file" %m_selObj )
            return None
        if ( not isObjDagContainer( m_selObj ) ):
            OpenMaya.MGlobal.displayWarning( "%s not a Asset" %m_selObj )
            return None    
    else:
        OpenMaya.MGlobal.displayWarning( "Select at list one object" )
        return None
    m_dagPathSel = getMDagPathFromNodeName( m_selObj )
    return m_dagPathSel
#------------------------------------------------------------------------------------------------------
#     replaceObjToRef
#------------------------------------------------------------------------------------------------------            
def replaceObjToRef ( m_inputObjStr ):
    """
    replace objects from selection to m_inputObjStr ( Asset from reference file )
    INPUT:  
            m_inputObjStr    - string    -    '|border_small_instance|border_small_inner' - asset from reference file
                        
    RETURN: True - if done properly, False - otherwise
    USAGE:  replaceObjToRef( "border_small_inner" )
    """
    if ( not isObjFromReferencedFile( m_inputObjStr ) ):
        OpenMaya.MGlobal.displayWarning( "%s not from Referenced file" %m_filename )
        return False
    if ( not isObjDagContainer( m_inputObjStr ) ):
        OpenMaya.MGlobal.displayWarning( "%s not a Asset" %m_filename )
        return False
    #------------------------------------------
    m_selectionList = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList( m_selectionList )
    m_selListIt = OpenMaya.MItSelectionList( m_selectionList )
    #----------------- add all instances to selection list -----------------  
    while not m_selListIt.isDone():
        m_objCurr = OpenMaya.MObject()
        m_selListIt.getDependNode( m_objCurr )
        if ( m_objCurr.hasFn( OpenMaya.MFn.kTransform ) ):
            m_fnDagCurr = OpenMaya.MFnDagNode( m_objCurr )
            # ignore referenced
            if ( not m_fnDagCurr.isFromReferencedFile() and not isObjInDagContainer( m_fnDagCurr.fullPathName() ) ):
                #------------------------------------------
                m_filename  = cmds.reference( m_inputObjStr, query = True, filename = True )
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
                m_dagPathLastAdded = getMDagPathFromNodeName( m_lasAdded )
                cmds.select( clear = True )
                # Get parent object
                m_objParent = m_fnDagCurr.parent( 0 )
                # replace pos, rot, scale ...
                TRSPPSPfromTo( m_fnDagCurr.fullPathName(), m_dagPathLastAdded.fullPathName() )
                if ( not m_objParent.hasFn( OpenMaya.MFn.kWorld ) ):
                    m_fnDagNodeParent = OpenMaya.MFnDagNode( m_objParent )
                    # parent to PARENT
                    try:
                        cmds.parent( m_dagPathLastAdded.fullPathName(), m_fnDagNodeParent.fullPathName() ) 
                    except:
                        print "Can't parent %s to %s" %( m_dagPathLastAdded.fullPathName(), m_fnDagNodeParent.fullPathName() )           
        m_selListIt.next()
    #--------------------------------------------
    setBlackBoxAll()
    #----------------- remove  -----------------
    m_resultString = 'select -clear;\n'          
    m_selListIt = OpenMaya.MItSelectionList( m_selectionList )
    while not m_selListIt.isDone():
        m_objCurr = OpenMaya.MObject()
        m_selListIt.getDependNode( m_objCurr )
        if ( m_objCurr.hasFn( OpenMaya.MFn.kTransform ) ):
            m_fnDagCurr = OpenMaya.MFnDagNode( m_objCurr )
            # ignore referenced
            if ( not m_fnDagCurr.isFromReferencedFile() and not isObjInDagContainer( m_fnDagCurr.fullPathName() ) ):
                # add to selection
                m_resultString += "select -add %s;\n" %m_fnDagCurr.fullPathName()
                #------------------------------------------            
        m_selListIt.next()
    m_resultString += "doDelete;\n"
    OpenMaya.MGlobal.executeCommand( m_resultString )