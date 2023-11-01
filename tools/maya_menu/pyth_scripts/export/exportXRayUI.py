#-----------------------------------------------------------------
#	SCRIPT			exportXRayUI.py
#	AUTHOR			Zaitsev Evgeniy
#				    ev.zaitsev@gmail.com
#	DATE:			15 July 2010
#
#	DESCRIPTION:	Script with common functions and global
#                   variables for exportr UIs
#
#       VERSION:	    201101
#
#-----------------------------------------------------------------
import maya.cmds as cmds    # work with maya commands
#-----------------------------------------------------------------------------------
#	global variables
#-----------------------------------------------------------------------------------
# tha name of the window
m_meshProportiesGuiID = "w_meshProportiesUI"
m_animProportiesGuiID = "w_animProportiesUI"
m_actorProportiesGuiID = "w_actorProportiesUI"
# checkbox color
m_checkBoxColor = (0.087,0.254,0.161)
m_multiselColor = (0.35,0.20,0.20)