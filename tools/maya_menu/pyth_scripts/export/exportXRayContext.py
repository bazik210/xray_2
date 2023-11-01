#-----------------------------------------------------------------
#	SCRIPT			exportXRayContext.py
#	AUTHOR			Zaitsev Evgeniy
#                   ev.zaitsev@gmail.com
#	DATE:			15 July 2010
#
#	DESCRIPTION:	Define tools for exporters
#                   S.T.A.L.K.E.R. 2 project
#
#  VERSION:	        201101
#
#-----------------------------------------------------------------
import maya.OpenMaya as OpenMaya            # work with maya API
import maya.OpenMayaAnim as OpenMayaAnim
import maya.cmds as cmds                    # work with maya commands
import maya.mel as mel                      # work with mel
import ConfigParser                         # config parser
import string                               # string parser
import re                                   # regular expression
import os                                   # work with file system
import sys                                  # python and maya environment
#-----------------------------------------------------------------------------------
#	global variables
#-----------------------------------------------------------------------------------
# the name of the node
m_meshNodeName = 'xray_mesh_lister'
m_animNodeName = 'xray_anim_lister'
# define the list of object types
m_objectType       =    ['Static', 'Dynamic']
# create tree of categories
m_categoryTree     =    {
                            (0, 'Architectural')    : ('Building', 'Props'),
                            (1, 'Location')         : ('Armory', 'Bathroom', 'Bedroom', 'Cemetery', 'Construction', 'Firestation', 'Fishers',
                                                       'Garage', 'Industrial', 'Kindergarten', 'Kitchen', 'Laboratory', 'Living_Room', 'Medical', 'Military',
                                                       'Museum', 'Port', 'Railroad', 'Sawmill', 'School', 'Shop', 'Street', 'Town', 'Village', 'Workshop'),
                            (2, 'Object')           : ('Anomaly', 'Artefacts', 'Barrel', 'Bed', 'Briks', 'Cabinet', 'Chair', 'Clothes',
                                                       'Concrete', 'Container', 'Cover', 'Debris', 'Devices', 'Door', 'Electronic', 'Fence',
                                                       'Fireplace', 'Flooring', 'Fx', 'House', 'Instrument', 'Light', 'Luke', 'Other', 'Outfit',
                                                       'Sanitary', 'Semafor', 'Sign', 'Table', 'Technics', 'Tent', 'Tomb', 'Tower', 'Tubes',
                                                       'Vehicles', 'Vent', 'Weapons', 'Wiring'),
                            (3, 'Creature')         : ('Actors', 'Monsters', 'Dead_Body'),
                            (4, 'Foliage')          : ('Tree', 'Bush', 'Grass'),
                        }
# define list of OpenMaya.MTime enumaration Unit 
# DO NOT CHANGE ORDERING
m_FPSlist = ['hours', 'minutes', 'seconds', 'miliseconds', '15 fps', 'Film (24 fps)', 'PAL (25 fps)', 'NTSC (30 fps)', 'Show (48 fps)',
             'PAL Field (50 fps)', 'NTSC Field (60 fps)', '2 fps', '3 fps', '4 fps', '5 fps', '6 fps', '8 fps', '10 fps', '12 fps',
             '16 fps', '20 fps', '40 fps', '75 fps', '80 fps', '100 fps', '120 fps', '125 fps', '150 fps', '200 fps', '240 fps', '250 fps',
             '300 fps', '375 fps', '400 fps', '500 fps', '600 fps', '750 fps', '1200 fps', '1500 fps', '2000 fps', '3000 fps', '6000 fps']
