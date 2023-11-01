import maya.standalone
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
import sys, os, string
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
#	UTILITIES
#-----------------------------------------------------------------------------------
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
    try:
        cmds.xray_path()
    except:
        sys.stderr.write( "Xray plugin don't loaded" )
        return None
    # get path
    if  ( 'models' == m_path ):
        m_xrayResourcesPath = cmds.xray_path( models = True )
    elif ( 'animations' == m_path ):
        m_xrayResourcesPath = cmds.xray_path( animations = True )
    elif ( 'skeletons' == m_path ):
        m_xrayResourcesPath = cmds.xray_path( animations = True ) + '/skeletons'
    elif ( 'physics' == m_path ):
        m_xrayResourcesPath = cmds.xray_path( ) + '/exported/physics'
    elif ( 'scenes' == m_path ):
        m_xrayResourcesPath = cmds.xray_path( ) + '/sources/scenes'
    # --------------------------------
    if ( os.path.exists( m_xrayResourcesPath ) ): # check if path to directory with resources exists
        return m_xrayResourcesPath # initializaton done
    else: # try to create directory
        if ( not createDirectories( cmds.xray_path() , m_xrayResourcesPath.replace( cmds.xray_path(), '' , 1 ) ) ):
            sys.stderr.write( "can't create directory %s\n" %m_xrayResourcesPath )                
            return None
    if ( os.path.exists( m_xrayResourcesPath ) ):
        return m_xrayResourcesPath # initializaton done
    else:
        sys.stderr.write( "can't find directory %s\n " %m_xrayResourcesPath )
        return None

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

def BeforeReferenceCheckCallback( retCode, fileObject, clientData ):
    m_newFilePath = getRealPathToReference( fileObject.rawFullName() )
    if ( not m_newFilePath ):    
        OpenMaya.MScriptUtil.setBool( retCode, False )
        sys.stdout.write( "   == > Can't find referenced file %s \n" %fileObject.rawFullName() )
    else:
        fileObject.setRawFullName( m_newFilePath )
        OpenMaya.MScriptUtil.setBool( retCode, True )  