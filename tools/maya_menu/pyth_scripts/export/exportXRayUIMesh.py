#-----------------------------------------------------------------
#	SCRIPT			exportXRayUIMesh.py
#	AUTHOR			Zaitsev Evgeniy
#				    ev.zaitsev@gmail.com
#	DATE:			28 September 2010
#
#	DESCRIPTION:	Script with user intreface for export
#                   meshes into S.T.A.L.K.E.R. 2 project
#
#       VERSION:	    201102
#
#-----------------------------------------------------------------
import maya.cmds as cmds    # work with maya commands
import maya.mel as mel      # work with mel
import ConfigParser         # config parser
import string               # string parser
import re                   # regular expression
import os                   # work with file system
from exportXRayContext import *        # common functions
from exportXRayContextMesh import *    # work with 'xray_mesh_lister' node
from exportXRayUI import *             # common functions for UI
#-----------------------------------------------------------------------------------
#	MAIN UI WINDOW CLASS FOR MESHES
#-----------------------------------------------------------------------------------
class m_meshWin(object):
    """
    Main UI window class definition
    """
    def __init__(self):
        """
        Initialize window
        """        
        self.m_node = m_meshNode() # initialize the node
        self.errorHandle = m_meshTrace() # error tracer
        # delete ui window if opened
        if cmds.window( m_meshProportiesGuiID, exists = True ):
            cmds.deleteUI( m_meshProportiesGuiID, window = True )
        # create the window
        self.m_window = cmds.window( m_meshProportiesGuiID, title = 'Mesh Properties', width = 500, height = 500 )
        # create the pane
        self.m_paneLayout = cmds.paneLayout(parent = self.m_window, configuration='single') # vertical2   <->  single
		#----------------------
		# Left Pane
		#----------------------
        self.m_formLayoutLeft = cmds.formLayout(parent = self.m_paneLayout)
        #----------------------
        # 3 buttons: Add, Del, Rename
        self.m_buttonAddMesh = cmds.button(parent = self.m_formLayoutLeft, 
                                      label = 'Add', 
                                      width = 80,
                                      command = lambda *args:self.uiButtonCallback("m_buttonAddMesh", args),
                                      annotation = "Select object in Outliner and press 'Add' button")
        self.m_buttonDelMesh = cmds.button(parent = self.m_formLayoutLeft,
                                      label = 'Delete',
                                      width = 80,
                                      command = lambda *args:self.uiButtonCallback("m_buttonDelMesh", args),
                                      annotation = "Remove selected items from srollList")
        self.m_buttonRefresh = cmds.button(parent = self.m_formLayoutLeft, 
                                         label = 'Refresh',
                                         width = 80,
                                         command = lambda *args:self.uiButtonCallback("m_buttonRefresh", args),
                                         annotation = "Refresh scrollList")
        #----------------------
        # create scrollList
        self.m_textScrollList = cmds.textScrollList(parent = self.m_formLayoutLeft,
                                               selectCommand = lambda *args:self.uiScrollListCallback(args),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = True )
        # create popupMenu
        self.m_textPopupMenu = cmds.popupMenu(parent = self.m_textScrollList)
        self.m_textPopupMenuItemRefresh = cmds.menuItem(parent = self.m_textPopupMenu, 
                                                   label = 'Refresh', 
                                                   command = lambda *args:self.uiPopupMenuCallback("m_textPopupMenuItemRefresh",args))        
        #----------------------
        # 3 buttons: Export All, Export Sel, Invert Sel
        self.m_buttonExportAll = cmds.button(parent = self.m_formLayoutLeft, 
                                        label = 'Export All', 
                                        width = 80,
                                        command = lambda *args:self.uiButtonCallback("m_buttonExportAll", args),
                                        annotation = "Export all items specified in scrollList")
        self.m_buttonExportSel = cmds.button(parent = self.m_formLayoutLeft, 
                                        label = 'Export Sel', 
                                        width = 80,
                                        command = lambda *args:self.uiButtonCallback("m_buttonExportSel", args),
                                        annotation = "Export only selected items specified in scrollList")
        self.m_buttonInvertSel = cmds.button(parent = self.m_formLayoutLeft,
                                        label = 'Invert Sel', 
                                        width = 80,
                                        command = lambda *args:self.uiButtonCallback("m_buttonInvertSel", args),
                                        annotation = "Invert selection")
        self.m_checkBoxNoDB = cmds.checkBox( label = "No DB", 
                                             value = False )           

        #----------------------
        # add txt label 'Save to'
        self.m_textSaveDir = cmds.text( parent = self.m_formLayoutLeft, label = 'Save to', align = 'left', width = 40)
        # add textField
        self.m_textFieldSaveDir = cmds.textField( parent = self.m_formLayoutLeft, 
                                            editable = False,
                                            changeCommand = lambda *args: self.uiSaveDirCallback("m_textFieldSaveDir",args),
                                            width = 150,
                                            annotation = "Save to")
        # add symbol button
        self.m_symbolButtonSaveDir = cmds.symbolButton(parent = self.m_formLayoutLeft, 
                                                  image = "navButtonBrowse.xpm", 
                                                  annotation = "Save to: Browse for the save folder",
                                                  command = lambda *args: self.uiSaveDirCallback("m_symbolButtonSaveDir",args))     											 
        #----------------------
        # Info Frame
        self.m_frameLayoutInfo = cmds.frameLayout(parent = self.m_formLayoutLeft, 
                                             labelVisible = False,
                                             marginHeight = 2,
                                             marginWidth = 2,                                              
                                             height = 28,
                                             borderStyle='etchedIn')
        # create progressBar
        self.m_progressControl = cmds.progressBar( parent = self.m_frameLayoutInfo, maxValue = 100 )
        cmds.progressBar( self.m_progressControl, edit = True, visible = False )                                             
		#----------------------
		# Format Left Pane
		#----------------------
        cmds.formLayout(self.m_formLayoutLeft, edit = True,
            attachForm =    [
                             (self.m_buttonAddMesh, 'top', 5),
                             (self.m_buttonAddMesh, 'left', 5), 
                             (self.m_buttonDelMesh, 'top', 5),
                             (self.m_buttonRefresh, 'top', 5),
                             (self.m_textScrollList, 'left', 5),
                             (self.m_textScrollList, 'right', 5),                            
                             (self.m_buttonExportAll, 'left', 5),    
                             (self.m_buttonExportAll, 'bottom', 5), 
                             (self.m_textFieldSaveDir,         'top', 5),							 
                             (self.m_textSaveDir,         'left', 5),
                             (self.m_symbolButtonSaveDir, 'right', 5),							 
                             (self.m_frameLayoutInfo, 'left', 5),
                             (self.m_frameLayoutInfo, 'right', 5),
                             (self.m_frameLayoutInfo, 'bottom', 5)
                            ],
            attachControl = [    
                             (self.m_buttonDelMesh, 'left', 5, self.m_buttonAddMesh),
                             (self.m_buttonRefresh, 'left', 5, self.m_buttonDelMesh),
                             (self.m_textScrollList, 'top', 5, self.m_buttonAddMesh),
                             (self.m_textScrollList, 'bottom', 5, self.m_buttonExportAll),
                             (self.m_buttonExportSel, 'left', 5, self.m_buttonExportAll),
                             (self.m_buttonInvertSel, 'left', 5, self.m_buttonExportSel),
                             (self.m_checkBoxNoDB, 'left', 5, self.m_buttonInvertSel),
                             (self.m_buttonExportAll, 'bottom', 5, self.m_textSaveDir),
                             (self.m_buttonExportSel, 'bottom', 5, self.m_textSaveDir),
                             (self.m_buttonInvertSel, 'bottom', 5, self.m_textSaveDir),
                             (self.m_checkBoxNoDB, 'bottom', 5, self.m_textSaveDir),
                                 (self.m_textSaveDir,  'bottom', 5, self.m_frameLayoutInfo),
                                 (self.m_textFieldSaveDir, 'left', 5, self.m_textSaveDir),
                                 (self.m_textFieldSaveDir, 'right', 5, self.m_symbolButtonSaveDir),
                                 (self.m_textFieldSaveDir,  'bottom', 5, self.m_frameLayoutInfo),
                                 (self.m_symbolButtonSaveDir, 'bottom', 5, self.m_frameLayoutInfo)			 
                            ],
            attachNone =    [    
                             (self.m_buttonAddMesh, 'right'),
                             (self.m_buttonAddMesh, 'bottom'),
                             (self.m_buttonDelMesh, 'right'),
                             (self.m_buttonDelMesh, 'bottom'),
                             (self.m_buttonRefresh, 'right'),
                             (self.m_buttonRefresh, 'bottom'),
                             (self.m_frameLayoutInfo, 'top'),                             
                             (self.m_buttonExportAll, 'right'),
                             (self.m_buttonExportAll, 'top'),
                             (self.m_buttonExportSel, 'right'),
                             (self.m_buttonExportSel, 'top'),
                             (self.m_buttonInvertSel, 'right'),
                             (self.m_buttonInvertSel, 'top'),
                             (self.m_checkBoxNoDB, 'right'),
                             (self.m_checkBoxNoDB, 'top') ,
                                 (self.m_textSaveDir, 'right'),
                                 (self.m_textSaveDir, 'top'), 
                                 (self.m_textFieldSaveDir, 'top'),
                                 (self.m_symbolButtonSaveDir, 'left'),
                                 (self.m_symbolButtonSaveDir, 'top')							 
                            ])                          
		#----------------------
		# Right Pane
		#----------------------
        self.m_formLayoutRight = cmds.formLayout(parent = self.m_paneLayout)
        #----------------------
        # add txt label 'Type'
        self.m_textSelectObjType = cmds.text(parent = self.m_formLayoutRight, label = 'Type', align = 'left', width = 40)
        # add option menu
        self.m_optionMenuObjType = cmds.optionMenu(parent = self.m_formLayoutRight,
                                              changeCommand = lambda *args: self.uiOptionMenuCallback("m_optionMenuObjType",args),
                                              annotation = "Select type of the object")
        for m_item in m_objectType:
            cmds.menuItem( parent = self.m_optionMenuObjType, label = m_item ) # read options from global array m_objectType  
        #----------------------
        # add txt label 'Tags' 
        self.m_textSelectObjTags = cmds.text(parent = self.m_formLayoutRight, label = 'Tags', align = 'left', width = 40)
        self.m_formLayoutTags = cmds.formLayout(parent = self.m_formLayoutRight)
        # array of tag group frames
        self.m_frameLayoutTag = []
        # array of checkBox tags in specified group frame
        self.m_checkBoxTag = []
        i = 0
        while ( i != len( m_categoryTree.keys() ) ):
            for key in m_categoryTree.keys():
                if ( i == key[0] ):
                   # global hash with tags m_categoryTree
                    self.m_frameLayoutTag.append(cmds.frameLayout(parent = self.m_formLayoutTags, 
                                                             label = key[1], 
                                                             marginHeight = 2, 
                                                             marginWidth = 2,
                                                             borderStyle='etchedIn'))
                    for val in m_categoryTree[key]: # global hash with tags m_categoryTree
                        self.m_checkBoxTag.append(cmds.checkBox(label = val, 
                                                           value = False,
                                                           changeCommand = lambda *args: self.uiCheckBoxCallback(args)))
            i +=1
        #----------------------------
        # initialize background color
        for m_tag in self.m_checkBoxTag:
            cmds.checkBox( m_tag, edit = True, enableBackground = True )
            cmds.checkBox( m_tag, edit = True, backgroundColor = m_checkBoxColor )
            cmds.checkBox( m_tag, edit = True, enableBackground = False )
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = True )
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = False )
        # allign all self.m_frameLayoutTag to the 'top' of self.m_formLayoutTags
        # exclude first item from array
        i = 0
        cmds.formLayout( self.m_formLayoutTags, edit = True,
                                                attachForm = ( ( self.m_frameLayoutTag[i], 'top',  0 ),
                                                               ( self.m_frameLayoutTag[i], 'left', 0 ) ) )
        for tagGroup in self.m_frameLayoutTag:
            if (i != 0):
                cmds.formLayout( self.m_formLayoutTags, edit = True,
                                                        attachControl = ( self.m_frameLayoutTag[i], 'left', 2, self.m_frameLayoutTag[i - 1] ) )
            i = i + 1 
        #----------------------
        # add txt label 'Save to'
        self.m_textSaveDir = cmds.text( parent = self.m_formLayoutRight, label = 'Save to', align = 'left', width = 40)
        # add textField
        self.m_textFieldSaveDir = cmds.textField( parent = self.m_formLayoutRight, 
                                            editable = False,
                                            changeCommand = lambda *args: self.uiSaveDirCallback("m_textFieldSaveDir",args),
                                            width = 150,
                                            annotation = "Save to")
        # add symbol button
        self.m_symbolButtonSaveDir = cmds.symbolButton(parent = self.m_formLayoutRight, 
                                                  image = "navButtonBrowse.xpm", 
                                                  annotation = "Save to: Browse for the save folder",
                                                  command = lambda *args: self.uiSaveDirCallback("m_symbolButtonSaveDir",args))     
		#----------------------
		# Format Right Pane
		#----------------------
        cmds.formLayout(self.m_formLayoutRight, edit = True,
            attachForm =    [    
                                 (self.m_textSelectObjType,   'top', 5),
                                 (self.m_textSelectObjType,   'left', 5),
                                 (self.m_optionMenuObjType,   'right', 5),
                                 (self.m_optionMenuObjType,   'top', 5),
                                 (self.m_textSelectObjTags,   'left', 5),
                                 (self.m_formLayoutTags,      'right', 5),
                                 (self.m_textSaveDir,         'left', 5),
                                 (self.m_symbolButtonSaveDir, 'right', 5)
                            ],
            attachControl = [    
                                 (self.m_optionMenuObjType, 'left', 5, self.m_textSelectObjType),
                                 (self.m_textSelectObjTags, 'top', 5, self.m_textSelectObjType),
                                 (self.m_formLayoutTags, 'left', 5, self.m_textSelectObjTags),
                                 (self.m_formLayoutTags, 'top', 5, self.m_optionMenuObjType),
                                 (self.m_textSaveDir, 'top', 5, self.m_formLayoutTags),
                                 (self.m_textFieldSaveDir, 'left', 5, self.m_textSaveDir),
                                 (self.m_textFieldSaveDir, 'right', 5, self.m_symbolButtonSaveDir),
                                 (self.m_textFieldSaveDir, 'top', 5, self.m_formLayoutTags),
                                 (self.m_symbolButtonSaveDir, 'top', 5, self.m_formLayoutTags)
                            ],
            attachNone =    [    
                                 (self.m_textSelectObjType, 'right'),
                                 (self.m_textSelectObjType, 'bottom'),
                                 (self.m_optionMenuObjType, 'bottom'),
                                 (self.m_textSelectObjTags, 'right'),
                                 (self.m_textSelectObjTags, 'bottom'),
                                 (self.m_formLayoutTags, 'bottom'),
                                 (self.m_textSaveDir, 'right'),
                                 (self.m_textSaveDir, 'bottom'), 
                                 (self.m_textFieldSaveDir, 'bottom'),
                                 (self.m_symbolButtonSaveDir, 'left'),
                                 (self.m_symbolButtonSaveDir, 'bottom')
                            ])
        # READ DATA FROM NODE
        self.uiRefreshScrollList()                          
        cmds.showWindow( m_meshProportiesGuiID )

    def uiPopupMenuCallback(self, *args):
        """
        Callback for popupMenu
        """
        m_items = args[0]
        #-------------------------------
        #     m_textPopupMenuItemRefresh
        #-------------------------------          
        if (m_items == 'm_textPopupMenuItemRefresh'):
            self.uiRefreshScrollList() # reload ScrollList
        
    def uiButtonCallback(self, *args):
        """
        Callback for buttons
        INPUT: [variable name]
        """
        m_button = args[0]
        #-------------------------------
        #     m_buttonAddMesh
        #-------------------------------
        if (m_button == 'm_buttonAddMesh'):            
            m_objSelectedLong = cmds.ls(selection = True, long = True) # get selected objects
            for m_obj in m_objSelectedLong:
                self.m_node.addObj(m_obj) # add object to node
            self.uiRefreshScrollList()  # reload ScrollList
        #-------------------------------
        #     m_buttonDelMesh
        #-------------------------------
        elif (m_button == 'm_buttonDelMesh'):
            m_items = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True ) # get selected items from scrollList            
            if (None != m_items):
                if (0 != len(m_items)):
                    for m_obj in m_items:
                        if (self.m_node.delObj(string.split(m_obj)[1])): # delete from node
                            cmds.textScrollList( self.m_textScrollList, edit = True, removeItem = [ m_obj ] ) # remove from scrollList 
                            # reset obj type background color
                            cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = True )
                            cmds.optionMenu( self.m_optionMenuObjType, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
                            cmds.optionMenu( self.m_optionMenuObjType, edit = True, select = 1 ) # reset objType
                            cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = False )                   
                            # reset tags and backg colors
                            for m_tag_checkbox in self.m_checkBoxTag:
                                cmds.checkBox( m_tag_checkbox, edit = True, value = False )
                                cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = True )
                                cmds.checkBox( m_tag_checkbox, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
                                cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = False )  
        #-------------------------------
        #     m_buttonRefresh
        #-------------------------------                                               
        elif (m_button == 'm_buttonRefresh'):
            self.uiRefreshScrollList() # reload ScrollList
        #-------------------------------
        #     m_buttonExportAll
        #-------------------------------                                     
        elif (m_button == 'm_buttonExportAll'):
            if ( cmds.checkBox( self.m_checkBoxNoDB, query = True, value = True ) ):
                self.m_node.exportDataNoDBAll()
            else:
                self.m_node.exportDataAll()
        #-------------------------------
        #     m_buttonExportSel
        #-------------------------------              
        elif (m_button == 'm_buttonExportSel'):
            if ( cmds.checkBox( self.m_checkBoxNoDB, query = True, value = True ) ):
                self.m_node.exportDataNoDBSel()
            else:
                self.m_node.exportDataSel()      
        #-------------------------------
        #     m_buttonInvertSel
        #-------------------------------
        elif (m_button == 'm_buttonInvertSel'):
            if (0 != self.m_node.count()):
                m_items_all = cmds.textScrollList( self.m_textScrollList, query = True, allItems = True )   # get all items from scrollList
                m_items_sel = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True ) # get selected items from scrollList   
                if (None != m_items_sel):            
                    if (0 != len(m_items_sel)):
                        m_items_unsel = []
                        cmds.textScrollList( self.m_textScrollList, deselectAll = True)
                        self.m_node.setAllSelectionStates(False) # reset selection state
                        for m_obj in m_items_all: # init unselected items list
                            if (0 == m_items_sel.count(m_obj)):
                                m_items_unsel.append(m_obj)       
                        for m_obj in m_items_unsel: # select unselected
                            self.m_node.setSelectionState(string.split(m_obj)[1], True)
                            cmds.textScrollList( self.m_textScrollList, edit = True, selectItem = m_obj )           
                else:
                    for m_obj in m_items_all:
                        cmds.textScrollList( self.m_textScrollList, edit = True, selectItem = m_obj ) 
                    self.m_node.setAllSelectionStates(True)
                self.uiLoadDataFromNode() # reload ScrollList
    
    def uiCheckBoxCallback(self, *args):
        """
        Callback for self.m_checkBoxTag
        """
        m_objSelTags = []  # array of cheked tags
        for m_tag in self.m_checkBoxTag:
            if ( cmds.checkBox( m_tag, query = True, value = True ) ):
                cmds.checkBox( m_tag, edit = True, enableBackground = True )
                cmds.checkBox( m_tag, edit = True, backgroundColor = m_checkBoxColor )                 
                m_objSelTags.append( getFullPathToTag( cmds.checkBox( m_tag, query = True, label = True ) ) )
            else:
                cmds.checkBox( m_tag, edit = True, enableBackground = False )  
        m_items = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True )  # get selected items from scrollList
        if (None != m_items):
            if (0 != len(m_items)):
                for m_obj in m_items:
                    self.m_node.setObjTag( string.split( m_obj )[1], m_objSelTags ) # write to node

    def uiOptionMenuCallback(self, *args):
        """
        Callback for self.m_optionMenuObjType
        """
        m_optionMenu = args[0]
        if ( m_optionMenu == 'm_optionMenuObjType' ):
            cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = True )
            cmds.optionMenu( self.m_optionMenuObjType, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
            cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = False )                   
            m_items = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True ) # get selected items from scrollList as string array                             
            if (None != m_items):
                if (0 != len(m_items)):
                    i = 0
                    while (i != len( m_items )):
                        m_obj = m_items[i] # get old label 
                        self.m_node.setObjType( string.split(m_obj)[1], cmds.optionMenu( self.m_optionMenuObjType, query = True, value = True ) ) # write to node
                        i += 1
            self.uiRefreshScrollList()   # reload ScrollList

    def uiSaveDirCallback(self, *args):
        """
        Callback for self.m_textFieldSaveDir and self.m_symbolButtonSaveDir
        """
        m_button = args[0]
        #-------------------------------
        #     m_textFieldSaveDir
        #-------------------------------
        if (m_button == 'm_textFieldSaveDir'):
            pass
        #--------------------------------
        #    m_symbolButtonSaveDir
        #--------------------------------
        elif (m_button == 'm_symbolButtonSaveDir'):
            if (None == getRealXrayResourcesPath( 'models' )): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH/models don't exists")
            else:
                #---------------------------------------------------------------------------------------
                # read data from node
                m_hash = self.m_node.iterItems()
                m_items = cmds.textScrollList( self.m_textScrollList, query = True, allItems = True ) # get all items from scrollList
                m_hashSelected = {} # create dictionary with selected items
                m_saveDir = ""
                for obj,data in m_hash.iteritems():
                    if ( data[0] ):
                        if ( None != m_items and 0 != len( m_items ) ):
                            i = 0
                            while ( i != len( m_items ) ):
                                m_obj = m_items[i] # get label
                                m_nodeName = string.split( m_obj )[1]
                                if ( obj == m_nodeName ):
                                    m_hashSelected[ obj ] = ( data[0], data[1], data[2], data[3], data[4] )
                                    break
                                i += 1
                # get selected DATA from the node
                m_hash = m_hashSelected
                if (0 != len( m_hash )):
                    #---------------------
                    #   set save dir
                    #---------------------
                    for obj,data in m_hash.iteritems():
                        m_saveDir = data[3] # get save dir
                        break
                m_realPath = getRealXrayResourcesPath( 'models' )
                m_startingDirectory = "%s%s" %( m_realPath, m_saveDir )
                #---------------------------------------------------------------------------------------
                self.m_fileDialog2SaveDir = cmds.fileDialog2( fileFilter='Folder',
                                                        dialogStyle=2,
                                                        fileMode=3, 
                                                        caption='Save file to ...',
                                                        startingDirectory = m_startingDirectory,
                                                        okCaption='Set Dir')
                if ( None == self.m_fileDialog2SaveDir ):
                   # print "Cancel button pressed"
                   pass
                else:
                    m_saveDir = self.m_fileDialog2SaveDir[0]
                    m_saveDir = m_saveDir.lower()
                    # if 'x:/resources/exported/models' don't exist, redefine m_saveDir
                    if ( -1 == m_saveDir.find( m_realPath ) ):
                        m_saveDir = m_realPath
                    m_saveDir = m_saveDir.replace( m_realPath, '' , 1 )
                    # Set button pressed
                    cmds.textField( self.m_textFieldSaveDir, edit = True, fileName = str( m_saveDir ) ) # write m_saveDir to textField
                    cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = True ) 
                    cmds.textField( self.m_textFieldSaveDir, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
                    cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = False ) 
                    # -------------------------------------------------
                    m_items = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True )  # get selected items from scrollList as string array                           
                    if (None != m_items):
                        if (0 != len(m_items)):
                            i = 0
                            while (i != len(m_items)):
                                m_obj = m_items[i] # get old label
                                self.m_node.setSaveDir( string.split(m_obj)[1], str( m_xrayResourcesPathMeshes + m_saveDir ) ) # write m_saveDir to the node
                                i += 1
                    self.uiRefreshScrollList() # reload ScrollList                                                      

    def uiScrollListCallback(self, *args):
        """
        Callback for self.m_textScrollList
        """        
        m_items_all = cmds.textScrollList( self.m_textScrollList, query = True, allItems = True ) # get all items from scrollList
        # reset selection state in the node
        for m_obj in m_items_all:
            self.m_node.setSelectionState( string.split(m_obj)[1], False )
        m_items = cmds.textScrollList( self.m_textScrollList, query = True, selectItem = True )  # get selected items from scrollList
        if (None != m_items):
            for m_obj in m_items:
                self.m_node.setSelectionState( string.split(m_obj)[1], True )
        self.uiLoadDataFromNode() # reload ScrollList

    def uiRefreshScrollList( self ):
        """
        Refresh self.m_textScrollList
        """
        # read data from node
        m_hash = self.m_node.iterItems()
        # obj      - '|group3|group1'
        # data[0]  - False       
        # data[1]  - 'Static'
        # data[2]  - ['Location|Industrial', 'Architectural|Building']
        # data[3]  - 'XRAY_RESOURCES_PATH/models/props'
        # data[4]  - 'group1    |group3|group1    Static    XRAY_RESOURCES_PATH/models/props'
        cmds.textScrollList( self.m_textScrollList, edit = True, removeAll = True ) # clear ScrollList
        for obj,data in m_hash.iteritems():
            cmds.textScrollList( self.m_textScrollList, edit = True, append = [ data[4] ] ) 
            if (data[0]):
                cmds.textScrollList( self.m_textScrollList, edit = True, selectItem = data[4] )
        # get selected DATA from the node 
        self.uiLoadDataFromNode()                      

    def uiLoadDataFromNode( self ):
        """
        Load data to right panel from
        """
        # reset obj type background color
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = True )
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, select = 1 ) # reset objType            
        cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = False )         
        # reset tags and backg colors
        for m_tag_checkbox in self.m_checkBoxTag:
            cmds.checkBox( m_tag_checkbox, edit = True, value = False )
            cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = True )
            cmds.checkBox( m_tag_checkbox, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
            cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = False )  
        # reset txtField background color
        cmds.textField( self.m_textFieldSaveDir, edit = True, fileName = '' ) # reset txtField
        cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = True ) 
        cmds.textField( self.m_textFieldSaveDir, edit = True, backgroundColor = m_checkBoxColor ) # single selection color
        cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = False ) 
        # ---------------------------------------------------------------------------
        # ---------------------------------------------------------------------------
        cmds.progressBar( self.m_progressControl, edit = True, visible = True )        # SHOW PROGRESS BAR
        cmds.progressBar( self.m_progressControl, edit = True, beginProgress = True )  # PROGRESS BEGIN   
        cmds.progressBar( self.m_progressControl, edit = True, step = 50 )             # PROGRESS 25%
        # read data from node
        m_hash = self.m_node.iterItems()
        # obj      - '|group3|group1'
        # data[0]  - False
        # data[1]  - 'Static'
        # data[2]  - ['Location|Industrial', 'Architectural|Building']
        # data[3]  - 'XRAY_RESOURCES_PATH/models/props'
        # data[4]  - 'group1    |group3|group1    Static    XRAY_RESOURCES_PATH/models/props'
        m_items = cmds.textScrollList( self.m_textScrollList, query = True, allItems = True ) # get all items from scrollList
        cmds.textScrollList( self.m_textScrollList, edit = True, deselectAll = True )  # deselect ALL items from scrollList
        m_hashSelected = {} # create dictionary with selected items
        for obj,data in m_hash.iteritems():
            if ( data[0] ):
                if ( None != m_items and 0 != len( m_items ) ):
                    i = 0
                    while ( i != len( m_items ) ):
                        m_obj = m_items[i] # get label
                        m_nodeName = string.split( m_obj )[1]
                        if ( obj == m_nodeName ):
                            cmds.textScrollList( self.m_textScrollList, edit = True, selectItem = m_obj )
                            m_hashSelected[ obj ] = ( data[0], data[1], data[2], data[3], data[4] )
                            break
                        i += 1
        # get selected DATA from the node 
        m_hash = m_hashSelected
        if (0 != len( m_hash )):    
            #---------------------
            #   set object type
            #---------------------
            for obj,data in m_hash.iteritems():
                m_objType = data[1] # get Obj Type                                                          
                if ( 0 == m_objectType.count(m_objType) ):
                    m_objType = m_objectType[0] # if stored value don't exists in global array m_objectType
                else:
                    break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_objType != data[1] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = True )
                cmds.optionMenu( self.m_optionMenuObjType, edit = True, backgroundColor = m_multiselColor )  # multi selection color
                cmds.optionMenu( self.m_optionMenuObjType, edit = True, select = 1 ) # reset objType            
                cmds.optionMenu( self.m_optionMenuObjType, edit = True, enableBackground = False )   
            else:
                cmds.optionMenu( self.m_optionMenuObjType, edit = True, select = m_objectType.index(m_objType) + 1  )
            cmds.progressBar( self.m_progressControl, edit = True, step = 25 )  # PROGRESS 50%                    
            #----------------------
            #   set object tags
            #----------------------
            # create array for storing amount of tags in selection
            m_tag_count = {}
            for m_tag_checkbox in self.m_checkBoxTag:                
                m_tag_count [ cmds.checkBox( m_tag_checkbox, query = True, label = True ) ] = 0
            for obj,data in m_hash.iteritems():
                # print ("KEY: %s <==> SEL: %s TYPE: %s TAG: %s" % (obj, data[0], data[1], data[2]))
                # need check tags stored in the node for existing in dictionary m_categoryTree
                # if stored tag don't exists, ignore them
                if ( None != data[2] ): # have some tags
                    if ( 0 != len(data[2]) ):
                        for m_tag in data[2]:
                            # from 'Location|Industrial'
                            # get list of strings like this: ['Location', 'Industrial']                            
                            m_tag_list = getListFromRegExpr( m_tag )
                            # key   - (2, 'Location')
                            # value - ('Town', 'Village', 'Industrial', 'Military', 'Laboratory', 'Construction')
                            i = 0
                            while (i != len( m_categoryTree.keys()) ):
                                for key in m_categoryTree.keys():
                                    if ( m_categoryTree.has_key((i, m_tag_list[0])) ): # if key exists in global array m_categoryTree
                                        if (0 != m_categoryTree[(i, m_tag_list[0])].count(m_tag_list[1])): # if tag exists in global array
                                            m_tag_count[m_tag_list[1]] += 1 # increse amount by 1
                                        break
                                i += 1
            # lets set cheked tags
            for m_tag_checkbox in self.m_checkBoxTag:
                if ( len( m_hash ) == m_tag_count[ cmds.checkBox( m_tag_checkbox, query = True, label = True ) ] ):
                    cmds.checkBox( m_tag_checkbox, edit = True, value = True )
                    cmds.checkBox( m_tag_checkbox, edit = True, backgroundColor = m_checkBoxColor ) # all items have this tag
                    cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = True )  
                if ( ( m_tag_count[ cmds.checkBox( m_tag_checkbox, query = True, label = True ) ] >= 1)
                       and ( m_tag_count[ cmds.checkBox( m_tag_checkbox, query = True, label = True ) ] < len( m_hash ) ) ):
                    cmds.checkBox( m_tag_checkbox, edit = True, value = True )
                    cmds.checkBox( m_tag_checkbox, edit = True, backgroundColor = m_multiselColor ) # not all items have the same tag
                    cmds.checkBox( m_tag_checkbox, edit = True, enableBackground = True )  
            cmds.progressBar( self.m_progressControl, edit = True, step = 25 ) # PROGRESS 75%
            #---------------------
            #   set save dir
            #---------------------
            for obj,data in m_hash.iteritems():
                m_saveDir = data[3] # get save dir
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_saveDir != data[3] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it                
                cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = True ) 
                cmds.textField( self.m_textFieldSaveDir, edit = True, backgroundColor = m_multiselColor ) # multi selection color
                cmds.textField( self.m_textFieldSaveDir, edit = True, enableBackground = False )
            cmds.textField( self.m_textFieldSaveDir, edit = True, fileName = str(m_saveDir) ) # reset txtField
            cmds.progressBar( self.m_progressControl, edit = True, step = 25 ) # PROGRESS 100%
        cmds.progressBar( self.m_progressControl, edit = True, endProgress = True ) # PROGRESS END
        cmds.progressBar( self.m_progressControl, edit = True, visible = False )    # HIDE PROGRESS BAR