# XRAY_RESOURCES_PATH defined in maya.env
m_xrayResourcesPathMeshes     = 'XRAY_RESOURCES_PATH/models'
m_xrayResourcesPathAnims      = 'XRAY_RESOURCES_PATH/animations'
m_xrayResourcesPathSkeletons  = 'XRAY_RESOURCES_PATH/animations/skeletons'
m_anim_epsilonMin             = 0.00005
m_anim_epsilonMax             = 0.1
m_anim_epsilonDefault         = 0.005
m_anim_saveDirDefault         = m_xrayResourcesPathAnims
m_anim_baseAnimationDefault   = m_xrayResourcesPathAnims + '/default'
m_skel_saveDirDefault         = m_xrayResourcesPathSkeletons
m_skin_skeletonDefault        = m_xrayResourcesPathSkeletons + '/human'
m_skin_saveDirDefault         = m_xrayResourcesPathMeshes
#-----------------------------------------------------------------------------------
#	TRACE CLASS
#-----------------------------------------------------------------------------------
class m_meshTrace(object):
    """
    class used to trace script errors into Output Maya Window
    """
    def __init__(self, m_initMsg = None):
        """
        initialize the default values
        """
        self.msg = m_initMsg

    def broadcastMessage(self):
        """
        display the error in the Output Maya Window
        """
        # 0 - kInteractive  	Running with a UI.
        # 1 - kBatch  	        Running without a UI.
        # 2 - kLibraryApp  	    A standalone (MLibrary) application is running.
        # 3 - kBaseUIMode  	    Running in baseUI mode.
        if (None != self.msg):
            if (2 == OpenMaya.MGlobal_mayaState()):
                m_fileName = sys.path[len(sys.path)-1]
                m_fileName = string.replace(m_fileName,'scripts','') + 'xrayMayaLog.txt'
                m_fileHandle = open ( m_fileName, 'a' )
                m_fileHandle.write ('TRACE: ' + self.msg + '\n' )
                m_fileHandle.close()
		self.msg = 'TRACE: ' + self.msg
		print(self.msg)
            if (0 == OpenMaya.MGlobal_mayaState()):
                self.msg = 'TRACE: ' + self.msg
                #print(self.msg)
                mel.eval('trace("'+ self.msg +'")')

    def raiseMessage(self, m_newMsg):
        """
        update the error and call out to display in Output Maya Window
        """
        self.msg = m_newMsg
        self.broadcastMessage()

    def clearLogFile(self):
        """
        """
        # if (2 == OpenMaya.MGlobal_mayaState()):
        #     m_fileName = sys.path[len(sys.path)-1]
        #     m_fileName = string.replace(m_fileName,'scripts','') + 'xrayMayaLog.txt'
        #     m_fileHandle = open ( m_fileName, 'w' )
        #     m_fileHandle.close()
#-----------------------------------------------------------------------------------
#	MAYA FILES CLASS
#-----------------------------------------------------------------------------------
class m_filesMaya(object):
    """
    class used for getting list of '.mb' files stored in specified directory
    USAGE:  mayaFiles = m_filesMaya()
            mayaFiles.getAllMayaFiles('e:/upload/test')
    """
    def __init__(self):
        """
        initialize the default values
        """
        self.m_mayaFileList = []

    def getAllMayaFiles(self, m_startCatalog):
        """
        RETURN: all '.mb' files stored in specified catalog        
        """
        self.m_mayaFileList = []
        self.getFiles(m_startCatalog)
        return self.m_mayaFileList

    def getFiles(self, m_startCatalog):
        """
        special function for recursion
        """
        if (os.path.exists(m_startCatalog)):               
            for m_fileName in os.listdir(m_startCatalog):
                m_newCatalog = m_startCatalog + '/' + m_fileName
                if (os.path.isfile(m_newCatalog)):
                    m_name = os.path.splitext(m_newCatalog)
                    if ('.mb' == m_name[1]):
                        self.m_mayaFileList.append(m_newCatalog)
                if (os.path.isdir(m_newCatalog)):            
                    self.getFiles(m_newCatalog)
