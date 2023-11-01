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
global m_erroNumber, m_stk2_root_path, m_file_stack
g_xrayPlugName = 'xray.maya.mll'
#-----------------------------------------------------------------------------------
def main():
    global m_erroNumber
    global m_stk2_root_path
    global m_file_stack
    m_file_stack = []
    m_erroNumber = 0

						
						
    m_stk2_root_path = None
    try:
        opts, args = getopt.getopt( sys.argv[1:], "o:",  [ 'option=', 
                                                           'mesh',
                                                           'skeleton',
                                                           'skin',
                                                           'anim',
                                                           'anim_add' ] )
    except getopt.GetoptError, err:
        print str( err ) # will print something like "option -a not recognized"
        sys.exit(2)
    m_options = [] # list of options	
    for m_option, m_value in opts:
        if m_option in ("-o", "--option"): m_stk2_root_path = m_value
        elif "--mesh"     == m_option: m_options.append( 'mesh' )
        elif "--skeleton" == m_option: m_options.append( 'skeleton' )
        elif "--skin"     == m_option: m_options.append( 'skin' )
        elif "--anim"     == m_option: m_options.append( 'anim' )
        elif "--anim_add" == m_option: m_options.append( 'anim_add' )
    if ( not m_stk2_root_path ):
       print ( "Specify project root directory" )
       sys.exit(2)

    processAllScenes( m_options )   
	

    print ("ERROR files:")
	
    for f in m_file_stack:
		print (f)	
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
    if m_options.count( "mesh" ):
        m_xrayResourcesPathMeshesConfig = getRealXrayResourcesPath( 'models' ) + '/categories.cfg' # files with object tags
        if (os.path.exists( m_xrayResourcesPathMeshesConfig )):
            os.remove( m_xrayResourcesPathMeshesConfig ) # remove file with tags
        m_meshNodeInst = m_meshNode()
    if m_options.count( "skeleton" ):
        m_skeletonNodeInst = m_skeletonNode()
    if m_options.count( "skin" ):
        m_skinNodeInst = m_skinNode()
    if m_options.count( "anim" ):
        m_animNodeInst = m_animNode()
    if m_options.count( "anim_add" ):
        m_animNodeInst = m_animNode()
    #-------------------------------------------------------------------------------   

	
    for m_mayaFile in m_mayaFiles:
        if ( os.path.exists(m_mayaFile.rstrip()) ):
            if( -1!=m_mayaFile.find('/temp/')):
                print( "Skip File :%s" %m_mayaFile.rstrip())
                continue

            try:
                print( "--------------------------------------------------")
                print( "Current File :%s" %m_mayaFile.rstrip())
                cmds.file( m_mayaFile.rstrip() , open = True , type = 'mayaBinary', force = True )  # open file as mayaBinary

            	# strat exporting
            	if m_options.count( "mesh" ):      doExportCommon(     m_meshNodeInst,     m_errorHandle, m_mayaFile.rstrip())
            	if m_options.count( "skeleton" ):  doExportCommon(     m_skeletonNodeInst, m_errorHandle, m_mayaFile.rstrip())
            	if m_options.count( "skin" ):      doExportCommon(     m_skinNodeInst,     m_errorHandle, m_mayaFile.rstrip())
            	if m_options.count( "anim" ):      doExportAnim(       m_animNodeInst,     m_errorHandle, m_mayaFile.rstrip())
            	if m_options.count( "anim_add" ):  doExportAnimAdd(    m_animNodeInst,     m_errorHandle, m_mayaFile.rstrip())
            	# end exporting

            except:
                m_errorHandle.raiseMessage( "ERROR Can't open file %s " %m_mayaFile )
                sys.stderr.write( "ERROR Can't open file %s " %m_mayaFile )
                #m_erroNumber = 3
                #sys.exit( m_erroNumber )
		m_file_stack.append(m_mayaFile.rstrip())
        else:
            m_errorHandle.raiseMessage( "ERROR Can't find file %s " %m_mayaFile )
            sys.exit( m_erroNumber )

    #-------------------------------------------------------------------------------    
    try:
        cmds.unloadPlugin( g_xrayPlugName )
    except:
        m_errorHandle.raiseMessage( "ERROR Can't unload Xray plugin" )
    # Remove Callback

    OpenMaya.MMessage.removeCallback( m_cb_id )

def doExportCommon( m_node, m_errorHandle, mayaFileName):
    global m_erroNumber
    global m_file_stack
    if ( m_node ):
        try:
            m_temp = m_node.exportData() # export all data stored in the file
            if ( 0 != m_temp ):          
				m_erroNumber = m_temp
				m_file_stack.append(mayaFileName)
        except:
            m_errorHandle.raiseMessage( "ERROR Can't exportData1" )
            m_erroNumber = 4
            m_file_stack.append(mayaFileName)

def doExportAnimCommon( m_node, m_errorHandle, m_mode, mayaFileName):
    global m_erroNumber
    global m_file_stack
    m_temp = 0	           
    if ( m_node ):        
        if ( m_node.getNode() ):  # check for existing 'xray_anim_lister' node in the file 
            try:
                if ( "anim" == m_mode ):      m_temp = m_node.exportDataIgnoreADD() # export all non-additive animations
                if ( "anim_add" == m_mode ):  m_temp = m_node.exportDataALL() # export all animations stored in the node
                if ( m_temp ):                
					m_erroNumber = m_temp
					m_file_stack.append(mayaFileName)

					
            except:
                m_errorHandle.raiseMessage( "ERROR Can't exportData2" )
                m_erroNumber = 4
                m_file_stack.append(mayaFileName)
          
def doExportAnim( m_node, m_errorHandle, mayaFileName):
    doExportAnimCommon( m_node, m_errorHandle, 'anim', mayaFileName)
             
def doExportAnimAdd( m_node, m_errorHandle, mayaFileName):
    doExportAnimCommon( m_node, m_errorHandle, 'anim_add', mayaFileName)

# run main() function when run script
if __name__ == "__main__":
   main()