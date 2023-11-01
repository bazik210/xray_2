#!/usr/bin/python
# -*- coding: utf-8 -*-
#----------------------------------------------------------------------------------
#	SCRIPT			exportXRayBatch.py
#	AUTHOR			Zaitsev Evgeniy
#				    ev.zaitsev@gmail.com
#	DATE:			03 February 2011
#
#   USAGE:      Run from shell bat file with following lines:
#
#        set MAYA=c:/Program Files/Autodesk/Maya2011
#        set STK2_ROOT_PATH=d:/hudson/jobs/Stalker_2-Polling/workspace
#        "%MAYA%/bin/mayapy.exe" "%STK2_ROOT_PATH%/tools/maya_menu/pyth_scripts/export/exportXRayBatch.py" --option %STK2_ROOT_PATH% --anim --mesh --skin --skeleton --anim_add
#----------------------------------------------------------------------------------
# import maya & maya libraries
import maya.standalone
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
import maya.mel as mel
# functions for python runtime environment
import getopt, sys, os, string, time
# import S.T.A.L.K.E.R. 2 project functions
from exportXRayContext import *      
from exportXRayContextMesh import *
from exportXRayContextSkeleton import *
from exportXRayContextSkin import *
from exportXRayContextAnim import * 
global m_erroNumber, m_stk2_root_path
g_xrayPlugName = 'xray.maya.mll'
#-----------------------------------------------------------------------------------
def main():
    global m_erroNumber
    global m_stk2_root_path
    m_erroNumber = 0
    m_stk2_root_path = None
    try:
        opts, args = getopt.getopt( sys.argv[1:], "o:",  [ 'option=' ] )
    except getopt.GetoptError, err:
        print str( err ) # will print something like "option -a not recognized"
        sys.exit(2)
    m_options = [] # list of options	
    for m_option, m_value in opts:
        if m_option in ("-o", "--option"): m_stk2_root_path = m_value
    if ( not m_stk2_root_path ):
       print ( "Specify project root directory" )
       sys.exit(2)
    processAllScenes( m_stk2_root_path )   
    sys.exit( m_erroNumber )

def processAllScenes( m_options ):
    global m_erroNumber
    m_xrayResourcesPathModels = m_stk2_root_path +'/resources/sources/scenes'
    try:
        maya.standalone.initialize( name = 'python' )
    except:
        sys.stderr.write( "Failed in initialize standalone application" )
        raise
    m_errorHandle = m_meshTrace() # init error tracer
    m_errorHandle.raiseMessage('----------------------------------------------')
    m_errorHandle.raiseMessage( time.strftime("Exporter Message Log start in %d/%m/%Y %H:%M:%S", time.localtime()) )
    m_mayaFiles = m_filesMaya()
    m_mayaFiles = m_mayaFiles.getAllMayaFiles( m_xrayResourcesPathModels ) # get all maya files stored in the specified folder
    m_mayaFiles = getFormattedStringList( m_mayaFiles ) 
    try:
        cmds.loadPlugin( g_xrayPlugName )   # try to load xray plugin
    except:
        m_errorHandle.raiseMessage('         Failed to load Xray plugin         ')
        raise
    if ( None == getRealXrayResourcesPath( 'models' ) ):
        m_errorHandle.raiseMessage('         Failed to load Xray plugin         ')
        raise 
    # Add Callback
    m_cb_id = OpenMaya.MSceneMessage.addCheckFileCallback( OpenMaya.MSceneMessage.kBeforeReferenceCheck, BeforeReferenceCheckCallback )    
    # init additinal data for specific exporting
    m_meshNodeInst = m_meshNode()
    #-------------------------------------------------------------------------------   
    for m_mayaFile in m_mayaFiles:
        if ( os.path.exists(m_mayaFile.rstrip()) ):
            try:
                print( "Next File :%s" %m_mayaFile.rstrip())
                cmds.file( m_mayaFile.rstrip() , open = True , type = 'mayaBinary', force = True )  # open file as mayaBinary
            except:
                m_errorHandle.raiseMessage( "ERROR Can't open file %s " %m_mayaFile )
                sys.stderr.write( "ERROR Can't open file %s " %m_mayaFile )
                m_erroNumber = 3
                continue
            # strat exporting
            if ( m_meshNodeInst.renameVisCol() ):
                cmds.file( save = True )
            # end exporting
        else:
            m_errorHandle.raiseMessage( "ERROR Can't find file %s " %m_mayaFile )
            m_erroNumber = 3
    #-------------------------------------------------------------------------------    
    try:
        cmds.unloadPlugin( g_xrayPlugName )
    except:
        m_errorHandle.raiseMessage( "ERROR Can't unload Xray plugin" )
    # Remove Callback
    OpenMaya.MMessage.removeCallback( m_cb_id )

# run main() function when run script
if __name__ == "__main__":
   main()