#-----------------------------------------------------------------------------------
#	MAYA ANIMATION LAYERS CLASS
#-----------------------------------------------------------------------------------
class m_animLayers( object ):
    """
    class used for getting MObjectArray() of animationLayers existing in scene
    USAGE:  m_anim_layers = m_animLayers()
            m_obj_array = OpenMaya.MPlugArray()
            m_obj_array = m_anim_layers.get()
    """
    def __init__( self ):
        """
        initialize the default values
        """
        self.m_obj_array = OpenMaya.MObjectArray()

    def get( self ):
        """
        RETURN: all animationLayers stored in scene
        """
        self.m_obj_array = OpenMaya.MObjectArray()        
        self.getAnimLayers( 'BaseAnimation' )
        return self.m_obj_array

    def getAnimLayers( self, m_node ):
        """
        INPUT  m_node - animationLayer name (String)
        special function for recursion
        """
        try:
            if ( cmds.animLayer( m_node, query = True, exists = True ) ):        
                m_animLayers = cmds.animLayer( m_node, query = True, children = True )
                for m_layer in m_animLayers:
                    m_animLayersTemp = cmds.animLayer( m_layer, query = True, children = True )
                    if ( None == m_animLayersTemp or 0 == len( m_animLayersTemp ) ):                    
                        self.m_obj_array.append( getMObjectFromNodeName( m_layer ) )
                    else:
                        self.m_obj_array.append( getMObjectFromNodeName( m_layer ) )
                        self.getAnimLayers( m_layer )
        except:
            pass
            
    def animLayersMuteSnapshot( self ):
        """
        create snapshot of mute attribute from all animLayer nodes
        """
        self.m_obj_array = self.get() # get all animation layers currently stored in maya file 
        self.m_animLayersMute = []
        for m_ind in range( self.m_obj_array.length() ):
            self.m_animLayersMute.append( cmds.animLayer( getNodeNameFromeMObject( self.m_obj_array[ m_ind ] ) , query = True, mute = True )  )
    
    def animLayersMuteAll( self ):
        """
        set to True .Mute attr in all animLayer nodes
        """              
        for m_ind in range( self.m_obj_array.length() ):
            cmds.animLayer( getNodeNameFromeMObject( self.m_obj_array[ m_ind ] ) , edit = True, mute = True )        
    
    def animLayersMuteRestore( self ):
        """
        restore from snapshot all animLayer nodes .Mute attribute
        """                           
        for m_ind in range( self.m_obj_array.length() ):
            cmds.animLayer( getNodeNameFromeMObject( self.m_obj_array[ m_ind ] ) , edit = True, mute = self.m_animLayersMute[m_ind] )

#-----------------------------------------------------------------------------------
#     RawConfigParser Overloaded class for writing sorted data
#-----------------------------------------------------------------------------------
class OrderedRawConfigParser( ConfigParser.RawConfigParser ):
    """
    Overload standart Class ConfigParser.RawConfigParser
    """
    def __init__( self, defaults = None, dict_type = dict ):
        ConfigParser.RawConfigParser.__init__( self, defaults = None, dict_type = dict )

    def write(self, fp):
        """Write an .ini-format representation of the configuration state."""
        if self._defaults:
            fp.write("[%s]\n" % DEFAULTSECT)
            for key in sorted( self._defaults ):                
                fp.write( "%s = %s\n" % (key, str( self._defaults[ key ] ).replace('\n', '\n\t')) )                 
            fp.write("\n")
        for section in self._sections:
            fp.write("[%s]\n" % section)
            for key in sorted( self._sections[section] ): 
                if key != "__name__":
                    fp.write("%s = %s\n" %
                             (key, str( self._sections[section][ key ] ).replace('\n', '\n\t')))    
            fp.write("\n")
