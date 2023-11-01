#!/usr/bin/python
# -*- coding: utf-8 -*-
# import maya & maya libraries
import maya.standalone
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
# functions for python runtime environment
import getopt, sys, os, string, time
# import S.T.A.L.K.E.R. 2 project functions
from changeMaterial import *
from materialContext import *
global m_erroNumber, m_stk2_root_path
g_xrayPlugName = 'xray.maya.mll'
#-----------------------------------------------------------------------------------
#     MAIN
#-----------------------------------------------------------------------------------
def main():
    global m_erroNumber
    global m_stk2_root_path
    m_erroNumber = 0
    m_stk2_root_path = None
    try:
        opts, args = getopt.getopt( sys.argv[1:], "o:ft",  [   'option=', 
                                                               'from=',
                                                               'to='     ] )
    except getopt.GetoptError, err:
        sys.stderr.write( str( err ) )# will print something like "option -a not recognized"
        sys.exit(2)
    m_options = [] # list of options	
    for m_option, m_value in opts:
        if   m_option in ( "-o", "--option" ): m_stk2_root_path = m_value
        elif m_option in ( "-f", "--from" ):   m_sourceType = m_value
        elif m_option in ( "-t", "--to" ):     m_destType = m_value
    if ( not m_stk2_root_path ):
       sys.stderr.write ( "Specify project root directory\n" )
       sys.exit(2) 
    processAllScenes( m_stk2_root_path, m_sourceType, m_destType ) 
    sys.exit( m_erroNumber )
#-----------------------------------------------------------------------------------
#     processAllScenes
#-----------------------------------------------------------------------------------
def processAllScenes( m_options, m_sourceType, m_destType ):
    global m_erroNumber
    m_xrayResourcesPathModels = m_stk2_root_path +'/resources/sources/scenes'
    try:
        maya.standalone.initialize( name = 'python' )
    except:
        sys.stderr.write( "Failed in initialize standalone application\n" )
        raise
    m_mayaFiles = m_filesMaya()
    m_mayaFiles = m_mayaFiles.getAllMayaFiles( m_xrayResourcesPathModels ) # get all maya files stored in the specified folder
    try:
        cmds.loadPlugin( g_xrayPlugName )   # try to load xray plugin
    except:
        sys.stderr.write('Failed to load Xray plugin\n')
        raise
    if ( None == getRealXrayResourcesPath( 'models' ) ):
        sys.stderr.write('Failed to load Xray plugin\n')
        raise 
    # Add Callback
    m_cb_id = OpenMaya.MSceneMessage.addCheckFileCallback( OpenMaya.MSceneMessage.kBeforeReferenceCheck, BeforeReferenceCheckCallback )    
    #-------------------------------------------------------------------------------   
    for m_mayaFile in m_mayaFiles:
        if ( os.path.exists( m_mayaFile ) ):
            try:
                sys.stdout.write( "Next File :%s\n" %m_mayaFile )
                cmds.file( m_mayaFile, open = True , type = 'mayaBinary', force = True )  # open file as mayaBinary
            except:
                sys.stderr.write( "ERROR Can't open file %s \n" %m_mayaFile )
                m_erroNumber = 3
                continue
            # strat change materials
            if ( changeMaterialTypeAll( m_sourceType, m_destType ) ):
                cmds.file( save = True )
            # end change materials
        else:
            sys.stderr.write( "ERROR Can't find file %s \n" %m_mayaFile )
            m_erroNumber = 3
    #-------------------------------------------------------------------------------    
    try:
        cmds.unloadPlugin( g_xrayPlugName )
    except:
        sys.stderr.write( "ERROR Can't unload Xray plugin\n" )
    # Remove Callback
    OpenMaya.MMessage.removeCallback( m_cb_id )

# run main() function when run script
if __name__ == "__main__":
   main()