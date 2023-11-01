#-----------------------------------------------------------------
#	SCRIPT			exportXRayUIActor.py
#	AUTHOR			Zaitsev Evgeniy
#				    ev.zaitsev@gmail.com
#	DATE:			20 October 2010
#
#	DESCRIPTION:	Script with user intreface for export
#                   meshes into S.T.A.L.K.E.R. 2 project
#
#       VERSION:	    201102
#
#-----------------------------------------------------------------
import maya.cmds as cmds
from exportXRayContext         import *  # common functions
from exportXRayContextAnim     import *
from exportXRayContextSkeleton import *
from exportXRayContextSkin     import *
from exportXRayUI              import *  # common functions for UI

class m_actorWin( object ):
    """
    Main UI window class defination
    """
    def __init__( self, m_defaultLayout = 'Skeleton' ):
        """
        Initialize window
        """
        self.errorHandle = m_meshTrace() # error tracer      
        if cmds.window( m_actorProportiesGuiID, exists = True ):
            cmds.deleteUI( m_actorProportiesGuiID )
        self.m_window = cmds.window( m_actorProportiesGuiID, title = "Actor Properties", width = 700, height = 500 )
        m_textWidth = 110
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutMain = cmds.formLayout( parent = self.m_window )
        self.m_tabLayout = cmds.tabLayout( parent = self.m_formLayoutMain, 
                                           innerMarginWidth = 5, 
                                           innerMarginHeight = 5,
                                           changeCommand = lambda *args: self.uiMainCallback( "m_tabLayout", args ) ) 
        cmds.formLayout( self.m_formLayoutMain, edit = True,  
                                            attachForm = ( ( self.m_tabLayout, 'top', 0 ), 
                                                           ( self.m_tabLayout, 'left', 0 ), 
                                                           ( self.m_tabLayout, 'bottom', 0 ), 
                                                           ( self.m_tabLayout, 'right', 0 ) ) )
        #-----------------------------------------------------------------------------------------------------------
        # form SKELETON
        #-----------------------------------------------------------------------------------------------------------
        self.m_formSkeleton = cmds.formLayout( parent = self.m_tabLayout ) # main form for SKELETON
        self.m_paneLayoutSkel = cmds.paneLayout( parent = self.m_formSkeleton, configuration = 'vertical2' )
        cmds.formLayout( self.m_formSkeleton, edit = True,  
                                            attachForm = ( ( self.m_paneLayoutSkel, 'top', 0 ), 
                                                           ( self.m_paneLayoutSkel, 'left', 0 ), 
                                                           ( self.m_paneLayoutSkel, 'bottom', 0 ), 
                                                           ( self.m_paneLayoutSkel, 'right', 0 ) ) )        
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutLeftSkel = cmds.formLayout( parent = self.m_paneLayoutSkel )
        #-----------------------------------------------------------------------------------------------------------
        self.m_buttonAddItemSkel = cmds.button(    parent = self.m_formLayoutLeftSkel,
                                          label = 'Add Item',
                                          width = 80,
                                          command = lambda *args: self.uiSkeletonCallback( "m_buttonAddItemSkel", args ),
                                          annotation = "Select object in Outliner and press 'Add' button" )
        self.m_buttonDelItemSkel = cmds.button( parent = self.m_formLayoutLeftSkel,
                                       label = 'Delete',
                                       width = 80,
                                       command = lambda *args: self.uiSkeletonCallback( "m_buttonDelItemSkel", args ),
                                       annotation = "Remove selected items from srollList" )
        self.m_buttonRefreshItemSkel = cmds.button( parent = self.m_formLayoutLeftSkel, 
                                                 label = 'Refresh',
                                                 width = 80,
                                                 command = lambda *args: self.uiSkeletonCallback("m_buttonRefreshItemSkel", args),
                                                 annotation = "Refresh scrollList")
        self.m_textScrollListItemSkel = cmds.textScrollList( parent = self.m_formLayoutLeftSkel,
                                               selectCommand = lambda *args: self.uiSkeletonCallback( "m_textScrollListItemSkel", args ),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = True )
        self.m_textPopupMenuItemSkel = cmds.popupMenu( parent = self.m_textScrollListItemSkel )
        self.m_textPopupMenuItemRefreshSkel = cmds.menuItem( parent = self.m_textPopupMenuItemSkel, 
                                                       label = 'Refresh',
                                                       command = lambda *args: self.uiSkeletonCallback( "m_textPopupMenuItemRefreshSkel",args ))
        self.m_buttonExportAllItemSkel = cmds.button( parent = self.m_formLayoutLeftSkel, 
                                                label = 'Export All', 
                                                width = 80,
                                                command = lambda *args: self.uiSkeletonCallback( "m_buttonExportAllItemSkel", args ),
                                                annotation = "Export all items specified in scrollList")
        self.m_buttonExportSelItemSkel = cmds.button( parent = self.m_formLayoutLeftSkel, 
                                                label = 'Export Sel', 
                                                width = 80,
                                                command = lambda *args: self.uiSkeletonCallback( "m_buttonExportSelItemSkel", args ),
                                                annotation = "Export only selected items specified in scrollList")
        self.m_frameLayoutInfoItemSkel = cmds.frameLayout( parent = self.m_formLayoutLeftSkel, 
                                                     labelVisible = False,
                                                     marginHeight = 2,
                                                     marginWidth = 2, 
                                                     height = 28,
                                                     borderStyle = 'etchedIn' ) 
        self.m_progressControlSkel = cmds.progressBar( parent = self.m_frameLayoutInfoItemSkel, maxValue = 100 )
        cmds.formLayout( self.m_formLayoutLeftSkel, edit = True,
            attachForm =    [
                             ( self.m_buttonAddItemSkel,         'top',    2 ),
                             ( self.m_buttonAddItemSkel,         'left',   2 ), 
                             ( self.m_buttonDelItemSkel,         'top',    2 ),
                             ( self.m_buttonRefreshItemSkel,     'top',    2 ),
                             ( self.m_textScrollListItemSkel,    'left',   2 ),
                             ( self.m_textScrollListItemSkel,    'right',  2 ),
                             ( self.m_buttonExportAllItemSkel,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemSkel,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemSkel,   'right',  2 ),
                             ( self.m_frameLayoutInfoItemSkel,   'bottom', 2 )
                            ],
            attachControl = [    
                             ( self.m_buttonDelItemSkel,         'left',   2, self.m_buttonAddItemSkel ),
                             ( self.m_buttonRefreshItemSkel,     'left',   2, self.m_buttonDelItemSkel ),
                             ( self.m_textScrollListItemSkel,    'top',    2, self.m_buttonAddItemSkel ),
                             ( self.m_textScrollListItemSkel,    'bottom', 2, self.m_buttonExportAllItemSkel ),
                             ( self.m_buttonExportSelItemSkel,   'left',   2, self.m_buttonExportAllItemSkel),
                             ( self.m_buttonExportAllItemSkel,   'bottom', 2, self.m_frameLayoutInfoItemSkel ),
                             ( self.m_buttonExportSelItemSkel,   'bottom', 2, self.m_frameLayoutInfoItemSkel ),
                            ],
            attachNone =    [    
                             ( self.m_buttonAddItemSkel,       'right'  ),
                             ( self.m_buttonAddItemSkel,       'bottom' ),
                             ( self.m_buttonDelItemSkel,       'right'  ),
                             ( self.m_buttonDelItemSkel,       'bottom' ),
                             ( self.m_buttonRefreshItemSkel,   'right'  ),
                             ( self.m_buttonRefreshItemSkel,   'bottom' ),
                             ( self.m_frameLayoutInfoItemSkel, 'top'    ),
                             ( self.m_buttonExportAllItemSkel, 'right'  ),
                             ( self.m_buttonExportAllItemSkel, 'top'    ),
                             ( self.m_buttonExportSelItemSkel, 'right'  ),
                             ( self.m_buttonExportSelItemSkel, 'top'    ), 
                            ]) 
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutRightSkel = cmds.formLayout( parent = self.m_paneLayoutSkel )
        #-----------------------------------------------------------------------------------------------------------
        self.m_textSaveDirSkel = cmds.text( parent = self.m_formLayoutRightSkel, label = 'Save to', align = 'right', width = m_textWidth )
        self.m_textFieldSaveDirSkel = cmds.textField( parent = self.m_formLayoutRightSkel, 
                                            editable = False,
                                            changeCommand = lambda *args: self.uiSkeletonCallback( "m_textFieldSaveDirSkel", args ),
                                            width = 150,
                                            annotation = "Save to")
        self.m_symbolButtonSaveDirSkel = cmds.symbolButton( parent = self.m_formLayoutRightSkel, 
                                                  image = "navButtonBrowse.xpm", 
                                                  annotation = "Save to: Browse for the save folder",
                                                  command = lambda *args: self.uiSkeletonCallback( "m_symbolButtonSaveDirSkel", args ))
        cmds.formLayout( self.m_formLayoutRightSkel, edit = True,
            attachForm =    [    
                                 ( self.m_textSaveDirSkel,           'top',   30 ), 
                                 ( self.m_textSaveDirSkel,           'left',  2 ), 
                                 ( self.m_textFieldSaveDirSkel,      'top',   30 ), 
                                 ( self.m_symbolButtonSaveDirSkel,   'top',   30 ), 
                                 ( self.m_symbolButtonSaveDirSkel,   'right', 2 ),
                            ], 
            attachControl = [
                                 ( self.m_textFieldSaveDirSkel,      'right', 2, self.m_symbolButtonSaveDirSkel ),
                                 ( self.m_textFieldSaveDirSkel,      'left',  2, self.m_textSaveDirSkel ),
                            ],
            attachNone =    [    
                                 ( self.m_textSaveDirSkel,           'right'  ),
                                 ( self.m_textSaveDirSkel,           'bottom' ),                                 
                                 ( self.m_textFieldSaveDirSkel,      'bottom' ),
                                 ( self.m_symbolButtonSaveDirSkel,   'left'   ), 
                                 ( self.m_symbolButtonSaveDirSkel,   'bottom' ),
                            ])
        #-----------------------------------------------------------------------------------------------------------
        # form SKIN
        #-----------------------------------------------------------------------------------------------------------
        self.m_formSkin = cmds.formLayout( parent = self.m_tabLayout )
        self.m_paneLayoutSkin = cmds.paneLayout( parent = self.m_formSkin, configuration = 'vertical2' )
        cmds.formLayout( self.m_formSkin, edit = True,  
                                            attachForm = ( ( self.m_paneLayoutSkin, 'top', 0 ), 
                                                           ( self.m_paneLayoutSkin, 'left', 0 ), 
                                                           ( self.m_paneLayoutSkin, 'bottom', 0 ), 
                                                           ( self.m_paneLayoutSkin, 'right', 0 ) ) ) 
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutLeftSkin = cmds.formLayout( parent = self.m_paneLayoutSkin )
        #-----------------------------------------------------------------------------------------------------------
        self.m_buttonAddItemSkin = cmds.button(    parent = self.m_formLayoutLeftSkin,
                                          label = 'Add Item',
                                          width = 80,
                                          command = lambda *args: self.uiSkinCallback( "m_buttonAddItemSkin", args ),
                                          annotation = "Select object in Outliner and press 'Add' button" )
        self.m_buttonDelItemSkin = cmds.button( parent = self.m_formLayoutLeftSkin,
                                       label = 'Delete',
                                       width = 80,
                                       command = lambda *args: self.uiSkinCallback( "m_buttonDelItemSkin", args ),
                                       annotation = "Remove selected items from srollList" )
        self.m_buttonRefreshItemSkin = cmds.button( parent = self.m_formLayoutLeftSkin, 
                                                 label = 'Refresh',
                                                 width = 80,
                                                 command = lambda *args: self.uiSkinCallback("m_buttonRefreshItemSkin", args),
                                                 annotation = "Refresh scrollList")
        self.m_textScrollListItemSkin = cmds.textScrollList( parent = self.m_formLayoutLeftSkin,
                                               selectCommand = lambda *args: self.uiSkinCallback( "m_textScrollListItemSkin", args ),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = True )
        self.m_textPopupMenuItemSkin = cmds.popupMenu( parent = self.m_textScrollListItemSkin )
        self.m_textPopupMenuItemRefreshSkin = cmds.menuItem( parent = self.m_textPopupMenuItemSkin, 
                                                       label = 'Refresh',
                                                       command = lambda *args: self.uiSkinCallback( "m_textPopupMenuItemRefreshSkin",args ))
        self.m_buttonExportAllItemSkin = cmds.button( parent = self.m_formLayoutLeftSkin, 
                                                label = 'Export All', 
                                                width = 80,
                                                command = lambda *args: self.uiSkinCallback( "m_buttonExportAllItemSkin", args ),
                                                annotation = "Export all items specified in scrollList")
        self.m_buttonExportSelItemSkin = cmds.button( parent = self.m_formLayoutLeftSkin, 
                                                label = 'Export Sel', 
                                                width = 80,
                                                command = lambda *args: self.uiSkinCallback( "m_buttonExportSelItemSkin", args ),
                                                annotation = "Export only selected items specified in scrollList")
        self.m_frameLayoutInfoItemSkin = cmds.frameLayout( parent = self.m_formLayoutLeftSkin, 
                                                     labelVisible = False,
                                                     marginHeight = 2,
                                                     marginWidth = 2, 
                                                     height = 28,
                                                     borderStyle = 'etchedIn' ) 
        self.m_checkBoxNoDB = cmds.checkBox(  parent = self.m_formLayoutLeftSkin, 
											label = "No DB", 
                                            value = False )  													 
        self.m_progressControlSkin = cmds.progressBar( parent = self.m_frameLayoutInfoItemSkin, maxValue = 100 )
        cmds.formLayout( self.m_formLayoutLeftSkin, edit = True,
            attachForm =    [
                             ( self.m_buttonAddItemSkin,         'top',    2 ),
                             ( self.m_buttonAddItemSkin,         'left',   2 ), 
                             ( self.m_buttonDelItemSkin,         'top',    2 ),
                             ( self.m_buttonRefreshItemSkin,     'top',    2 ),
                             ( self.m_textScrollListItemSkin,    'left',   2 ),
                             ( self.m_textScrollListItemSkin,    'right',  2 ),
                             ( self.m_buttonExportAllItemSkin,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemSkin,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemSkin,   'right',  2 ),
                             ( self.m_frameLayoutInfoItemSkin,   'bottom', 2 )
                            ],
            attachControl = [    
                             ( self.m_buttonDelItemSkin,         'left',   2, self.m_buttonAddItemSkin ),
                             ( self.m_buttonRefreshItemSkin,     'left',   2, self.m_buttonDelItemSkin ),
                             ( self.m_textScrollListItemSkin,    'top',    2, self.m_buttonAddItemSkin ),
                             ( self.m_textScrollListItemSkin,    'bottom', 2, self.m_buttonExportAllItemSkin ),
                             ( self.m_buttonExportSelItemSkin,   'left',   2, self.m_buttonExportAllItemSkin),
                             ( self.m_buttonExportAllItemSkin,   'bottom', 2, self.m_frameLayoutInfoItemSkin ),
                             ( self.m_buttonExportSelItemSkin,   'bottom', 2, self.m_frameLayoutInfoItemSkin ),
                             (self.m_checkBoxNoDB, 'left', 5, self.m_buttonExportSelItemSkin),
							 (self.m_checkBoxNoDB, 'bottom', 5, self.m_frameLayoutInfoItemSkin),
                            ],
            attachNone =    [    
                             ( self.m_buttonAddItemSkin,       'right'  ),
                             ( self.m_buttonAddItemSkin,       'bottom' ),
                             ( self.m_buttonDelItemSkin,       'right'  ),
                             ( self.m_buttonDelItemSkin,       'bottom' ),
                             ( self.m_buttonRefreshItemSkin,   'right'  ),
                             ( self.m_buttonRefreshItemSkin,   'bottom' ),
                             ( self.m_frameLayoutInfoItemSkin, 'top'    ),
                             ( self.m_buttonExportAllItemSkin, 'right'  ),
                             ( self.m_buttonExportAllItemSkin, 'top'    ),
                             ( self.m_buttonExportSelItemSkin, 'right'  ),
                             ( self.m_buttonExportSelItemSkin, 'top'    ), 
							 (self.m_checkBoxNoDB, 'right'),
							 (self.m_checkBoxNoDB, 'top') ,
                            ]) 
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutRightSkin = cmds.formLayout( parent = self.m_paneLayoutSkin )
        #-----------------------------------------------------------------------------------------------------------          
        self.m_textSkeletonSkin = cmds.text( parent = self.m_formLayoutRightSkin, label = 'Skeleton', align = 'right', width = m_textWidth )
        self.m_textFieldSkeletonSkin = cmds.textField( parent = self.m_formLayoutRightSkin, 
                                                    editable = False,
                                                    changeCommand = lambda *args: self.uiSkinCallback( "m_textFieldSkeletonSkin", args ),
                                                    width = 150,
                                                    annotation = "Select skeleton from file")
        self.m_symbolButtonSkeletonSkin = cmds.symbolButton( parent = self.m_formLayoutRightSkin, 
                                                          image = "navButtonBrowse.xpm", 
                                                          annotation = "Select skeleton from file",
                                                          command = lambda *args: self.uiSkinCallback( "m_symbolButtonSkeletonSkin", args )) 
        self.m_textSaveDirSkin = cmds.text( parent = self.m_formLayoutRightSkin, label = 'Save to', align = 'right', width = m_textWidth )
        self.m_textFieldSaveDirSkin = cmds.textField( parent = self.m_formLayoutRightSkin, 
                                                    editable = False,
                                                    changeCommand = lambda *args: self.uiSkinCallback( "m_textFieldSaveDirSkin", args ),
                                                    width = 150,
                                                    annotation = "Save to" )
        self.m_symbolButtonSaveDirSkin = cmds.symbolButton( parent = self.m_formLayoutRightSkin, 
                                                          image = "navButtonBrowse.xpm", 
                                                          annotation = "Save to: Browse for the save folder",
                                                          command = lambda *args: self.uiSkinCallback( "m_symbolButtonSaveDirSkin", args ))
        cmds.formLayout( self.m_formLayoutRightSkin, edit = True,
            attachForm =    [    
                                 ( self.m_textSkeletonSkin,            'top',   30 ), 
                                 ( self.m_textSkeletonSkin,            'left',  2 ), 
                                 ( self.m_textFieldSkeletonSkin,       'top',   30 ), 
                                 ( self.m_symbolButtonSkeletonSkin,    'right', 2 ),
                                 ( self.m_symbolButtonSkeletonSkin,    'top',   30 ),
                                 ( self.m_textSaveDirSkin,             'left',  2 ),
                                 ( self.m_symbolButtonSaveDirSkin,     'right', 2 ),
                            ], 
            attachControl = [    
                                 ( self.m_textFieldSkeletonSkin,       'left',  2, self.m_textSkeletonSkin ),
                                 ( self.m_textFieldSkeletonSkin,       'right', 2, self.m_symbolButtonSkeletonSkin ),
                                 ( self.m_textSaveDirSkin,             'top',   2, self.m_textFieldSkeletonSkin ),
                                 ( self.m_textFieldSaveDirSkin,        'top',   2, self.m_textFieldSkeletonSkin ),
                                 ( self.m_textFieldSaveDirSkin,        'right', 2, self.m_symbolButtonSaveDirSkin ),
                                 ( self.m_textFieldSaveDirSkin,        'left',  2, self.m_textSaveDirSkin ),
                                 ( self.m_symbolButtonSaveDirSkin,     'top',   2, self.m_textFieldSkeletonSkin ), 
                            ],
            attachNone =    [    
                                 ( self.m_textSkeletonSkin,          'right'  ),
                                 ( self.m_textSkeletonSkin,          'bottom' ),
                                 ( self.m_textFieldSkeletonSkin,     'bottom' ), 
                                 ( self.m_symbolButtonSkeletonSkin,  'left'   ), 
                                 ( self.m_symbolButtonSkeletonSkin,  'bottom' ),
                                 ( self.m_textSaveDirSkin,           'right'  ),
                                 ( self.m_textSaveDirSkin,           'bottom' ),
                                 ( self.m_textFieldSaveDirSkin,      'bottom' ),
                                 ( self.m_symbolButtonSaveDirSkin,   'left'   ), 
                                 ( self.m_symbolButtonSaveDirSkin,   'bottom' ),
                            ])
        #-----------------------------------------------------------------------------------------------------------
        # form ANIMATION
        #-----------------------------------------------------------------------------------------------------------
        self.m_formAnimation = cmds.formLayout( parent = self.m_tabLayout )        
        self.m_paneLayoutAnim = cmds.paneLayout( parent = self.m_formAnimation, configuration = 'vertical3' )
        cmds.formLayout( self.m_formAnimation, edit = True,  
                                            attachForm = ( ( self.m_paneLayoutAnim, 'top', 0 ), 
                                                           ( self.m_paneLayoutAnim, 'left', 0 ), 
                                                           ( self.m_paneLayoutAnim, 'bottom', 0 ), 
                                                           ( self.m_paneLayoutAnim, 'right', 0 ) ) )
        #----------------------
        # Left Pane
        #----------------------
        self.m_formLayoutLeftAnim = cmds.formLayout( parent = self.m_paneLayoutAnim )
        # 3 buttons: Add, Del, Rename
        self.m_buttonAddSkinAnim = cmds.button( parent = self.m_formLayoutLeftAnim, 
                                      label = 'Add Mesh',
                                      width = 80,
                                      command = lambda *args:self.uiAnimationCallback( "m_buttonAddSkinAnim", args ),
                                      annotation = "Select skinned mesh in Outliner and press 'Add Mesh' button")
        self.m_buttonDelSkinAnim = cmds.button( parent = self.m_formLayoutLeftAnim,
                                      label = 'Delete Mesh',
                                      width = 80,
                                      command = lambda *args:self.uiAnimationCallback( "m_buttonDelSkinAnim", args ),
                                      annotation = "Remove mesh from list, warning: deleting with all animations specified in central scrollList")                 
        #----------------------
        # create scrollList
        self.m_textScrollListSkinAnim = cmds.textScrollList( parent = self.m_formLayoutLeftAnim,
                                               selectCommand = lambda *args:self.uiAnimationCallback( "m_textScrollListSkinAnim", args ),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = False )
        # create popupMenu
        self.m_textPopupMenuSkinAnim = cmds.popupMenu( parent = self.m_textScrollListSkinAnim )
        self.m_textPopupMenuSkinItemRefreshAnim = cmds.menuItem( parent = self.m_textPopupMenuSkinAnim, 
                                                   label = 'Refresh', 
                                                   command = lambda *args:self.uiAnimationCallback("m_textPopupMenuSkinItemRefreshAnim",args))        
        #----------------------
        # Info Frame
        self.m_frameLayoutInfoSkinAnim = cmds.frameLayout( parent = self.m_formLayoutLeftAnim, 
                                             labelVisible = False,
                                             marginHeight = 2,
                                             marginWidth = 2,                                              
                                             height = 28,
                                             borderStyle='etchedIn')
        # create progressBar
        self.m_progressControlSkinAnim = cmds.progressBar(parent = self.m_frameLayoutInfoSkinAnim, maxValue=100)                
        cmds.formLayout( self.m_formLayoutLeftAnim, edit = True,
            attachForm =    [
                             ( self.m_buttonAddSkinAnim,       'top',    2),
                             ( self.m_buttonAddSkinAnim,       'left',   2), 
                             ( self.m_buttonDelSkinAnim,       'top',    2),
                             ( self.m_textScrollListSkinAnim,  'left',   2),
                             ( self.m_textScrollListSkinAnim,  'right',  2), 
                             ( self.m_frameLayoutInfoSkinAnim, 'left',   2),
                             ( self.m_frameLayoutInfoSkinAnim, 'right',  2),
                             ( self.m_frameLayoutInfoSkinAnim, 'bottom', 2)
                            ],
            attachControl = [    
                             (self.m_buttonDelSkinAnim,      'left',   2, self.m_buttonAddSkinAnim),
                             (self.m_textScrollListSkinAnim, 'top',    2, self.m_buttonAddSkinAnim),
                             (self.m_textScrollListSkinAnim, 'bottom', 2, self.m_frameLayoutInfoSkinAnim)
                            ],
            attachNone =    [    
                             (self.m_buttonAddSkinAnim,       'right'  ),
                             (self.m_buttonAddSkinAnim,       'bottom' ),
                             (self.m_buttonDelSkinAnim,       'right'  ),
                             (self.m_buttonDelSkinAnim,       'bottom' ),
                             (self.m_frameLayoutInfoSkinAnim, 'top'    ),
                            ])
        #----------------------
        # Center Pane
        #----------------------
        self.m_formLayoutCenterAnim = cmds.formLayout( parent = self.m_paneLayoutAnim ) 
        # 2 buttons: Add, Del       
        self.m_buttonAddClipAnim = cmds.button( parent = self.m_formLayoutCenterAnim, 
                                      label = 'Add Anim', 
                                      width = 80,
                                      command = lambda *args:self.uiAnimationCallback( "m_buttonAddClipAnim", args ),
                                      annotation = 'Add animation clip')
        self.m_buttonDelClipAnim = cmds.button( parent = self.m_formLayoutCenterAnim,
                                      label = 'Delete Anim',
                                      width = 80,
                                      command = lambda *args:self.uiAnimationCallback( "m_buttonDelClipAnim", args ),
                                      annotation = 'Delete animation clip')
        self.m_buttonSetClipAnim = cmds.button( parent = self.m_formLayoutCenterAnim, 
                                        label = 'Set', 
                                        width = 80,
                                        command = lambda *args:self.uiAnimationCallback( "m_buttonSetClipAnim", args ),
                                        annotation = 'Set playbackOptions from selected animation') 
        # create scrollList 
        self.m_textScrollListClipAnim = cmds.textScrollList( parent = self.m_formLayoutCenterAnim,
                                               selectCommand = lambda *args:self.uiAnimationCallback( "m_textScrollListClipAnim", args ),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = True )
        # create popupMenu                                        
        self.m_textPopupMenuClipAnim = cmds.popupMenu( parent = self.m_textScrollListClipAnim )
        self.m_textPopupMenuAnimItemClipRefresh = cmds.menuItem( parent = self.m_textPopupMenuClipAnim, 
                                                   label = 'Refresh', 
                                                   command = lambda *args:self.uiAnimationCallback( "m_textPopupMenuAnimItemClipRefresh",args ))
        # 2 buttons: Export All, Export Sel
        self.m_buttonExportAllClipAnim = cmds.button( parent = self.m_formLayoutCenterAnim,
                                        label = 'Export All',
                                        width = 80,
                                        command = lambda *args:self.uiAnimationCallback( "m_buttonExportAllClipAnim", args ),
                                        annotation = 'Export all animations from slected mesh in left scrollList')
        self.m_buttonExportSelClipAnim = cmds.button( parent = self.m_formLayoutCenterAnim, 
                                        label = 'Export Sel',
                                        width = 80,
                                        command = lambda *args:self.uiAnimationCallback( "m_buttonExportSelClipAnim", args ),
                                        annotation = 'Export only selected animations from slected mesh in left scrollList')
        # Info Frame
        self.m_frameLayoutInfoClipAnim = cmds.frameLayout( parent = self.m_formLayoutCenterAnim, 
                                             labelVisible = False,
                                             marginHeight = 2,
                                             marginWidth = 2,
                                             height = 28, 
                                             borderStyle = 'etchedIn')
        # create progressBar
        self.m_progressControlClipAnim = cmds.progressBar( parent = self.m_frameLayoutInfoClipAnim, maxValue = 100 )
        cmds.formLayout( self.m_formLayoutCenterAnim, edit = True,
            attachForm =    [
                             ( self.m_buttonAddClipAnim,       'top',    2),
                             ( self.m_buttonAddClipAnim,       'left',   2),
                             ( self.m_buttonDelClipAnim,       'top',    2),
                             ( self.m_buttonSetClipAnim,       'top',    2),
                             ( self.m_textScrollListClipAnim,  'left',   2),
                             ( self.m_textScrollListClipAnim,  'right',  2), 
                             ( self.m_buttonExportAllClipAnim, 'left',   2),
                             ( self.m_frameLayoutInfoClipAnim, 'left',   2),
                             ( self.m_frameLayoutInfoClipAnim, 'right',  2),
                             ( self.m_frameLayoutInfoClipAnim, 'bottom', 2) 
                            ],
            attachControl = [    
                             ( self.m_buttonDelClipAnim,       'left',   2, self.m_buttonAddClipAnim ),
                             ( self.m_buttonSetClipAnim,       'left',   2, self.m_buttonDelClipAnim ),
                             ( self.m_textScrollListClipAnim,  'top',    2, self.m_buttonAddClipAnim ),
                             ( self.m_textScrollListClipAnim,  'bottom', 2, self.m_buttonExportAllClipAnim ),
                             ( self.m_buttonExportSelClipAnim, 'left',   2, self.m_buttonExportAllClipAnim ),
                             ( self.m_buttonExportAllClipAnim, 'bottom', 2, self.m_frameLayoutInfoClipAnim ),
                             ( self.m_buttonExportSelClipAnim, 'bottom', 2, self.m_frameLayoutInfoClipAnim ) 
                            ],
            attachNone =    [    
                             ( self.m_buttonAddClipAnim,       'right'),
                             ( self.m_buttonAddClipAnim,       'bottom'),
                             ( self.m_buttonDelClipAnim,       'right'),
                             ( self.m_buttonDelClipAnim,       'bottom'),
                             ( self.m_buttonSetClipAnim,       'right'),
                             ( self.m_buttonSetClipAnim,       'bottom'),
                             ( self.m_frameLayoutInfoClipAnim, 'top'),
                             ( self.m_buttonExportAllClipAnim, 'right'),
                             ( self.m_buttonExportAllClipAnim, 'top'),
                             ( self.m_buttonExportSelClipAnim, 'right'),
                             ( self.m_buttonExportSelClipAnim, 'top')
                            ])          
        #----------------------
        # Right Pane
        #----------------------
        self.m_formLayoutRightAnim = cmds.formLayout( parent = self.m_paneLayoutAnim )
        # add txt label 'Animation Name'
        self.m_textAnimNameAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Animation Name', align = 'right', width = m_textWidth )
        # add textField
        self.m_textFieldAnimNameAnim = cmds.textField( parent = self.m_formLayoutRightAnim, 
                                            editable = True, 
                                            changeCommand = lambda *args: self.uiAnimationCallback( "m_textFieldAnimNameAnim",args ),
                                            width = 150,
                                            annotation = "Enter name of animation, sample: 'idle'" )
        # add txt label 'Start Frame'
        self.m_textStartFrameAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Start Frame', align = 'right', width = 110 )
        self.m_floatFieldStartFrameAnim = cmds.floatField( parent = self.m_formLayoutRightAnim,
                                                minValue= -10000000, 
                                                maxValue = 10000000,
                                                step = 1.0 , 
                                                width = 50, 
                                                annotation = 'Start Animation',
                                                precision = 2,
                                                changeCommand = lambda *args: self.uiAnimationCallback("m_floatFieldStartFrameAnim",args))
        # add txt label 'End Frame' 
        self.m_textEndFrameAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'End Frame', align = 'right', width = m_textWidth )
        self.m_floatFieldEndFrameAnim = cmds.floatField(parent = self.m_formLayoutRightAnim, 
                                            minValue= -10000000, 
                                            maxValue = 10000000, 
                                            step = 1.0 , 
                                            width = 50, 
                                            annotation = 'End Animation',
                                            precision = 2,
                                            changeCommand = lambda *args: self.uiAnimationCallback( "m_floatFieldEndFrameAnim",args ) )
        # add txt label 'Local Coordinat Center'
        self.m_textLCCAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Local Coordinat Center', align = 'right', width = m_textWidth )
        # add textField
        self.m_textFieldLCCAnim = cmds.textField( parent = self.m_formLayoutRightAnim, 
                                            editable = True, 
                                            changeCommand = lambda *args: self.uiAnimationCallback( "m_textFieldLCCAnim",args ),
                                            width = 150,
                                            annotation = "Enter Transform name, sample: 'locator1'" )
        #----------------------
        # add txt label 'Save to'
        self.m_textSaveDirAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Save to', align = 'right', width = m_textWidth )
        # add textField
        self.m_textFieldSaveDirAnim = cmds.textField( parent = self.m_formLayoutRightAnim,
                                            editable = False,
                                            changeCommand = lambda *args: self.uiAnimationCallback( "m_textFieldSaveDirAnim",args ),
                                            width = 150,
                                            annotation = "Save to" )
        # add symbol button
        self.m_symbolButtonSaveDirAnim = cmds.symbolButton( parent = self.m_formLayoutRightAnim, 
                                                  image = "navButtonBrowse.xpm", 
                                                  annotation = "Save to: Browse for the save folder",
                                                  command = lambda *args: self.uiAnimationCallback( "m_symbolButtonSaveDirAnim",args ) ) 
        #----------------------
        # add txt label 'Time'
        self.m_textFPSAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Time', align = 'right', width = m_textWidth )
        self.m_optionMenuFPSAnim = cmds.optionMenu( parent = self.m_formLayoutRightAnim,
                                                      changeCommand = lambda *args: self.uiAnimationCallback("m_optionMenuFPSAnim",args),
                                                      annotation = "Select Frame Rate")
        for m_item in m_FPSlist:
            cmds.menuItem( parent = self.m_optionMenuFPSAnim, label = m_item )                                          
        #----------------------
        # add txt label 'Compression Quality'
        self.m_textEpsilonAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Compression Quality', align = 'right', width = m_textWidth )
        self.m_floatSliderGrpEpsilonAnim = cmds.floatSliderGrp( parent = self.m_formLayoutRightAnim,
                             field         = True, 
                             minValue      = m_anim_epsilonMin, 
                             maxValue      = m_anim_epsilonMax,
                             fieldMinValue = m_anim_epsilonMin,
                             fieldMaxValue = m_anim_epsilonMax,
                             value         = m_anim_epsilonDefault,
                             step          = 0.001,
                             sliderStep    = 0.001,
                             fieldStep     = 0.001, 
                             precision     = 5,
                             changeCommand = lambda *args: self.uiAnimationCallback( "m_floatSliderGrpEpsilonAnim",args ),
                             annotation     = "Select Compression Quality" )                 
        self.m_textEpsilonQualityBestAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Best', align = 'left' )
        self.m_textEpsilonQualityWorstAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Worst', align = 'right' )
        #----------------------
        # add txt label "Additive"
        self.m_textAdditiveAnim = cmds.text( parent = self.m_formLayoutRightAnim, label = 'Additive', align = 'right', width = m_textWidth )
        self.m_checkBoxIsAdditiveAnim = cmds.checkBox( parent = self.m_formLayoutRightAnim,
                                                  label = '',
                                                  value = False,
                                                  changeCommand = lambda *args: self.uiAnimationCallback( "m_checkBoxIsAdditiveAnim", args ),
                                                  annotation = "Is animation Additive" )
        #---------------------- 
        # add txt label "Base Animation"
        self.m_textBaseAnimationAnim =  cmds.text( parent = self.m_formLayoutRightAnim, label = 'Base Animation', align = 'left')  
        self.m_textFieldBaseAnimationAnim = cmds.textField( parent = self.m_formLayoutRightAnim,
                                                    editable = False,
                                                    changeCommand = lambda *args: self.uiAnimationCallback( "m_textFieldBaseAnimationAnim",args ),
                                                    width = 150,
                                                    annotation = "Select Base Animation from file" )
        self.m_symbolButtonBaseAnimationAnim = cmds.symbolButton( parent = self.m_formLayoutRightAnim,
                                                          image = "navButtonBrowse.xpm", 
                                                          annotation = "Select Base Animation from file",
                                                          command = lambda *args: self.uiAnimationCallback( "m_symbolButtonBaseAnimationAnim",args ) ) 
        #----------------------
        # add txt label "Animation Layers"
        self.m_textAnimationLayersAnim =  cmds.text( parent = self.m_formLayoutRightAnim, label = 'Animation Layers', align = 'right', width = m_textWidth )
        self.m_frameLayoutAnimLayerAnim = cmds.frameLayout(  parent = self.m_formLayoutRightAnim,
                                               labelVisible = False,
                                               marginHeight = 2, 
                                               marginWidth = 2,
                                               borderStyle = 'etchedIn' )
        # array of checkBox animation Layers
        self.m_checkBoxAnimLayerAnim = []
        cmds.formLayout( self.m_formLayoutRightAnim, edit = True,
            attachForm =    [    
                                 ( self.m_textAnimNameAnim,              'top',   30), 
                                 ( self.m_textAnimNameAnim,              'left',  2), 
                                 ( self.m_textFieldAnimNameAnim,         'top',   30), 
                                 ( self.m_textFieldAnimNameAnim,         'right', 2),
                                 ( self.m_textStartFrameAnim,            'left',  2),
                                 ( self.m_floatFieldStartFrameAnim,      'right', 2),
                                 ( self.m_textEndFrameAnim,              'left',  2),
                                 ( self.m_floatFieldEndFrameAnim,        'right', 2),
                                 ( self.m_textLCCAnim,                   'left',  2),
                                 ( self.m_textFieldLCCAnim,              'right', 2),
                                 ( self.m_textSaveDirAnim,               'left',  2),
                                 ( self.m_symbolButtonSaveDirAnim,       'right', 2),
                                 ( self.m_textFPSAnim,                   'left',  2),
                                 ( self.m_textEpsilonAnim,               'left',  2),
                                 ( self.m_floatSliderGrpEpsilonAnim,     'right', 2),
                                 ( self.m_textEpsilonQualityWorstAnim,   'right', 10),
                                 ( self.m_textAdditiveAnim,              'left',  2),
                                 ( self.m_symbolButtonBaseAnimationAnim, 'right', 2),
                                 ( self.m_textAnimationLayersAnim,       'left',  2),
                            ], 
            attachControl = [    
                                 ( self.m_textFieldAnimNameAnim,          'left',  2,  self.m_textAnimNameAnim ),
                                 ( self.m_textStartFrameAnim,             'top',   2,  self.m_textFieldAnimNameAnim ),
                                 ( self.m_floatFieldStartFrameAnim,       'left',  2,  self.m_textStartFrameAnim ),
                                 ( self.m_floatFieldStartFrameAnim,       'top',   2,  self.m_textFieldAnimNameAnim ), 
                                 ( self.m_textEndFrameAnim,               'top',   2,  self.m_floatFieldStartFrameAnim ),
                                 ( self.m_floatFieldEndFrameAnim,         'left',  2,  self.m_textEndFrameAnim ),
                                 ( self.m_floatFieldEndFrameAnim,         'top',   2,  self.m_floatFieldStartFrameAnim ),
                                 ( self.m_textLCCAnim,                    'top',   2,  self.m_floatFieldEndFrameAnim ),
                                 ( self.m_textFieldLCCAnim,               'left',  2,  self.m_textLCCAnim ),
                                 ( self.m_textFieldLCCAnim,               'top',   2,  self.m_floatFieldEndFrameAnim ),
                                 ( self.m_textSaveDirAnim,                'top',   2,  self.m_textFieldLCCAnim ), 
                                 ( self.m_textFieldSaveDirAnim,           'left',  2,  self.m_textSaveDirAnim ),
                                 ( self.m_textFieldSaveDirAnim,           'right', 2,  self.m_symbolButtonSaveDirAnim ),
                                 ( self.m_textFieldSaveDirAnim,           'top',   2,  self.m_textFieldLCCAnim ),
                                 ( self.m_symbolButtonSaveDirAnim,        'top',   2,  self.m_textFieldLCCAnim ),
                                 ( self.m_textFPSAnim,                    'top',   2,  self.m_textFieldSaveDirAnim ),
                                 ( self.m_optionMenuFPSAnim,              'left',  2,  self.m_textFPSAnim ),
                                 ( self.m_optionMenuFPSAnim,              'top',   2,  self.m_textFieldSaveDirAnim ),
                                 ( self.m_textEpsilonAnim,                'top',   2,  self.m_optionMenuFPSAnim ),
                                 ( self.m_floatSliderGrpEpsilonAnim,      'left',  2,  self.m_textEpsilonAnim ),
                                 ( self.m_floatSliderGrpEpsilonAnim,      'top',   2,  self.m_optionMenuFPSAnim ),
                                 ( self.m_textEpsilonQualityBestAnim,     'top',   0,  self.m_floatSliderGrpEpsilonAnim ),
                                 ( self.m_textEpsilonQualityBestAnim,     'left',  90, self.m_textEpsilonAnim ),
                                 ( self.m_textEpsilonQualityWorstAnim,    'top',   0,  self.m_floatSliderGrpEpsilonAnim ),
                                 ( self.m_textAdditiveAnim,               'top',   35, self.m_textEpsilonAnim ),
                                 ( self.m_checkBoxIsAdditiveAnim,         'left',  2,  self.m_textAdditiveAnim ),
                                 ( self.m_checkBoxIsAdditiveAnim,         'top',   35, self.m_textEpsilonAnim ), 
                                 ( self.m_textBaseAnimationAnim,          'left',  0,  self.m_checkBoxIsAdditiveAnim ),
                                 ( self.m_textBaseAnimationAnim,          'top',   35, self.m_textEpsilonAnim ),
                                 ( self.m_textFieldBaseAnimationAnim,     'left',  2,  self.m_textBaseAnimationAnim ),
                                 ( self.m_textFieldBaseAnimationAnim,     'top',   35, self.m_textEpsilonAnim ),
                                 ( self.m_textFieldBaseAnimationAnim,     'right', 2,  self.m_symbolButtonBaseAnimationAnim ), 
                                 ( self.m_symbolButtonBaseAnimationAnim,  'top',   35, self.m_textEpsilonAnim ), 
                                 ( self.m_textAnimationLayersAnim,        'top',   15, self.m_textAdditiveAnim ),
                                 ( self.m_frameLayoutAnimLayerAnim,       'left',  2,  self.m_textAnimationLayersAnim ),
                                 ( self.m_frameLayoutAnimLayerAnim,       'top',   15, self.m_textAdditiveAnim ),
                            ],
            attachNone =    [
                                 ( self.m_textAnimNameAnim,               'right'),
                                 ( self.m_textAnimNameAnim,               'bottom'),
                                 ( self.m_textFieldAnimNameAnim,          'bottom'),
                                 ( self.m_textStartFrameAnim,             'right'),
                                 ( self.m_textStartFrameAnim,             'bottom'),
                                 ( self.m_floatFieldStartFrameAnim,       'bottom'), 
                                 ( self.m_textEndFrameAnim,               'right'),
                                 ( self.m_textEndFrameAnim,               'bottom'),
                                 ( self.m_floatFieldEndFrameAnim,         'bottom'),
                                 ( self.m_textLCCAnim,                    'right'),
                                 ( self.m_textLCCAnim,                    'bottom'),
                                 ( self.m_textFieldLCCAnim,               'bottom'),
                                 ( self.m_textSaveDirAnim,                'right'),
                                 ( self.m_textSaveDirAnim,                'bottom'), 
                                 ( self.m_textFieldSaveDirAnim,           'bottom'),
                                 ( self.m_symbolButtonSaveDirAnim,        'left'),
                                 ( self.m_symbolButtonSaveDirAnim,        'bottom'),
                                 ( self.m_textFPSAnim,                    'right'),
                                 ( self.m_textFPSAnim,                    'bottom'),
                                 ( self.m_optionMenuFPSAnim,              'bottom'), 
                                 ( self.m_optionMenuFPSAnim,              'right'),
                                 ( self.m_textEpsilonAnim,                'right'),
                                 ( self.m_textEpsilonAnim,                'bottom'),
                                 ( self.m_floatSliderGrpEpsilonAnim,      'bottom'),
                                 ( self.m_textEpsilonQualityBestAnim,     'right'),
                                 ( self.m_textEpsilonQualityBestAnim,     'bottom'),
                                 ( self.m_textEpsilonQualityWorstAnim,    'left'),
                                 ( self.m_textEpsilonQualityWorstAnim,    'bottom'),
                                 ( self.m_textAdditiveAnim,               'right'),
                                 ( self.m_textAdditiveAnim,               'bottom'),   
                                 ( self.m_checkBoxIsAdditiveAnim,         'right'),
                                 ( self.m_checkBoxIsAdditiveAnim,         'bottom'),
                                 ( self.m_textBaseAnimationAnim,          'right'),
                                 ( self.m_textBaseAnimationAnim,          'bottom'),
                                 ( self.m_textFieldBaseAnimationAnim,     'bottom'),
                                 ( self.m_symbolButtonBaseAnimationAnim,  'left'),
                                 ( self.m_symbolButtonBaseAnimationAnim,  'bottom'), 
                                 ( self.m_textAnimationLayersAnim,        'right'),
                                 ( self.m_textAnimationLayersAnim,        'bottom'),
                                 ( self.m_frameLayoutAnimLayerAnim,       'right'),
                                 ( self.m_frameLayoutAnimLayerAnim,       'bottom'),
                            ])                    
        #-----------------------------------------------------------------------------------------------------------
        # form PHYSICS
        #-----------------------------------------------------------------------------------------------------------        
        self.m_formPhysics = cmds.formLayout( parent = self.m_tabLayout )
        self.m_paneLayoutPhysics = cmds.paneLayout( parent = self.m_formPhysics, configuration = 'vertical2' )
        cmds.formLayout( self.m_formPhysics, edit = True,  
                                            attachForm = ( ( self.m_paneLayoutPhysics, 'top', 0 ), 
                                                           ( self.m_paneLayoutPhysics, 'left', 0 ), 
                                                           ( self.m_paneLayoutPhysics, 'bottom', 0 ), 
                                                           ( self.m_paneLayoutPhysics, 'right', 0 ) ) )   
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutLeftPhysics = cmds.formLayout( parent = self.m_paneLayoutPhysics )
        #-----------------------------------------------------------------------------------------------------------
        self.m_buttonAddItemPhysics = cmds.button(    parent = self.m_formLayoutLeftPhysics,
                                          label = 'Add Item',
                                          width = 80,
                                          command = lambda *args: self.uiPhysicsCallback( "m_buttonAddItemPhysics", args ),
                                          annotation = "Select object in Outliner and press 'Add' button" )
        self.m_buttonDelItemPhysics = cmds.button( parent = self.m_formLayoutLeftPhysics,
                                       label = 'Delete',
                                       width = 80,
                                       command = lambda *args: self.uiPhysicsCallback( "m_buttonDelItemPhysics", args ),
                                       annotation = "Remove selected items from srollList" )
        self.m_buttonRefreshItemPhysics = cmds.button( parent = self.m_formLayoutLeftPhysics,
                                                 label = 'Refresh',
                                                 width = 80,
                                                 command = lambda *args: self.uiPhysicsCallback("m_buttonRefreshItemPhysics", args),
                                                 annotation = "Refresh scrollList")
        self.m_textScrollListItemPhysics = cmds.textScrollList( parent = self.m_formLayoutLeftPhysics,
                                               selectCommand = lambda *args: self.uiPhysicsCallback( "m_textScrollListItemPhysics", args ),
                                               font = 'smallFixedWidthFont',
                                               allowMultiSelection = True )
        self.m_textPopupMenuItemPhysics = cmds.popupMenu( parent = self.m_textScrollListItemPhysics )
        self.m_textPopupMenuItemRefreshPhysics = cmds.menuItem( parent = self.m_textPopupMenuItemPhysics, 
                                                       label = 'Refresh',
                                                       command = lambda *args: self.uiPhysicsCallback( "m_textPopupMenuItemRefreshPhysics",args ))
        self.m_buttonExportAllItemPhysics = cmds.button( parent = self.m_formLayoutLeftPhysics, 
                                                label = 'Export All', 
                                                width = 80,
                                                command = lambda *args: self.uiPhysicsCallback( "m_buttonExportAllItemPhysics", args ),
                                                annotation = "Export all items specified in scrollList")
        self.m_buttonExportSelItemPhysics = cmds.button( parent = self.m_formLayoutLeftPhysics, 
                                                label = 'Export Sel', 
                                                width = 80,
                                                command = lambda *args: self.uiPhysicsCallback( "m_buttonExportSelItemPhysics", args ),
                                                annotation = "Export only selected items specified in scrollList")
        self.m_frameLayoutInfoItemPhysics = cmds.frameLayout( parent = self.m_formLayoutLeftPhysics, 
                                                     labelVisible = False,
                                                     marginHeight = 2,
                                                     marginWidth = 2, 
                                                     height = 28,
                                                     borderStyle = 'etchedIn' ) 
        self.m_progressControlPhysics = cmds.progressBar( parent = self.m_frameLayoutInfoItemPhysics, maxValue = 100 )
        cmds.formLayout( self.m_formLayoutLeftPhysics, edit = True,
            attachForm =    [
                             ( self.m_buttonAddItemPhysics,         'top',    2 ),
                             ( self.m_buttonAddItemPhysics,         'left',   2 ), 
                             ( self.m_buttonDelItemPhysics,         'top',    2 ),
                             ( self.m_buttonRefreshItemPhysics,     'top',    2 ),
                             ( self.m_textScrollListItemPhysics,    'left',   2 ),
                             ( self.m_textScrollListItemPhysics,    'right',  2 ),
                             ( self.m_buttonExportAllItemPhysics,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemPhysics,   'left',   2 ),
                             ( self.m_frameLayoutInfoItemPhysics,   'right',  2 ),
                             ( self.m_frameLayoutInfoItemPhysics,   'bottom', 2 )
                            ],
            attachControl = [    
                             ( self.m_buttonDelItemPhysics,         'left',   2, self.m_buttonAddItemPhysics ),
                             ( self.m_buttonRefreshItemPhysics,     'left',   2, self.m_buttonDelItemPhysics ),
                             ( self.m_textScrollListItemPhysics,    'top',    2, self.m_buttonAddItemPhysics ),
                             ( self.m_textScrollListItemPhysics,    'bottom', 2, self.m_buttonExportAllItemPhysics ),
                             ( self.m_buttonExportSelItemPhysics,   'left',   2, self.m_buttonExportAllItemPhysics),
                             ( self.m_buttonExportAllItemPhysics,   'bottom', 2, self.m_frameLayoutInfoItemPhysics ),
                             ( self.m_buttonExportSelItemPhysics,   'bottom', 2, self.m_frameLayoutInfoItemPhysics ),
                            ],
            attachNone =    [    
                             ( self.m_buttonAddItemPhysics,       'right'  ),
                             ( self.m_buttonAddItemPhysics,       'bottom' ),
                             ( self.m_buttonDelItemPhysics,       'right'  ),
                             ( self.m_buttonDelItemPhysics,       'bottom' ),
                             ( self.m_buttonRefreshItemPhysics,   'right'  ),
                             ( self.m_buttonRefreshItemPhysics,   'bottom' ),
                             ( self.m_frameLayoutInfoItemPhysics, 'top'    ),
                             ( self.m_buttonExportAllItemPhysics, 'right'  ),
                             ( self.m_buttonExportAllItemPhysics, 'top'    ),
                             ( self.m_buttonExportSelItemPhysics, 'right'  ),
                             ( self.m_buttonExportSelItemPhysics, 'top'    ), 
                            ])         
        #-----------------------------------------------------------------------------------------------------------
        self.m_formLayoutRightPhysics = cmds.formLayout( parent = self.m_paneLayoutPhysics )
        #-----------------------------------------------------------------------------------------------------------
        self.m_textSaveDirPhysics = cmds.text( parent = self.m_formLayoutRightPhysics, label = 'Save to', align = 'right', width = m_textWidth )
        self.m_textFieldSaveDirPhysics = cmds.textField( parent = self.m_formLayoutRightPhysics,
                                            editable = False,
                                            changeCommand = lambda *args: self.uiPhysicsCallback( "m_textFieldSaveDirPhysics", args ),
                                            width = 150,
                                            annotation = "Save to")
        self.m_symbolButtonSaveDirPhysics = cmds.symbolButton( parent = self.m_formLayoutRightPhysics, 
                                                  image = "navButtonBrowse.xpm", 
                                                  annotation = "Save to: Browse for the save folder",
                                                  command = lambda *args: self.uiPhysicsCallback( "m_symbolButtonSaveDirPhysics", args ))
        cmds.formLayout( self.m_formLayoutRightPhysics, edit = True,
            attachForm =    [    
                                 ( self.m_textSaveDirPhysics,           'top',   30 ), 
                                 ( self.m_textSaveDirPhysics,           'left',  2 ), 
                                 ( self.m_textFieldSaveDirPhysics,      'top',   30 ), 
                                 ( self.m_symbolButtonSaveDirPhysics,   'top',   30 ), 
                                 ( self.m_symbolButtonSaveDirPhysics,   'right', 2 ),
                            ], 
            attachControl = [    
                                 ( self.m_textFieldSaveDirPhysics,        'right', 2, self.m_symbolButtonSaveDirPhysics ),
                                 ( self.m_textFieldSaveDirPhysics,        'left',  2, self.m_textSaveDirPhysics ),
                            ],
            attachNone =    [    
                                 ( self.m_textSaveDirPhysics,           'right'  ),
                                 ( self.m_textSaveDirPhysics,           'bottom' ),                                 
                                 ( self.m_textFieldSaveDirPhysics,      'bottom' ),
                                 ( self.m_symbolButtonSaveDirPhysics,   'left'   ), 
                                 ( self.m_symbolButtonSaveDirPhysics,   'bottom' ),
                            ])                
        #-----------------------------------------------------------------------------------------------------------
        # m_tabLayout SETUP
        #-----------------------------------------------------------------------------------------------------------
        cmds.tabLayout( self.m_tabLayout, edit = True, tabLabel = ( 
                                                    ( self.m_formSkeleton,     'Skeleton Properties'), 
                                                    ( self.m_formSkin,         'Skin Properties' ),
                                                    ( self.m_formAnimation,    'Animation Properties' ),
                                                    ( self.m_formPhysics,      'Physics Properties' )
                                                    ) )
        #-----------------------------------------------------------------------------------------------------------
        cmds.showWindow( m_actorProportiesGuiID )
        #-----------------------------------------------------------------------------------------------------------
        # initialize the nodes
        #
        self.m_nodeSkeleton = m_skeletonNode()
        self.m_nodeSkin     = m_skinNode()
        self.m_nodeAnims    = m_animNode()
        #
        #
        if   ( 'Skeleton' == m_defaultLayout ):
            cmds.tabLayout( self.m_tabLayout, edit = True, selectTabIndex = 1 )
            cmds.progressBar( self.m_progressControlSkel, edit = True, visible = False )
            self.uiSkeletonRefreshScrollList() # reload ScrollList            
        elif ( 'Skin' == m_defaultLayout ):
            cmds.tabLayout( self.m_tabLayout, edit = True, selectTabIndex = 2 )
            cmds.progressBar( self.m_progressControlSkin, edit = True, visible = False ) 
            self.uiSkinRefreshScrollList()     # reload ScrollList
        elif ( 'Animation' == m_defaultLayout ):
            cmds.tabLayout( self.m_tabLayout, edit = True, selectTabIndex = 3 )
            cmds.progressBar( self.m_progressControlSkinAnim, edit = True, visible = False ) 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, visible = False ) 
            self.uiAnimRefreshScrollList()  
        elif ( 'Physics' == m_defaultLayout ):
            cmds.tabLayout( self.m_tabLayout, edit = True, selectTabIndex = 4 )         

    def uiMainCallback( self, *args ):
        """
        Callback for Main UI
        """
        m_input = args[0]    
        #-------------------------------
        #     m_tabLayout
        #-------------------------------
        if ( 'm_tabLayout' == m_input ):
            m_curTabIndex = cmds.tabLayout( self.m_tabLayout, query = True, selectTabIndex = True )
            if   ( 1 == m_curTabIndex ):
                cmds.progressBar( self.m_progressControlSkel, edit = True, visible = False )
                self.uiSkeletonRefreshScrollList() 
            elif ( 2 == m_curTabIndex ):
                cmds.progressBar( self.m_progressControlSkin, edit = True, visible = False ) 
                self.uiSkinRefreshScrollList() 
            elif ( 3 == m_curTabIndex ):
                cmds.progressBar( self.m_progressControlSkinAnim, edit = True, visible = False ) 
                cmds.progressBar( self.m_progressControlClipAnim, edit = True, visible = False )             
                self.uiAnimRefreshScrollList() 
            elif ( 4 == m_curTabIndex ):
                print "Physics Properties"
                
    #------------------------------------------------------------------------------------------------------
    #     uiAnimationCallback
    #------------------------------------------------------------------------------------------------------
    def uiAnimationCallback( self, *args ):
        """
        Callback for Animation UI
        """   
        m_input = args[0]
        #-------------------------------
        #    m_checkBoxAnimLayerAnim
        #-------------------------------
        if ( 'm_checkBoxAnimLayerAnim' == m_input ):
            m_checkboxStates = {}
            for m_checkbox in self.m_checkBoxAnimLayerAnim:
                m_checkboxStates[ cmds.checkBox( m_checkbox, query = True, label = True )   ] = cmds.checkBox( m_checkbox, query = True, value = True )             
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )    # get selected items from scrollList as string array 
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID     = int( string.split( m_obj )[0] )                   # get clip ID 
                    self.m_nodeAnims.setClipAnimLayers( m_clipID, m_checkboxStates ) # write data to node  
            self.uiAnimLoadDataFromNode( ) # Refresh proporties at right side        
        #-------------------------------
        #     m_buttonAddSkinAnim
        #-------------------------------
        if ( 'm_buttonAddSkinAnim' == m_input ):
            m_objSelectedLong = cmds.ls( selection = True, long = True ) # get selected objects
            for m_obj in m_objSelectedLong:
                self.m_nodeAnims.addSkin( m_obj )    # add object to node
            self.uiAnimRefreshScrollList()           # reload ScrollList 
        #-------------------------------
        #     m_buttonDelSkinAnim
        #-------------------------------
        if ( 'm_buttonDelSkinAnim' == m_input ):
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]
                self.m_confirmDialogAnim = cmds.confirmDialog( title         = 'Confirm', 
                                                          message       = 'Are you sure?', 
                                                          button        = ['Yes','No'], 
                                                          defaultButton = 'No', 
                                                          cancelButton  = 'No', 
                                                          dismissString = 'No')
                if ( 'Yes' == self.m_confirmDialogAnim ):
                    self.m_nodeAnims.delSkin( m_skinNameSelected )  
                    self.uiAnimRefreshScrollList()  # reload ScrollList            
        #-------------------------------
        #     m_textScrollListSkinAnim
        #-------------------------------
        if ( 'm_textScrollListSkinAnim' == m_input ):            
            m_items_all = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, allItems = True ) # get all items from scrollList
            # reset selection state in the node
            for m_obj in m_items_all:
                self.m_nodeAnims.setSkinSelectionState( string.split(m_obj)[1], False ) 
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]
                self.m_nodeAnims.setSkinSelectionState( m_skinNameSelected, True)                                                   
            self.uiAnimRefreshScrollList( 'clips' )      
        #-------------------------------
        #     m_textPopupMenuSkinItemRefreshAnim
        #-------------------------------
        if ( 'm_textPopupMenuSkinItemRefreshAnim' == m_input ):
            self.uiAnimRefreshScrollList()                       
        #-------------------------------
        #     m_buttonAddClipAnim
        #-------------------------------
        if ( 'm_buttonAddClipAnim' == m_input ):
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'            
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]
                self.m_nodeAnims.addClip( m_skinNameSelected )
                self.uiAnimRefreshScrollList( 'clips' )
        #-------------------------------
        #     m_buttonDelClipAnim
        #-------------------------------
        if ( 'm_buttonDelClipAnim' == m_input ):
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'  
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]
                self.m_confirmDialogAnim = cmds.confirmDialog( title         = 'Confirm', 
                                                          message       = 'Are you sure?', 
                                                          button        = ['Yes','No'], 
                                                          defaultButton = 'No', 
                                                          cancelButton  = 'No', 
                                                          dismissString = 'No')
                if ( 'Yes' == self.m_confirmDialogAnim ): # check confirm                
                    m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )   # get selected items from scrollList as string array                    
                    if ( None != m_items and 0 != len( m_items ) ):
                        for m_obj in m_items:
                            m_clipID   = int( string.split( m_obj )[0] )              # get clip ID
                            self.m_nodeAnims.delClip( m_skinNameSelected, m_clipID )  # del clip
                    self.uiAnimRefreshScrollList( 'clips' )                 
        #-------------------------------
        #     m_buttonSetClipAnim
        #-------------------------------
        if ( 'm_buttonSetClipAnim' == m_input ):
            m_startFrame = float(cmds.playbackOptions( query = True, min = True ))
            m_endFrame   = float(cmds.playbackOptions( query = True, max = True ))
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'  
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]
                m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )  # get selected items from scrollList as string array
                if ( None != m_items ):
                    m_clipID   = int( string.split( m_items[0] )[0] )
                    m_hash = self.m_nodeAnims.iterItemsClip( 'sel', m_skinNameSelected )
                    """   
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
                     data[13] - '/default.clip'       - m_baseAnimation
                     data[14] - True                  - m_baseAnimationIsDefault  
                     data[15] - [ True, False, ... ]  - m_animLayers  
                    """                                      
                    for obj,data in m_hash.iteritems():
                        if ( m_clipID == obj ):
                            m_startFrame  = data[3]
                            m_endFrame    = data[4]
                            m_FPS         = data[10]
                OpenMaya.MTime.setUIUnit(m_FPS)
                cmds.playbackOptions( edit = True, min = m_startFrame )
                cmds.playbackOptions( edit = True, max = m_endFrame )  
        #-------------------------------
        #     m_textScrollListClipAnim
        #-------------------------------
        if ( 'm_textScrollListClipAnim' == m_input ):
            m_items_all = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, allItems = True ) # get all items from scrollList 
            # reset selection state in the node
            for m_obj in m_items_all:
                self.m_nodeAnims.setClipSelectionState( int(string.split( m_obj )[0] ), False )                
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )  # get list of selected 'clips'    
            if ( None != m_items ):
                for m_obj in m_items:
                    self.m_nodeAnims.setClipSelectionState( int(string.split(m_obj)[0]), True )        
            self.uiAnimLoadDataFromNode( ) 
        #-------------------------------
        #     m_textPopupMenuAnimItemClipRefresh
        #-------------------------------
        if ( 'm_textPopupMenuAnimItemClipRefresh' == m_input ):
            self.uiAnimRefreshScrollList( 'clips' ) 
        #-------------------------------
        #     m_buttonExportAllClipAnim
        #-------------------------------
        if ( 'm_buttonExportAllClipAnim' == m_input ):
            self.m_nodeAnims.exportDataALL()
        #-------------------------------
        #     m_buttonExportSelClipAnim
        #-------------------------------
        if ( 'm_buttonExportSelClipAnim' == m_input ):
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'  
            if ( None != m_skinNameSelected ):
                m_skinNameSelected = string.split( m_skinNameSelected[0] )[1]        
                self.m_nodeAnims.exportDataOnlySEL( m_skinNameSelected )        
        #-------------------------------
        #     m_textFieldAnimNameAnim
        #-------------------------------
        if ( 'm_textFieldAnimNameAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID   = int( string.split(m_obj)[0] )           # get clip ID                    
                    m_animName = cmds.textField( self.m_textFieldAnimNameAnim, query = True, fileName = True ) # get new aniamtion name                                         
                    self.m_nodeAnims.setClipName( m_clipID, m_animName )        # write data to node            
            self.uiAnimRefreshScrollList( 'clips' )                  
        #-------------------------------
        #     m_floatFieldStartFrameAnim
        #-------------------------------
        if ( 'm_floatFieldStartFrameAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID   = int( string.split(m_obj)[0] )                                                     # get clip ID 
                    m_startFrame = cmds.floatField( self.m_floatFieldStartFrameAnim, query = True, value = True )  # get Start frame                                                
                    self.m_nodeAnims.setClipStartFrame( m_clipID, m_startFrame )                                   # write data to node          
            self.uiAnimRefreshScrollList( 'clips' )            
        #-------------------------------
        #     m_floatFieldEndFrameAnim
        #-------------------------------
        if ( 'm_floatFieldEndFrameAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID   = int( string.split(m_obj)[0] )                                                 # get clip ID 
                    m_endFrame = cmds.floatField( self.m_floatFieldEndFrameAnim, query = True, value = True )  # get End frame                                                                    
                    self.m_nodeAnims.setClipEndFrame( m_clipID, m_endFrame )                                   # write data to node             
            self.uiAnimRefreshScrollList( 'clips' )              
        #-------------------------------
        #     m_textFieldLCCAnim
        #-------------------------------
        if ( 'm_textFieldLCCAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID   = int( string.split(m_obj)[0] )                                                    # get clip ID              
                    m_localCoordCenter = cmds.textField( self.m_textFieldLCCAnim, query = True, fileName = True ) # any kTransform                                        
                    self.m_nodeAnims.setClipLocalCoordCenter( m_clipID, m_localCoordCenter )                      # write data to node             
            self.uiAnimRefreshScrollList( 'clips' )            
        #-------------------------------
        #     m_textFieldSaveDirAnim
        #-------------------------------
        if ( 'm_textFieldSaveDirAnim' == m_input ):
            print "m_textFieldSaveDirAnim"                 
        #-------------------------------
        #     m_symbolButtonSaveDirAnim
        #-------------------------------
        if ( 'm_symbolButtonSaveDirAnim' == m_input ):
            if (None == getRealXrayResourcesPath( 'animations/single' )): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            else:  
                self.m_fileDialog2SaveDirAnim = cmds.fileDialog2(fileFilter  ='Folder',
                                                        dialogStyle = 2, 
                                                        fileMode    = 3, 
                                                        caption     = 'Save file to ...', 
                                                        startingDirectory = getRealXrayResourcesPath( 'animations/single' ), 
                                                        okCaption         = 'Set Dir')
                if (None == self.m_fileDialog2SaveDirAnim):
                    pass
                else:
                    m_saveDir = self.m_fileDialog2SaveDirAnim[0]
                    m_saveDir = m_saveDir.lower()
                    m_realPath = getRealXrayResourcesPath( 'animations/single' )
                    # if 'x:/resources/exported/animations' don't exist, redefine m_saveDir
                    if (-1 == m_saveDir.find(m_realPath)):
                        m_saveDir = m_realPath
                    m_saveDir   = m_saveDir.replace(m_realPath, '' , 1)
                    m_saveDir   = str( m_xrayResourcesPathAnims + m_saveDir )
                    m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True ) # get selected items from scrollList as string array                    
                    if (None != m_items and 0 != len(m_items) ):
                        for m_obj in m_items:
                            m_clipID  = int(string.split(m_obj)[0])                  # get clip ID                                         
                            self.m_nodeAnims.setClipSaveDir(m_clipID, m_saveDir)     # write data to node                                                                         
            self.uiAnimRefreshScrollList( 'clips' )           
        #-------------------------------
        #     m_optionMenuFPSAnim
        #-------------------------------
        if ( 'm_optionMenuFPSAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID  = int(string.split(m_obj)[0])                                          # get clip ID    
                    m_FPS = cmds.optionMenu( self.m_optionMenuFPSAnim, query = True, select = True )  # get OpenMaya.MTime.unit() as int index
                    self.m_nodeAnims.setClipFPS( m_clipID, m_FPS )         # write data to node         
            self.uiAnimRefreshScrollList( 'clips' )           
        #-------------------------------
        #     m_floatSliderGrpEpsilonAnim
        #-------------------------------
        if ( 'm_floatSliderGrpEpsilonAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID  = int(string.split(m_obj)[0])  # get clip ID                                                               
                    m_epsilon = cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, query = True, value = True ) # get Epsilon 
                    self.m_nodeAnims.setClipEpsilon( m_clipID, m_epsilon )         # write data to node         
            self.uiAnimRefreshScrollList( 'clips' )             
        #-------------------------------
        #     m_checkBoxIsAdditiveAnim
        #-------------------------------
        if ( 'm_checkBoxIsAdditiveAnim' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )
            if ( None != m_items and 0 != len(m_items) ):
                for m_obj in m_items:
                    m_clipID  = int(string.split(m_obj)[0])  # get clip ID 
                    m_isAdditive = cmds.checkBox( self.m_checkBoxIsAdditiveAnim, query = True, value = True ) # get Is Additive                                                       
                    self.m_nodeAnims.setClipIsAdditive( m_clipID, m_isAdditive ) # write data to node
            self.uiAnimRefreshScrollList( 'clips' )            
        #-------------------------------
        #     m_textFieldBaseAnimationAnim
        #-------------------------------
        if ( 'm_textFieldBaseAnimationAnim' == m_input ):
            print "m_textFieldBaseAnimationAnim"
        #-------------------------------
        #     m_symbolButtonBaseAnimationAnim
        #-------------------------------
        if ( 'm_symbolButtonBaseAnimationAnim' == m_input ):
            if (None == getRealXrayResourcesPath( 'animations/single' )): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            else:  
                self.m_fileDialog2BaseAnimationAnim = cmds.fileDialog2(
                                                        fileFilter = "All Files (*.*)",
                                                        dialogStyle = 2, 
                                                        fileMode    = 1, 
                                                        caption     = 'Select Base Animation', 
                                                        startingDirectory = getRealXrayResourcesPath( 'animations/single' ),
                                                        okCaption   = 'Select' )
                if (None == self.m_fileDialog2BaseAnimationAnim):
                    pass
                else:
                    m_baseAnimation = self.m_fileDialog2BaseAnimationAnim[0]
                    m_baseAnimation = m_baseAnimation.lower()
                    m_baseAnimation = os.path.splitext( m_baseAnimation )[0] # cut file extension                    
                    m_realPath = getRealXrayResourcesPath( 'animations/single' )
                    # if 'x:/resources/exported/animations' don't exist, redefine m_baseAnimation
                    if (-1 == m_baseAnimation.find( m_realPath )):
                        m_baseAnimation = m_realPath + string.replace( m_anim_baseAnimationDefault, m_xrayResourcesPathAnims, '', 1 )
                    # write to node :
                    m_baseAnimation = m_xrayResourcesPathAnims + m_baseAnimation.replace( m_realPath, '', 1 )
                    # -------------------------------------------------    
                    m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, selectItem = True )                    
                    if ( None != m_items and 0 != len(m_items) ):
                        for m_obj in m_items:
                            m_clipID  = int(string.split(m_obj)[0])                           # get clip ID                                         
                            self.m_nodeAnims.setClipBaseAnimation( m_clipID, m_baseAnimation )     # write data to node                                                           
            self.uiAnimRefreshScrollList( 'clips' )
        
    def uiAnimRefreshScrollList( self, m_stat = 'skins' ):
        """
        Refresh self.m_textScrollListSkinAnim
                self.m_textScrollListClipAnim
        INPUT:  m_stat = 'skins' or 'clips'
                'skins'  - refresh self.m_textScrollListSkinAnim and self.m_textScrollListClipAnim
                'clips'  - refresh only self.m_textScrollListClipAnim
        """  
        if ( 'skins' == m_stat ):    
            # read data from node
            m_hash = self.m_nodeAnims.iterItemsSkin()
            """ 
            obj     - '|group1|stalker'
            data[0] - False
            data[1] - 'stalker     |group1|stalker'
            """
            cmds.textScrollList( self.m_textScrollListSkinAnim, edit = True, removeAll = True ) # clear ScrollList
            for obj,data in m_hash.iteritems():
                cmds.textScrollList( self.m_textScrollListSkinAnim, edit = True, append = [ data[1] ] ) 
                if ( data[0] ):
                    cmds.textScrollList( self.m_textScrollListSkinAnim, edit = True, selectItem = data[1] ) 
            cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, removeAll = True ) # clear ScrollList                          
            # reload ScrollList 'clips' for selected Mesh 
            self.uiAnimRefreshScrollList( 'clips' ) 
        if ( 'clips' == m_stat ):
            m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'            
            if ( None == m_skinNameSelected ):
                self.uiAnimLoadDataFromNode( )
                return None            
            m_skinName = string.split( m_skinNameSelected[0] )[1]                
            # read data from node
            m_hash = self.m_nodeAnims.iterItemsClip( 'sel', m_skinName )
            """   
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
             data[13] - '/default.clip'       - m_baseAnimation
             data[14] - True                  - m_baseAnimationIsDefault  
             data[15] - [ True, False, ... ]  - m_animLayers  
            """  
            cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, removeAll = True ) # clear ScrollList
            for obj,data in m_hash.iteritems():
                cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, append = [ data[7] ] ) 
                if ( data[1] ):
                    cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, selectItem = data[7] )
            # get selected DATA from the node
            self.uiAnimLoadDataFromNode( )

    def uiAnimLoadDataFromNode( self ):
        """
        Load data to scrollLists from the node
        """    
        # reset Animation Clip name
        cmds.textField( self.m_textFieldAnimNameAnim, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldAnimNameAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldAnimNameAnim, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldAnimNameAnim, edit = True, enableBackground = False )             
        # reset Start Frame
        cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, enableBackground = True )
        cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, value = float( cmds.playbackOptions( query = True, min = True ) ) ) 
        cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, enableBackground = False )
        # reset End Frame
        cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, enableBackground = True )
        cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, value = float( cmds.playbackOptions( query = True, max = True ) ) ) 
        cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, enableBackground = False )
        # reset Local Coordinat Center
        cmds.textField( self.m_textFieldLCCAnim, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldLCCAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldLCCAnim, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldLCCAnim, edit = True, enableBackground = False )                   
        # reset Save Dir
        cmds.textField( self.m_textFieldSaveDirAnim, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldSaveDirAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldSaveDirAnim, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldSaveDirAnim, edit = True, enableBackground = False )     
        # reset FPS        
        cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, enableBackground = True )
        cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        m_FPS = OpenMaya.MTime_uiUnit()           # get current ui Unit
        cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, select = m_FPS ) # set this Unit as Default
        cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, enableBackground = False )
        # reset Epsilon
        cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, enableBackground = True )
        cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, value = m_anim_epsilonDefault ) 
        cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, enableBackground = False )        
        # reset Is Additive
        cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, enableBackground = True )   
        cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, backgroundColor = m_checkBoxColor )            
        cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, value = False )
        # reset BaseAnimation
        cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, enableBackground = False )  
        cmds.text(         self.m_textBaseAnimationAnim, edit = True, visible = False ) 
        cmds.textField(    self.m_textFieldBaseAnimationAnim, edit = True, visible = False ) 
        cmds.symbolButton( self.m_symbolButtonBaseAnimationAnim, edit = True, visible = False ) 
        # reset AnimationLayers
        # remove old buttons
        for m_ind in range( len( self.m_checkBoxAnimLayerAnim ) ):
            cmds.deleteUI( self.m_checkBoxAnimLayerAnim.pop(), control = True )                 
        # insert new
        m_anim_layers = m_animLayers() # class for Animation Layer nodes
        m_obj_array = m_anim_layers.get() # get all animation layers currently stored in maya file         
        for m_ind in range( m_obj_array.length() - 1 , -1 , -1 ):
            self.m_checkBoxAnimLayerAnim.append( cmds.checkBox( parent = self.m_frameLayoutAnimLayerAnim,
                                                  label = getNodeNameFromeMObject( m_obj_array[ m_ind ] ), 
                                                  value = False,
                                                  changeCommand = lambda *args: self.uiAnimationCallback( "m_checkBoxAnimLayerAnim", args ) ) )
        for m_checkbox in self.m_checkBoxAnimLayerAnim:
            cmds.checkBox( m_checkbox, edit = True, enableBackground = True )
            cmds.checkBox( m_checkbox, edit = True, backgroundColor = m_checkBoxColor ) 
            cmds.checkBox( m_checkbox, edit = True, enableBackground = False )
        #---------------------------------------------
        # synchronize animation layers in the node
        self.m_nodeAnims.animLayerNodesSynchronize()
        # read data from node
        m_skinNameSelected = cmds.textScrollList( self.m_textScrollListSkinAnim, query = True, selectItem = True )   # get list of selected 'skins'            
        if ( None == m_skinNameSelected ):
            return None
        m_skinName = string.split( m_skinNameSelected[0] )[1]                        
        m_hash = self.m_nodeAnims.iterItemsClip( 'sel', m_skinName )
        """   
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
         data[13] - '/default.clip'       - m_baseAnimation
         data[14] - True                  - m_baseAnimationIsDefault  
         data[15] - [ True, False, ... ]  - m_animLayers
        """         
        m_items = cmds.textScrollList( self.m_textScrollListClipAnim, query = True, allItems = True )    # get all items from scrollList
        cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, deselectAll = True )   # deselect ALL items from scrollList                          
        m_hashSelected = {} # create dictionary with selected items
        for obj,data in m_hash.iteritems():
            if ( data[1] ):
                if ( None != m_items and 0 != len( m_items ) ):
                    i = 0
                    while ( i != len( m_items ) ):
                        m_obj = m_items[i] # get label
                        m_clipID    = int( string.split( m_obj )[0] )
                        if ( obj == m_clipID ):
                            cmds.textScrollList( self.m_textScrollListClipAnim, edit = True, selectItem = m_obj )
                            m_hashSelected[obj] = ( data[0], data[1], data[2], data[3], data[4], data[5], data[6], 
                                                data[7], data[8], data[9], data[10], data[11], data[12], data[13],
                                                data[14], data[15] )
                            break
                        i += 1
        #---------------------------------------------  
        cmds.progressBar( self.m_progressControlClipAnim, edit = True, visible = True )        # SHOW PROGRESS BAR
        cmds.progressBar( self.m_progressControlClipAnim, edit = True, beginProgress = True )  # PROGRESS BEGIN              
        # get selected DATA from the node 
        m_hash = m_hashSelected
        if ( 0 != len( m_hash ) ):
            #---------------------
            #   set animation name
            #---------------------
            for obj,data in m_hash.iteritems():
                m_animName = data[2] # get name of animation
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_animName != data[2] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldAnimNameAnim, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldAnimNameAnim, edit = True, backgroundColor = m_multiselColor ) # multi selection color                      
                cmds.textField( self.m_textFieldAnimNameAnim, edit = True, enableBackground = False )  
            cmds.textField( self.m_textFieldAnimNameAnim, edit = True, fileName = str(m_animName) )  
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10% 
            #---------------------
            #   set start frame
            #---------------------                
            for obj,data in m_hash.iteritems():
                m_startFrame = data[3] # get start frame
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_startFrame != data[3] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, enableBackground = True )
                cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, backgroundColor = m_multiselColor )                 
                cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, enableBackground = False )            
            cmds.floatField( self.m_floatFieldStartFrameAnim, edit = True, value = float( m_startFrame ) )                 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%                   
            #---------------------
            #   set end frame
            #---------------------                
            for obj,data in m_hash.iteritems():
                m_endFrame = data[4] # get end frame                                                         
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_endFrame != data[4] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, enableBackground = True )
                cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, backgroundColor = m_multiselColor )                 
                cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, enableBackground = False )            
            cmds.floatField( self.m_floatFieldEndFrameAnim, edit = True, value = float( m_endFrame ) ) 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #---------------------
            #   set local coordinat center
            #---------------------                
            for obj,data in m_hash.iteritems():
                m_localCoordCenter = data[5] # get lcc                                                         
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_localCoordCenter != data[5] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldLCCAnim, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldLCCAnim, edit = True, backgroundColor = m_multiselColor )                    
                cmds.textField( self.m_textFieldLCCAnim, edit = True, enableBackground = False )               
            cmds.textField( self.m_textFieldLCCAnim, edit = True, fileName = str( m_localCoordCenter ) )     
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #----------------------------------
            #   set save dir
            #----------------------------------
            for obj,data in m_hash.iteritems():
                m_saveDir = data[6] # get save dir
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_saveDir != data[6] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldSaveDirAnim, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldSaveDirAnim, edit = True, backgroundColor = m_multiselColor )                      
                cmds.textField( self.m_textFieldSaveDirAnim, edit = True, enableBackground = False )                
            cmds.textField( self.m_textFieldSaveDirAnim, edit = True, fileName = str( m_saveDir ) )   
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #----------------------------------
            #   set FPS
            #---------------------------------- 
            for obj,data in m_hash.iteritems():
                m_FPS = data[10] # get FPS                                                         
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_FPS != data[10] ):
                    m_hasDifference = True # find difference
                    break
            if (m_hasDifference): # if we have differences in multiselection let's color it
                cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, enableBackground = True )
                cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, backgroundColor = m_multiselColor )                 
                cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, enableBackground = False )            
            cmds.optionMenu( self.m_optionMenuFPSAnim, edit = True, select = m_FPS ) 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10% 
            #----------------------------------
            #   set Epsilon
            #----------------------------------
            for obj,data in m_hash.iteritems():
                m_epsilon = data[11] # get Epsilon                                                         
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_epsilon != data[11] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, enableBackground = True )
                cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, backgroundColor = m_multiselColor )                 
                cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, enableBackground = False ) 
            cmds.floatSliderGrp( self.m_floatSliderGrpEpsilonAnim, edit = True, value = m_epsilon ) 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10% 
            #----------------------------------
            #   set isAdditive
            #----------------------------------
            for obj,data in m_hash.iteritems():
                m_isAdditive = data[12] # get isAdditive
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_isAdditive != data[12] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, enableBackground = True )
                cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, backgroundColor = m_multiselColor )
            cmds.checkBox( self.m_checkBoxIsAdditiveAnim, edit = True, value = m_isAdditive )    
            cmds.text(         self.m_textBaseAnimationAnim, edit = True,          visible = m_isAdditive )
            cmds.textField(    self.m_textFieldBaseAnimationAnim, edit = True,     visible = m_isAdditive )  
            cmds.symbolButton( self.m_symbolButtonBaseAnimationAnim, edit = True,  visible = m_isAdditive ) 
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #----------------------------------
            #   set base animation
            #----------------------------------
            for obj,data in m_hash.iteritems():
                m_baseAnimation = data[13] # get baseAnimation
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_baseAnimation != data[13] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, backgroundColor = m_multiselColor )           
                cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, enableBackground = False )  
            cmds.textField( self.m_textFieldBaseAnimationAnim, edit = True, fileName = str( m_baseAnimation ) )      
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #----------------------------------
            #   set checkBoxAnimLayer
            #----------------------------------                 
            m_anim_count  = [] # array for storing amount of selection AnimLayers       
            # lets count them all                   
            m_anim_layers = data[15][0] # get array of bool
            for m_layer in range( len( m_anim_layers ) ):
                m_anim_count.append( 0 )
            for obj,data in m_hash.iteritems():
                m_anim_layers = data[15][0] # get array of bool
                for m_layer in range( len( m_anim_layers ) ):
                    if ( m_anim_layers[ m_layer ] ):
                        m_anim_count[ m_layer ] += 1
                    #print " %s %s %s" %( m_layer, m_anim_layers[ m_layer ], m_anim_count[ m_layer ] ) 
            for m_checkbox in self.m_checkBoxAnimLayerAnim:
                m_current_anim_layer_index = self.m_nodeAnims.animLayerNodesFindIndex( cmds.checkBox( m_checkbox, query = True, label = True ) ) # return (2, 5)
                
                if ( m_current_anim_layer_index ):
                    m_count = m_anim_count[ m_current_anim_layer_index[0] ]
                    #m_index = m_current_anim_layer_index[0]
                    #print "%s %s" %( m_count, m_index )
                    if   ( len( m_hash ) == m_count ):
                        cmds.checkBox( m_checkbox, edit = True, value = True ) 
                        cmds.checkBox( m_checkbox, edit = True, enableBackground = True )
                        cmds.checkBox( m_checkbox, edit = True, backgroundColor = m_checkBoxColor ) 
                    elif ( ( m_count >= 1 ) and ( m_count < len( m_hash ) ) ):
                        cmds.checkBox( m_checkbox, edit = True, value = True ) 
                        # lets color different labels
                        cmds.checkBox( m_checkbox, edit = True, enableBackground = True )
                        cmds.checkBox( m_checkbox, edit = True, backgroundColor = m_multiselColor )                         
            cmds.progressBar( self.m_progressControlClipAnim, edit = True, step = 10 ) # PROGRESS 10%
            #----------------------------------- 
        cmds.progressBar( self.m_progressControlClipAnim, edit = True, endProgress = True ) # PROGRESS END 
        cmds.progressBar( self.m_progressControlClipAnim, edit = True, visible = False )    # HIDE PROGRESS BAR
            
    #------------------------------------------------------------------------------------------------------
    #     uiSkinCallback
    #------------------------------------------------------------------------------------------------------
    def uiSkinCallback( self, *args ):
        """
        Callback for Skin UI
        """
        m_input = args[0]
        #-------------------------------
        #     m_buttonAddItemSkin
        #-------------------------------
        if ( 'm_buttonAddItemSkin' == m_input ):
            m_objSelectedLong = cmds.ls( selection = True, long = True ) # get selected objects
            for m_obj in m_objSelectedLong:
                self.m_nodeSkin.addObj( m_obj ) # add object to node
            self.uiSkinRefreshScrollList()  # reload ScrollList                    
        #-------------------------------
        #     m_buttonDelItemSkin
        #-------------------------------
        if ( 'm_buttonDelItemSkin' == m_input ):
            m_items = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, selectItem = True )
            if ( None != m_items ):
                if ( 0 != len( m_items ) ):
                    for m_obj in m_items:                        
                        if ( self.m_nodeSkin.delObj( string.split( m_obj )[1] ) ): # delete from node
                            cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, removeItem = [ m_obj ] ) # remove from scrollList 
                            cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = True )
                            cmds.textField( self.m_textFieldSkeletonSkin, edit = True, backgroundColor = m_checkBoxColor ) 
                            cmds.textField( self.m_textFieldSkeletonSkin, edit = True, fileName = '' )        
                            cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = False )                                                              
                            cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = True )
                            cmds.textField( self.m_textFieldSaveDirSkin, edit = True, backgroundColor = m_checkBoxColor ) 
                            cmds.textField( self.m_textFieldSaveDirSkin, edit = True, fileName = '' )        
                            cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = False )        
        #-------------------------------
        #     m_buttonRefreshItemSkin
        #-------------------------------
        if ( 'm_buttonRefreshItemSkin' == m_input ):
            self.uiSkinRefreshScrollList() # reload ScrollList        
        #-------------------------------
        #     m_textScrollListItemSkin
        #-------------------------------
        if ( 'm_textScrollListItemSkin' == m_input ):
            m_items_all = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, allItems = True )    # get all items from scrollList
            # reset selection state in the node
            for m_obj in m_items_all:
                self.m_nodeSkin.setSelectionState( string.split( m_obj )[1], False )
            m_items = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, selectItem = True ) # get selected items from scrollList
            if ( None != m_items ):
                for m_obj in m_items:
                    self.m_nodeSkin.setSelectionState( string.split( m_obj )[1], True )
            self.uiSkinLoadDataFromNode() # reload ScrollList        
        #-------------------------------
        #     m_textPopupMenuItemRefreshSkin
        #------------------------------- 
        if ( 'm_textPopupMenuItemRefreshSkin' == m_input ):
            self.uiSkinRefreshScrollList()                                       
        #-------------------------------
        #     m_buttonExportAllItemSkin
        #-------------------------------
        if ( 'm_buttonExportAllItemSkin' == m_input ):
            if ( cmds.checkBox( self.m_checkBoxNoDB, query = True, value = True ) ):
                self.m_nodeSkin.exportDataNoDBAll()
            else:
                self.m_nodeSkin.exportDataAll()			
			
        #-------------------------------
        #     m_buttonExportSelItemSkin
        #-------------------------------
        if ( 'm_buttonExportSelItemSkin' == m_input ):
            if ( cmds.checkBox( self.m_checkBoxNoDB, query = True, value = True ) ):
                self.m_nodeSkin.exportDataNoDBSel()
            else:
                self.m_nodeSkin.exportDataSel()           
        #-------------------------------
        #     m_symbolButtonSkeletonSkin 
        #-------------------------------
        if ( 'm_symbolButtonSkeletonSkin' == m_input ):
            if ( None == getRealXrayResourcesPath( 'skeletons' ) ): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            else:  
                self.m_fileDialog2SkeletonSkin = cmds.fileDialog2( fileFilter        = "All Files (*.*)",
                                                                   dialogStyle       = 2, 
                                                                   fileMode          = 1,
                                                                   caption           = 'Select reference skeleton',
                                                                   startingDirectory = getRealXrayResourcesPath( 'skeletons' ),
                                                                   okCaption         = 'Select skeleton' )
                if ( None == self.m_fileDialog2SkeletonSkin ):
                   # print "Cancel button pressed"
                   pass
                else:
                    m_skeleton = self.m_fileDialog2SkeletonSkin[0]
                    m_skeleton = m_skeleton.lower()
                    m_skeleton = os.path.splitext( m_skeleton )[0] # cut file extension
                    m_realPath = getRealXrayResourcesPath( 'skeletons' )
                    # if 'x:/resources/exported/animations/skeletons' don't exist, redefine m_skeleton
                    if ( -1 == m_skeleton.find( m_realPath ) ):
                        m_skeleton = m_realPath + string.replace( m_skin_skeletonDefault, m_xrayResourcesPathSkeletons, '', 1 )
                    # write to node :
                    m_skeleton = m_xrayResourcesPathSkeletons + m_skeleton.replace( m_realPath, '', 1 )
                    # -------------------------------------------------
                    m_items = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, selectItem = True ) # get selected items from scrollList
                    if (None != m_items):
                        if (0 != len(m_items)):
                            i = 0
                            while (i != len(m_items)):
                                m_obj = m_items[i] # get old label
                                self.m_nodeSkin.setSkeleton( string.split(m_obj)[1], m_skeleton ) # write m_saveDir to the node
                                i += 1
                    self.uiSkinRefreshScrollList() # reload ScrollList            
        #-------------------------------
        #     m_symbolButtonSaveDirSkin
        #------------------------------- 
        if ( 'm_symbolButtonSaveDirSkin' == m_input ):
            if ( None == getRealXrayResourcesPath( 'models' ) ): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            else:  
                self.m_fileDialog2SaveDirSkin = cmds.fileDialog2( fileFilter = 'Folder',
                                                                  dialogStyle = 2,
                                                                  fileMode = 3, 
                                                                  caption = 'Save file to ...',
                                                                  startingDirectory = getRealXrayResourcesPath( 'models' ),
                                                                  okCaption = 'Set Dir' )
                if ( None == self.m_fileDialog2SaveDirSkin ):
                   # print "Cancel button pressed"
                   pass
                else:
                    m_saveDir = self.m_fileDialog2SaveDirSkin[0]
                    m_saveDir = m_saveDir.lower()
                    m_realPath = getRealXrayResourcesPath( 'models' )
                    # if 'x:/resources/exported/models' don't exist, redefine m_saveDir
                    if ( -1 == m_saveDir.find( m_realPath ) ):
                        m_saveDir = m_realPath
                    # -------------------------------------------------
                    m_items = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, selectItem = True ) # get selected items from scrollList
                    m_saveDir = m_xrayResourcesPathMeshes + m_saveDir.replace( m_realPath, '' , 1 )
                    if (None != m_items):
                        if (0 != len(m_items)):
                            i = 0
                            while (i != len(m_items)):
                                m_obj = m_items[i] # get old label
                                self.m_nodeSkin.setSaveDir( string.split(m_obj)[1], m_saveDir ) # write m_saveDir to the node
                                i += 1
                    self.uiSkinRefreshScrollList() # reload ScrollList   

    def uiSkinRefreshScrollList( self ):
        """
        Refresh self.m_textScrollListItemSkin
        """  
        # read data from node
        m_hash = self.m_nodeSkin.iterItems()
        # obj      - '|group1|stalker1'
        # data[0]  - False       
        # data[1]  - '/models/actors'
        # data[2]  - '/animations/skeletons/human'
        # data[3]  - 'stalker1    |group1|stalker1    /models/actors/stalker1.model   /animations/skeletons/human.skeleton'
        cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, removeAll = True ) # clear ScrollList
        for obj,data in m_hash.iteritems():
            cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, append = [ data[3] ] ) 
            if ( data[0] ):
                cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, selectItem = data[3] ) 
        # get selected DATA from the node 
        self.uiSkinLoadDataFromNode() 

    def uiSkinLoadDataFromNode( self ):
        """
        Load data to right panel from
        """
        cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldSkeletonSkin, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldSkeletonSkin, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = False )
        cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldSaveDirSkin, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldSaveDirSkin, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = False )  
        # ---------------------------------------------------------------------------
        cmds.progressBar( self.m_progressControlSkin, edit = True, visible = True )        # SHOW PROGRESS BAR
        cmds.progressBar( self.m_progressControlSkin, edit = True, beginProgress = True )  # PROGRESS BEGIN   
        cmds.progressBar( self.m_progressControlSkin, edit = True, step = 33 )             # PROGRESS 33% 
        # read data from node
        m_hash = self.m_nodeSkin.iterItems()  
        # obj      - '|group1|stalker1'
        # data[0]  - False       
        # data[1]  - '/models/actors'
        # data[2]  - '/animations/skeletons/human'
        # data[3]  - 'stalker1    |group1|stalker1    /models/actors/stalker1.model   /animations/skeletons/human.skeleton'
        m_items = cmds.textScrollList( self.m_textScrollListItemSkin, query = True, allItems = True )    # get all items from scrollList
        cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, deselectAll = True )   # deselect ALL items from scrollList 
        m_hashSelected = {} # create dictionary with selected items
        for obj,data in m_hash.iteritems():
            if ( data[0] ):
                if ( None != m_items and 0 != len( m_items ) ):
                    i = 0
                    while ( i != len( m_items ) ):
                        m_obj = m_items[i] # get label
                        m_nodeName = string.split( m_obj )[1]
                        if ( obj == m_nodeName ):
                            cmds.textScrollList( self.m_textScrollListItemSkin, edit = True, selectItem = m_obj )
                            m_hashSelected[ obj ] = ( data[0], data[1], data[2], data[3] )
                            break
                        i += 1
        # get selected DATA from the node                         
        m_hash = m_hashSelected
        if (0 != len( m_hash )):
            #---------------------
            #   set save dir
            #---------------------
            for obj,data in m_hash.iteritems():
                m_saveDir = data[1] # get save dir                                                          
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_saveDir != data[1] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldSaveDirSkin, edit = True, backgroundColor = m_multiselColor ) # multi selection color
                cmds.textField( self.m_textFieldSaveDirSkin, edit = True, enableBackground = False )
            cmds.textField( self.m_textFieldSaveDirSkin, edit = True, fileName = str( m_saveDir ) )                  
            cmds.progressBar( self.m_progressControlSkin, edit = True, step = 33 )      # PROGRESS 66%
            #---------------------
            #   set save dir
            #---------------------
            for obj,data in m_hash.iteritems():
                m_skeleton = data[2] # get m_skeleton                                                         
                break
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_skeleton != data[2] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldSkeletonSkin, edit = True, backgroundColor = m_multiselColor ) # multi selection color
                cmds.textField( self.m_textFieldSkeletonSkin, edit = True, enableBackground = False )
            cmds.textField( self.m_textFieldSkeletonSkin, edit = True, fileName = str( m_skeleton ) )                  
            cmds.progressBar( self.m_progressControlSkin, edit = True, step = 33 )      # PROGRESS 99%                        
        cmds.progressBar( self.m_progressControlSkin, edit = True, endProgress = True ) # PROGRESS END
        cmds.progressBar( self.m_progressControlSkin, edit = True, visible = False )    # HIDE PROGRESS BAR                              

    #------------------------------------------------------------------------------------------------------
    #     uiSkeletonCallback
    #------------------------------------------------------------------------------------------------------
    def uiSkeletonCallback( self, *args ):
        """
        Callback for Skeleton UI
        """
        m_input = args[0]
        #-------------------------------
        #     m_buttonAddItemSkel
        #-------------------------------
        if ( 'm_buttonAddItemSkel' == m_input ):
            m_objSelectedLong = cmds.ls( selection = True, long = True ) # get selected objects
            for m_obj in m_objSelectedLong:
                self.m_nodeSkeleton.addObj( m_obj ) # add object to node
            self.uiSkeletonRefreshScrollList()  # reload ScrollList
        #-------------------------------
        #     m_buttonDelItemSkel
        #-------------------------------
        if ( 'm_buttonDelItemSkel' == m_input ):                        
            m_items = cmds.textScrollList( self.m_textScrollListItemSkel, query = True, selectItem = True )
            if ( None != m_items ):
                if ( 0 != len( m_items ) ):
                    for m_obj in m_items:                        
                        if ( self.m_nodeSkeleton.delObj( string.split( m_obj )[1] ) ): # delete from node                            
                            cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, removeItem = [ m_obj ] ) # remove from scrollList      
                            # reset obj type background color
                            cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = True )
                            cmds.textField( self.m_textFieldSaveDirSkel, edit = True, backgroundColor = m_checkBoxColor ) 
                            cmds.textField( self.m_textFieldSaveDirSkel, edit = True, fileName = '' )        
                            cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = False )
        #-------------------------------
        #     m_buttonRefreshItemSkel
        #-------------------------------
        if ( 'm_buttonRefreshItemSkel' == m_input ):
            self.uiSkeletonRefreshScrollList() # reload ScrollList
        #-------------------------------
        #     m_textScrollListItemSkel
        #-------------------------------
        if ( 'm_textScrollListItemSkel' == m_input ):
            m_items_all = cmds.textScrollList( self.m_textScrollListItemSkel, query = True, allItems = True )    # get all items from scrollList
            # reset selection state in the node
            for m_obj in m_items_all:
                self.m_nodeSkeleton.setSelectionState( string.split( m_obj )[1], False )
            m_items = cmds.textScrollList( self.m_textScrollListItemSkel, query = True, selectItem = True ) # get selected items from scrollList
            if ( None != m_items ):
                for m_obj in m_items:
                    self.m_nodeSkeleton.setSelectionState( string.split( m_obj )[1], True )
            self.uiSkeletonLoadDataFromNode() # reload ScrollList
        #-------------------------------
        #     m_textPopupMenuItemRefreshSkel
        #-------------------------------
        if ( 'm_textPopupMenuItemRefreshSkel' == m_input ):
            self.uiSkeletonRefreshScrollList() # reload ScrollList
        #-------------------------------
        #     m_buttonExportAllItemSkel
        #-------------------------------
        if ( 'm_buttonExportAllItemSkel' == m_input ):
            self.m_nodeSkeleton.exportData( False ) 
        #-------------------------------
        #     m_buttonExportSelItemSkel
        #-------------------------------
        if ( 'm_buttonExportSelItemSkel' == m_input ):
            self.m_nodeSkeleton.exportData( True )  
        #-------------------------------
        #     m_symbolButtonSaveDirSkel
        #-------------------------------
        if ( 'm_symbolButtonSaveDirSkel' == m_input ):
            if ( None == getRealXrayResourcesPath( 'skeletons' ) ): # initialize 'XRAY_RESOURCES_PATH'
                self.errorHandle.raiseMessage("m_skeletonNode::__init__ XRAY_RESOURCES_PATH don't exists")
            else:  
                self.m_fileDialog2SaveDir = cmds.fileDialog2( fileFilter = 'Folder',
                                                              dialogStyle = 2, 
                                                              fileMode = 3, 
                                                              caption = 'Save file to ...', 
                                                              startingDirectory = getRealXrayResourcesPath( 'skeletons' ),
                                                              okCaption = 'Set Dir' )
                if (None == self.m_fileDialog2SaveDir):
                   # print "Cancel button pressed"
                   pass
                else:
                    m_saveDir = self.m_fileDialog2SaveDir[0]
                    m_saveDir = m_saveDir.lower()
                    m_realPath = getRealXrayResourcesPath( 'skeletons' )
                    # if 'x:/resources/exported/animations/skeletons' don't exist, redefine m_saveDir
                    if ( -1 == m_saveDir.find( m_realPath ) ):
                        m_saveDir = m_realPath                    
                    # -------------------------------------------------
                    m_items = cmds.textScrollList( self.m_textScrollListItemSkel, query = True, selectItem = True ) # get selected items from scrollList  
                    m_saveDir = m_saveDir.replace( m_realPath, '' , 1 )                                              
                    if (None != m_items):
                        if (0 != len(m_items)):
                            i = 0
                            while (i != len(m_items)):
                                m_obj = m_items[i] # get old label
                                self.m_nodeSkeleton.setSaveDir(string.split(m_obj)[1],str( m_xrayResourcesPathSkeletons + m_saveDir )) # write m_saveDir to the node
                                i += 1
                    self.uiSkeletonRefreshScrollList() # reload ScrollList   

    def uiSkeletonRefreshScrollList( self ):
        """
        Refresh self.m_textScrollListItemSkel
        """  
        # read data from node
        m_hash = self.m_nodeSkeleton.iterItems()
        # obj      - '|group1|human'
        # data[0]  - False       
        # data[1]  - 'XRAY_RESOURCES_PATH/animations/skeletons'
        # data[2]  - 'human    |group1|human    x:/resources/exported/animations/skeletons/human'
        # data[3]  - 'x:/resources/exported/animations/skeletons/'
        cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, removeAll = True ) # clear ScrollList
        for obj,data in m_hash.iteritems():
            cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, append = [ data[2] ] ) 
            if ( data[0] ):
                cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, selectItem = data[2] ) 
        # get selected DATA from the node 
        self.uiSkeletonLoadDataFromNode() 

    def uiSkeletonLoadDataFromNode( self ):
        """
        Load data to right panel from
        """
        # reset txtField background color
        cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = True )
        cmds.textField( self.m_textFieldSaveDirSkel, edit = True, backgroundColor = m_checkBoxColor ) 
        cmds.textField( self.m_textFieldSaveDirSkel, edit = True, fileName = '' )        
        cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = False )
        # ---------------------------------------------------------------------------
        cmds.progressBar( self.m_progressControlSkel, edit = True, visible = True )        # SHOW PROGRESS BAR
        cmds.progressBar( self.m_progressControlSkel, edit = True, beginProgress = True )  # PROGRESS BEGIN
        cmds.progressBar( self.m_progressControlSkel, edit = True, step = 50 )             # PROGRESS 50%
        # read data from node
        m_hash = self.m_nodeSkeleton.iterItems()
        # obj      - '|group1|human'
        # data[0]  - False
        # data[1]  - 'XRAY_RESOURCES_PATH/animations/skeletons'
        # data[2]  - 'human    |group1|human    x:/resources/exported/animations/skeletons/human'
        # data[3]  - 'x:/resources/exported/animations/skeletons/'
        m_items = cmds.textScrollList( self.m_textScrollListItemSkel, query = True, allItems = True )    # get all items from scrollList
        cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, deselectAll = True )   # deselect ALL items from scrollList
        m_hashSelected = {} # create dictionary with selected items                    
        for obj,data in m_hash.iteritems():
            if ( data[0] ):
                if ( None != m_items and 0 != len( m_items ) ):
                    i = 0
                    while ( i != len( m_items ) ):
                        m_obj = m_items[i] # get label
                        m_nodeName = string.split( m_obj )[1]
                        if ( obj == m_nodeName ):
                            cmds.textScrollList( self.m_textScrollListItemSkel, edit = True, selectItem = m_obj )
                            m_hashSelected[ obj ] = ( data[0], data[1], data[2], data[3] )
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
            m_hasDifference = False
            for obj,data in m_hash.iteritems():
                if ( m_saveDir != data[3] ):
                    m_hasDifference = True # find difference
                    break
            if ( m_hasDifference ): # if we have differences in multiselection let's color it
                cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = True )
                cmds.textField( self.m_textFieldSaveDirSkel, edit = True, backgroundColor = m_multiselColor ) # multi selection color
                cmds.textField( self.m_textFieldSaveDirSkel, edit = True, enableBackground = False )
            cmds.textField( self.m_textFieldSaveDirSkel, edit = True, fileName = str( m_saveDir ) )                  
            cmds.progressBar( self.m_progressControlSkel, edit = True, step = 50 )      # PROGRESS 100%
        cmds.progressBar( self.m_progressControlSkel, edit = True, endProgress = True ) # PROGRESS END
        cmds.progressBar( self.m_progressControlSkel, edit = True, visible = False )    # HIDE PROGRESS BAR