#-----------------------------------------------------------------------------------
#	UTILITIES
#-----------------------------------------------------------------------------------
def getFullPathToTag(m_tag = None):
    """
    INPUT  string value like: 'Industrial'
    RETURN string value like: 'Location|Industrial' based on global array m_categoryTree
           or None - if m_tag not specified and m_tag don't exists in dictionary of tags
    USAGE  newstring = getFullPathToTag('Industrial')
    """
    errorHandle = m_meshTrace()
    if (m_tag == None):
        errorHandle.raiseMessage("getFullPathToTag Specify input tag")
        return None
    for key in m_categoryTree.keys(): # search in global array
        if (0 != m_categoryTree[key].count(m_tag)):
            return ("%s|%s" %(key[1], m_tag))
    errorHandle.raiseMessage("getFullPathToTag Input tag %s don't exist in dictionary m_categoryTree" %m_tag)
    return None

def getListFromRegExpr(m_string = None):
    """
    INPUT:  string like this: 'Location|Industrial'
    RETURN: list of strings like this: ['Location', 'Industrial']
            or None - if m_string not specified
    USAGE:  newlist = getListFromRegExpr('Location|Industrial')
    """
    if (m_string == None):
        errorHandle = m_meshTrace()
        errorHandle.raiseMessage("getListFromRegExpr Specify input string")
        return None
    m_list = []
    m_looklike = r'[^|]*' # r - raw, init string as is
    m_matcher = re.compile(m_looklike,re.IGNORECASE)
    m_list = m_matcher.findall(m_string)
    while (m_list.count('') != 0):
        m_list.remove('')
    return m_list

def getRealXrayResourcesPath( m_path = 'models' ):
    """
    INPUT:  string like this:   'models'
                                'animations'
                                'physics'
                                'skeletons'
                                'scenes'
    RETURN: string like: 
                    'x:/resources/exported/models' 
                    'x:/resources/exported/animations'
                    'x:/resources/exported/physics'
                    'x:/resources/exported/animations/skeletons'
                    'x:/resources/sources/scenes'
            if initialization done successfully
            None otherwise
    """
    errorHandle = m_meshTrace()

    XRAY_RESOURCES_PATH = mel.eval ('getenv "XRAY_RESOURCES_PATH" ')
    
    if  ( 'models' == m_path ):
        m_xrayResourcesPath = XRAY_RESOURCES_PATH + '/exported/models'
    elif ( 'animations/single' == m_path ):
        m_xrayResourcesPath = XRAY_RESOURCES_PATH + '/exported/animations/single'
    elif ( 'skeletons' == m_path ):
        m_xrayResourcesPath = XRAY_RESOURCES_PATH + '/exported/animations/skeletons'
    elif ( 'physics' == m_path ):
        m_xrayResourcesPath = XRAY_RESOURCES_PATH + '/exported/physics'
    elif ( 'scenes' == m_path ):
        m_xrayResourcesPath = XRAY_RESOURCES_PATH + '/sources/scenes'
      
    # --------------------------------
    if ( os.path.exists( m_xrayResourcesPath ) ): # check if path to directory with resources exists
        return m_xrayResourcesPath # initializaton done
    else:
        errorHandle.raiseMessage( 'path [' + m_xrayResourcesPath + ']does not exist, check XRAY_RESOURCES_PATH variable in maya.env' )
        return None
        
def getPartialXrayResourcesPath( m_path = 'models' ):
    """
    INPUT:  string like this:   'models'
    RETURN: string like: 
                    'XRAY_RESOURCES_PATH/models/props/street' 
            if initialization done successfully            
    """
    m_filepath = OpenMaya.MFileIO.currentFile()
    m_filepath = m_filepath.lower()
    m_filename = os.path.splitext( os.path.split( m_filepath )[1] )[0]
    m_filepath = os.path.split( m_filepath )[0]
    m_currDir = "%s/%s_instance" %( m_filepath, m_filename )
    #----------------------------------------------------------------
    if ( os.path.exists( m_currDir ) ):
        m_filepath = m_currDir
    m_return = m_xrayResourcesPathMeshes
    m_partialPath = None
    if ( 'untitled' != m_filename ):
        m_realPath = getRealXrayResourcesPath( 'scenes' )
        if ( m_realPath ):
            if ( -1 != m_filepath.find( m_realPath ) ):
                m_partialPath = m_filepath.replace( m_realPath, '' , 1 )          
    if ( 'models' == m_path ):
        m_return = m_xrayResourcesPathMeshes  
        if ( m_partialPath ):
            m_return = "%s%s" %( m_xrayResourcesPathMeshes, m_partialPath )
    return m_return

def getRealPathToReference( m_filepath ):
    """
    get valid path to file
    INPUT:  m_filepath - string like 
            "x:/resources/sources/scenes/architecture/research_institute/fence_metal_instance/fence_metal_small.mb"
    RETURN: string, path to file - if file exists ( os.path.exists ),
                None - if file don't exists or Xray plugin don't loaded               
    """
    m_realPath = getRealXrayResourcesPath('scenes')
    if ( not m_realPath ):
        return None
    m_resultPath = m_filepath
    if ( not os.path.exists( m_filepath ) ):
        m_filepathTmp1 = "%s%s" %( m_realPath, os.path.splitdrive( m_filepath )[1] )
        if ( not os.path.exists( m_filepathTmp1 )  ):
            try:
                m_filepathTmp2 = "%s%s" %( m_realPath, string.split( m_filepath, 'resources/sources/scenes' )[1] ) 
            except:
                m_filepathTmp2 = m_filepathTmp1
            if ( not os.path.exists( m_filepathTmp2 )  ):
                return None
            else:
                m_resultPath = m_filepathTmp2
        else:
            m_resultPath = m_filepathTmp1
    return m_resultPath    

def getShortName( m_string ):
    """
    INPUT:  string like this: '|props|box|box_01'
    RETURN: 'box_01'
    """
    m_list = []
    m_looklike = r'[^|]*$' # r - raw, init string as is
    m_matcher = re.compile(m_looklike,re.IGNORECASE)
    m_list = m_matcher.findall(m_string)
    return m_list[0]

def addSpacesToString(m_inputStr, m_numberOfSpaces):
    """
    INPUT:  string 'box_01'
    RETURN: string 'box_01    '    
    USAGE:  new_string = addSpacesToString('box_01',4)
    """
    for n in xrange(m_numberOfSpaces):
        m_inputStr += ' '
    return m_inputStr 

def makeName(m_inputString, m_type = 'nodename'):
    """
    INPUT:  m_inputString = 'com1 d f f@ d ! '
            m_type        = 'nodename', 'filename'
            
    RETURN: 'com1_d_f_f__d___'
            if m_type == nodename return with '|' character
    """
    if   ('nodename' == m_type):
        m_badchars = re.compile(r'[^A-Za-z0-9_|. ]+|^\.|\.$|^ | $|^$|\s')
        return m_badchars.sub('_', m_inputString)
    elif ('filename' == m_type):
        m_badchars = re.compile(r'[^A-Za-z0-9_. ]+|^\.|\.$|^ | $|^$|\s')
        m_badnames = re.compile(r'(aux|com[1-9]|con|lpt[1-9]|prn)(\.|$)')
        m_name = m_badchars.sub('', m_inputString)
        if m_badnames.match(m_name):
            m_name = '_' + m_name
        return m_name

def getFormattedStringList(m_inputList):
    """
    INPUT:  ['123s'           , 'fn knf nk kssss', 'fdf b bfbf m'   ]
    RETURN: ['123s           ', 'fn knf nk kssss', 'fdf b bfbf m   ']
    """
    m_returnList = []
    m_maxLenght = 0
    for m_item in m_inputList:
        if (m_maxLenght < len(m_item)):
            m_maxLenght = len(m_item)
    for m_item in m_inputList:
        m_returnList.append(addSpacesToString(m_item,m_maxLenght - len(m_item)))
    return m_returnList

def createDirectories(m_existDir, m_needCreateDir):
    """
    Create directory hierarchy based on 2 strings - m_existDir and m_needCreateDir
    INPUT:    m_existDir       - string, like: 'C:/TEMP'
              m_needCreateDir  - string, like: '/test1/test2/test3'

    RETURN:   True - if all directories creates properly, False - otherwise
    """
    m_list = m_needCreateDir.rsplit('/')
    while (0 != m_list.count('') ):
        m_list.remove('')
    for m_cat in m_list:
        m_existDir += '/' + m_cat
        if (not os.path.exists(m_existDir)):
            try:
                os.mkdir(m_existDir)
            except:
                return False
    return True

def isMeshHasSkinCluster(m_objName):
    """
    INPUT:  m_objName = '|group2|stalker'
            full path to kTransform object stored in scene
    RETURN: True - if m_objName has any skinCluster, False - otherwise            
    """
    m_selectionList = OpenMaya.MSelectionList()     # create selectionList
    m_objPath = OpenMaya.MDagPath()                 # define MDagPath
    try:
        m_selectionList.add(m_objName)              # add object to selectionList     
    except:
        return False 
    try:
        m_selectionList.getDagPath(0, m_objPath)        # get first element in the selection list and connect with MDagPath
    except:
        return False        
    m_dagIterator = OpenMaya.MItDag(OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid)        # create NULL dagIterator
    m_dagIterator.reset(m_objPath.node(), OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kInvalid)  # connect dagIterator with MDagPath             
    while not m_dagIterator.isDone(): # iterate
        m_objCurr = m_dagIterator.currentItem()    # current item
        if (m_objCurr.hasFn(OpenMaya.MFn.kMesh)):  # if current == kMesh
            m_objNameMObj = OpenMaya.MObject(m_objCurr)  # create MObject and connect with current item      
            #print m_dagIterator.fullPathName() # Debug info
            try: # iterate in m_objNameMObj throw DependencyGraph based on kSkinClusterFilter
                m_itDG = OpenMaya.MItDependencyGraph(m_objNameMObj,OpenMaya.MFn.kSkinClusterFilter, OpenMaya.MItDependencyGraph.kUpstream)
                while not m_itDG.isDone():
                    m_currentCluster = m_itDG.currentItem()
                    m_fnSkin = OpenMayaAnim.MFnSkinCluster(m_currentCluster)
                    #print m_fnSkin.name() # Debug info
                    return True # SkinCluster exists
                    break
            except:
                return False
                #print 'No skin cluster'
        m_dagIterator.next() # next item
    return False 

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

def getMDagPathFromNodeName( m_node_name ):
    """
    INPUT:  m_node_name = '|group6|group5'
    RETURN: MDagPath if m_node_name exists,
            None - otherwise
    """
    m_selectionList = OpenMaya.MSelectionList()
    m_objPath = OpenMaya.MDagPath()
    try:            
        m_selectionList.add( m_node_name )
        m_selectionList.getDagPath( 0, m_objPath )
    except:
        return None
    return m_objPath 

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
    
def findMIntArray( m_array, m_find_value ):      
    """
    INPUT:  m_array      - OpenMaya.MIntArray()
            m_find_value - int 
    RETURN: True  - if m_find_value exist in m_array
            False - otherwise
    """      
    for m_ind in m_array:
        if ( int( m_find_value ) == m_ind ):
            return True
    return False

def BeforeReferenceCheckCallback( retCode, fileObject, clientData ):
    m_errorHandle = m_meshTrace()
    m_newFilePath = getRealPathToReference( fileObject.rawFullName() )
    if ( not m_newFilePath ):    
        OpenMaya.MScriptUtil.setBool( retCode, False )
        m_errorHandle.raiseMessage( "   == > Can't find referenced file %s " %fileObject.rawFullName() )
    else:
        fileObject.setRawFullName( m_newFilePath )
        OpenMaya.MScriptUtil.setBool( retCode, True )    