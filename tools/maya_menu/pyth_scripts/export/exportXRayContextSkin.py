# -----------------------------------------------------------------
# SCRIPT			exportXRayContextSkin.py
# AUTHOR			Zaitsev Evgeniy
#                   ev.zaitsev@gmail.com
# DATE:			21 October 2010
#
# DESCRIPTION:	Define tools for skin exporter
#                   S.T.A.L.K.E.R. 2 project
#
#  VERSION:	        201102
#
# -----------------------------------------------------------------
import maya.OpenMaya as OpenMaya    # work with maya API
import maya.cmds as cmds            # work with maya commands
import maya.mel as mel              # work with mel
import string                       # string parser
from exportXRayContext import *     # common functions
# -----------------------------------------------------------------------------------
# SKIN NODE CLASS
# -----------------------------------------------------------------------------------


class m_skinNode(object):
    """
    store a dictionary of objects
    """

    def __init__(self):
        """
        initialize the default attributes
        """
        self.errorHandle = m_meshTrace()  # error tracer

    def getNode(self):
        """
        find in maya scene all MFn.kTransform's with '_201102_skin_skins' attribute
        RETURN: MSelectionList - list of MDagPath's to nodes with attr '_201102_skin_skins'
        """
        m_selectionList = OpenMaya.MSelectionList()    # create selectionList
        m_dagIterator = OpenMaya.MItDag(OpenMaya.MItDag.kBreadthFirst)
        while not m_dagIterator.isDone():  # iterate
            m_objCurr = m_dagIterator.currentItem()    # current item
            m_objPath = OpenMaya.MDagPath()            # define MDagPath
            m_dagIterator.getPath(m_objPath)
            if (m_objCurr.hasFn(OpenMaya.MFn.kTransform)):
                m_nodeFn = OpenMaya.MFnDependencyNode()
                m_nodeFn.setObject(m_objCurr)
                if (not m_nodeFn.isFromReferencedFile()):
                    try:
                        m_nodeFn.attribute('_201102_skin_skins')
                        m_nodeFn.attribute('_201102_skin_isSel')
                        m_nodeFn.attribute('_201102_skin_skeleton')
                        m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                        m_nodeFn.attribute('_201102_skin_saveDir')
                        m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                        m_selectionList.add(m_objPath)
                    except BaseException:
                        pass
            next(m_dagIterator)  # next item
        return m_selectionList

    def addObj(self, m_obj):
        """
        add '%s._201102_skin_skins' attribute to the m_obj node

        INPUT:  m_skins                   - String, full path to the node, like: '|group6|group5'
                m_isSel                   - set to True if object in list selected
                m_skeletonIsDefault       - is m_skeleton has Default value
                m_skeleton                - location of the skeleton
                m_saveDirIsDefault        - is m_saveDir has Default value
                m_saveDir                 - location of exported skins

        STORED: m_skins                   - '|group1|stalker1'
                m_isSel                   - False
                m_skeletonIsDefault       - True
                m_skeleton                - 'XRAY_RESOURCES_PATH/animations/skeletons/human'
                m_saveDirIsDefault        -
                m_saveDir                 - 'XRAY_RESOURCES_PATH/models'

        RETURN: True if object was added properly, False otherwise
        """
        m_isSel = False
        m_skeletonIsDefault = True
        m_skeleton = m_skin_skeletonDefault
        m_saveDirIsDefault = True
        m_saveDir = m_skin_saveDirDefault
        # -----------------------------------------
        m_objPath = getMDagPathFromNodeName(m_obj)
        if (not m_objPath):
            self.errorHandle.raiseMessage("Can't find object %s" % m_obj)
            return False
        m_nodeMObj = m_objPath.node()
        if (not m_nodeMObj.hasFn(OpenMaya.MFn.kTransform)):
            self.errorHandle.raiseMessage("Select Transform node please")
            return False
        m_nodeFn = OpenMaya.MFnDependencyNode()
        try:
            m_nodeFn.setObject(m_nodeMObj)
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't set MObject() to MFnDependencyNode()")
            return False
        try:
            m_nodeFn.attribute('_201102_skin_skins')
            m_nodeFn.attribute('_201102_skin_isSel')
            m_nodeFn.attribute('_201102_skin_skeleton')
            m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
            m_nodeFn.attribute('_201102_skin_saveDir')
            m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
            self.errorHandle.raiseMessage(
                "Attribute %s._201102_skin_skins already exists" %
                m_nodeFn.name())
            return False
        except BaseException:
            pass
        m_nodeName = m_objPath.fullPathName()
        try:
            cmds.addAttr(m_nodeName, longName='_201102_skin_skins',
                         niceName='Objects',
                         attributeType='compound',
                         numberOfChildren=5)
            cmds.addAttr(
                m_nodeName,
                parent='_201102_skin_skins',
                longName='_201102_skin_isSel',
                niceName='Is Selected',
                attributeType='bool')
            cmds.addAttr(
                m_nodeName,
                parent='_201102_skin_skins',
                longName='_201102_skin_skeleton',
                niceName='Skeleton',
                dataType='string')
            cmds.addAttr(
                m_nodeName,
                parent='_201102_skin_skins',
                longName='_201102_skin_skeletonIsDefault',
                niceName='Skeleton Is Default',
                attributeType='bool')
            cmds.addAttr(
                m_nodeName,
                parent='_201102_skin_skins',
                longName='_201102_skin_saveDir',
                niceName='Save to',
                dataType='string')
            cmds.addAttr(
                m_nodeName,
                parent='_201102_skin_skins',
                longName='_201102_skin_saveDirIsDefault',
                niceName='Save to Is Default',
                attributeType='bool')
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't add attribute '_201102_skeletons' to node %s" %
                m_nodeName)
            return False
        # ----------- unlock --------------
        cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=False)
        cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=False)
        cmds.setAttr('%s._201102_skin_skeleton' % m_nodeName, lock=False)
        cmds.setAttr(
            '%s._201102_skin_skeletonIsDefault' %
            m_nodeName, lock=False)
        cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=False)
        cmds.setAttr(
            '%s._201102_skin_saveDirIsDefault' %
            m_nodeName, lock=False)
        # ------------- set ---------------
        cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, m_isSel)
        cmds.setAttr(
            '%s._201102_skin_skeleton' %
            m_nodeName,
            m_skeleton,
            type='string')
        cmds.setAttr(
            '%s._201102_skin_skeletonIsDefault' %
            m_nodeName, m_skeletonIsDefault)
        cmds.setAttr(
            '%s._201102_skin_saveDir' %
            m_nodeName,
            m_saveDir,
            type='string')
        cmds.setAttr(
            '%s._201102_skin_skeletonIsDefault' %
            m_nodeName, m_saveDirIsDefault)
        # ------------ lock ---------------
        cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=True)
        cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=True)
        cmds.setAttr('%s._201102_skin_skeleton' % m_nodeName, lock=True)
        cmds.setAttr(
            '%s._201102_skin_skeletonIsDefault' %
            m_nodeName, lock=True)
        cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=True)
        cmds.setAttr(
            '%s._201102_skin_saveDirIsDefault' %
            m_nodeName, lock=True)
        return True

    def delObj(self, m_obj):
        """
        delete '%s._201102_skin_skins' attribute from the m_objPath node
        INPUT:  m_obj     - String, full path to the node, like: '|group6|group5'
        RETURN: True if attr was deleted from the node, False otherwise
        """
        m_objPath = getMDagPathFromNodeName(m_obj)
        if (not m_objPath):
            self.errorHandle.raiseMessage("Can't find object %s" % m_obj)
            return False
        m_nodeMObj = m_objPath.node()
        m_nodeFn = OpenMaya.MFnDependencyNode()
        try:
            m_nodeFn.setObject(m_nodeMObj)
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't set MObject() to MFnDependencyNode()")
            return False
        try:
            m_nodeFn.attribute('_201102_skin_skins')
            m_nodeFn.attribute('_201102_skin_isSel')
            m_nodeFn.attribute('_201102_skin_skeleton')
            m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
            m_nodeFn.attribute('_201102_skin_saveDir')
            m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
            m_nodeName = m_objPath.fullPathName()
            # ----------- unlock --------------
            cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_skeleton' % m_nodeName, lock=False)
            cmds.setAttr(
                '%s._201102_skin_skeletonIsDefault' %
                m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=False)
            cmds.setAttr(
                '%s._201102_skin_saveDirIsDefault' %
                m_nodeName, lock=False)
            # ----------- delete --------------
            cmds.deleteAttr('%s._201102_skin_skins' % m_nodeName)
            return True
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't find %s._201102_skin_skins attribute" %
                m_nodeName)
        return False

    def count(self):
        """
        RETURN: count of nodes with '%s._201102_skel_skeletons' attribute
        """
        m_selectionList = self.getNode()
        return m_selectionList.length()

    def printObj(self):
        """
        read node and print the content of the dictionary, for debug purpose only
        """
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            m_listIt.getDagPath(m_objPath)  # get current Dag
            m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
            m_nodeName = m_objPath.fullPathName()  # get full string path to node
            try:
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                self.errorHandle.raiseMessage(
                    "KEY: %s <==> SEL: %s SKEL: %s SKELDEF %s DIR %s DIRDEF %s" %
                    (m_nodeName,
                     cmds.getAttr(
                         '%s._201102_skin_isSel' %
                         m_nodeName),
                        cmds.getAttr(
                         '%s._201102_skin_skeleton' %
                         m_nodeName),
                        cmds.getAttr(
                         '%s._201102_skin_skeletonIsDefault' %
                         m_nodeName),
                        cmds.getAttr(
                         '%s._201102_skin_saveDir' %
                         m_nodeName),
                        cmds.getAttr(
                         '%s._201102_skin_saveDirIsDefault' %
                         m_nodeName)))
            except BaseException:
                pass
            next(m_listIt)

    def lockAllItems(self):
        """
        lock all items in the node
        """
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            m_listIt.getDagPath(m_objPath)  # get current Dag
            m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
            m_nodeName = m_objPath.fullPathName()  # get full string path to node
            try:
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                # ------------ lock ---------------
                cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=True)
                cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=True)
                cmds.setAttr(
                    '%s._201102_skin_skeleton' %
                    m_nodeName, lock=True)
                cmds.setAttr(
                    '%s._201102_skin_skeletonIsDefault' %
                    m_nodeName, lock=True)
                cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=True)
                cmds.setAttr(
                    '%s._201102_skin_saveDirIsDefault' %
                    m_nodeName, lock=True)
            except BaseException:
                pass
            next(m_listIt)

    def unlockAllItems(self):
        """
        unlock all items in the node
        """
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            m_listIt.getDagPath(m_objPath)  # get current Dag
            m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
            m_nodeName = m_objPath.fullPathName()  # get full string path to node
            try:
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                # ----------- unlock --------------
                cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=False)
                cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=False)
                cmds.setAttr(
                    '%s._201102_skin_skeleton' %
                    m_nodeName, lock=False)
                cmds.setAttr(
                    '%s._201102_skin_skeletonIsDefault' %
                    m_nodeName, lock=False)
                cmds.setAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName, lock=False)
                cmds.setAttr(
                    '%s._201102_skin_saveDirIsDefault' %
                    m_nodeName, lock=False)
            except BaseException:
                pass
            next(m_listIt)

    def setAttribute(self, m_obj, m_attrName, m_attrValue):
        """
        INPUT:  m_obj       - key value ( example: '|group4|group3' )
                m_attrName  - name of the attribute
                m_attrValue - value stored at this attribute
        RETURN: True if m_attrValue was set, False otherwise
        USAGE:  my_node.setAttribute( '|group4', '_201102_skin_isSel', True )
        """
        m_objPath = getMDagPathFromNodeName(m_obj)
        if (not m_objPath):
            self.errorHandle.raiseMessage("Can't find object %s" % m_obj)
            return False
        m_nodeMObj = m_objPath.node()
        m_nodeFn = OpenMaya.MFnDependencyNode()
        m_nodeName = m_objPath.fullPathName()  # get full path to node
        try:
            m_nodeFn.setObject(m_nodeMObj)
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't set MObject() to MFnDependencyNode()")
            return False
        try:
            m_nodeFn.attribute('_201102_skin_skins')
            m_nodeFn.attribute('_201102_skin_isSel')
            m_nodeFn.attribute('_201102_skin_skeleton')
            m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
            m_nodeFn.attribute('_201102_skin_saveDir')
            m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't find %s._201102_skin_skins attribute" %
                m_nodeName)
            return False

        # ----------- unlock --------------
        try:
            cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_skeleton' % m_nodeName, lock=False)
            cmds.setAttr(
                '%s._201102_skin_skeletonIsDefault' %
                m_nodeName, lock=False)
            cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=False)
            cmds.setAttr(
                '%s._201102_skin_saveDirIsDefault' %
                m_nodeName, lock=False)
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't unlock %s._201102_skin_skins attribute" %
                m_nodeName)
            return False
        # -----------------------------------------------------------------------------------------------
        # _201102_skin_isSel
        # -----------------------------------------------------------------------------------------------
        if ("_201102_skin_isSel" == m_attrName):
            m_isSelected = bool(m_attrValue)
            try:
                cmds.setAttr(
                    '%s._201102_skin_isSel' %
                    m_nodeName, m_isSelected)
            except BaseException:
                self.errorHandle.raiseMessage(
                    "Can't setAttr %s._201102_skin_isSel" %
                    m_nodeName)
        # -----------------------------------------------------------------------------------------------
        # _201102_skin_saveDir
        # -----------------------------------------------------------------------------------------------
        if ("_201102_skin_saveDir" == m_attrName):
            m_saveDir = m_attrValue
            try:
                if (m_saveDir != m_skin_saveDirDefault):
                    m_saveDirIsDefault = False
                else:
                    m_saveDirIsDefault = True
                cmds.setAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName, m_saveDir, type='string')
                cmds.setAttr(
                    '%s._201102_skin_saveDirIsDefault' %
                    m_nodeName, m_saveDirIsDefault)
            except BaseException:
                self.errorHandle.raiseMessage(
                    "Can't setAttr %s._201102_skin_saveDir" %
                    m_nodeName)
        # -----------------------------------------------------------------------------------------------
        # _201102_skin_skeleton
        # -----------------------------------------------------------------------------------------------
        if ("_201102_skin_skeleton" == m_attrName):
            m_skeleton = m_attrValue
            try:
                if (m_skeleton != m_skin_skeletonDefault):
                    m_skeletonIsDefault = False
                else:
                    m_skeletonIsDefault = True
                cmds.setAttr(
                    '%s._201102_skin_skeleton' %
                    m_nodeName, m_skeleton, type='string')
                cmds.setAttr(
                    '%s._201102_skin_skeletonIsDefault' %
                    m_nodeName, m_skeletonIsDefault)
            except BaseException:
                self.errorHandle.raiseMessage(
                    "Can't setAttr %s._201102_skin_skeleton" %
                    m_nodeName)
        # ------------- lock --------------
        try:
            cmds.setAttr('%s._201102_skin_skins' % m_nodeName, lock=True)
            cmds.setAttr('%s._201102_skin_isSel' % m_nodeName, lock=True)
            cmds.setAttr('%s._201102_skin_skeleton' % m_nodeName, lock=True)
            cmds.setAttr(
                '%s._201102_skin_skeletonIsDefault' %
                m_nodeName, lock=True)
            cmds.setAttr('%s._201102_skin_saveDir' % m_nodeName, lock=True)
            cmds.setAttr(
                '%s._201102_skin_saveDirIsDefault' %
                m_nodeName, lock=True)
        except BaseException:
            self.errorHandle.raiseMessage(
                "Can't lock %s._201102_skin_skins attribute" %
                m_nodeName)
        # ---------------------------------
        return True

    def setSaveDir(self, m_obj, m_saveDir):
        """
        set save directory
        INPUT:  m_obj     - key value (example: '|group4|group3')
                m_saveDir - string value, path to folder for save current obj
        RETURN: True if saveDir was set, False otherwise
        USAGE:  my_node.setSaveDir( '|group4', 'XRAY_RESOURCES_PATH/models' )
        """
        if (self.setAttribute(m_obj, '_201102_skin_saveDir', m_saveDir)):
            return True
        else:
            return False

    def setSelectionState(self, m_obj, m_isSelected=False):
        """
        set to True if object in list selected
        INPUT:  m_obj        - key value from list self.obj (example: '|group4|group3')
                m_isSelected - boolean value, True - selected, False - unselected
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setSelectionState( '|group4', False )
        """
        if (self.setAttribute(m_obj, '_201102_skin_isSel', m_isSelected)):
            return True
        else:
            return False

    def setSkeleton(self, m_obj, m_skeleton):
        """
        set to True if object in list selected
        INPUT:  m_obj        - key value from list self.obj (example: '|group4|group3')
                m_skeleton   - string, path to the skeleton (example: 'XRAY_RESOURCES_PATH/animations/skeletons/human' )
        RETURN: True if selection state was defined, False otherwise
        USAGE:  my_node.setSkeleton( '|group4', 'XRAY_RESOURCES_PATH/animations/skeletons/human' )
        """
        if (self.setAttribute(m_obj, '_201102_skin_skeleton', m_skeleton)):
            return True
        else:
            return False

    def iterItems(self):
        """
        you can iterate in program using this method
        RETURN: hash all data stored in the node
        """
        # ------------------------------------------------------------
        if (0 == OpenMaya.MGlobal_mayaState()):  # MAIN PROGRESS BAR
            gMainProgressBar = mel.eval('$tmp = $gMainProgressBar')
            cmds.progressBar(gMainProgressBar,
                             edit=True,
                             beginProgress=False,
                             isInterruptable=False,
                             maxValue=100)
            cmds.progressBar(
                gMainProgressBar,
                edit=True,
                beginProgress=True)  # BEGIN PROGRESS
        # ------------------------------------------------------------
        m_returnHash = {}
        # [ maxLen( getShortName( m_obj ) ), maxLen( m_obj ), maxLen( m_objFilename ), maxLen( m_skelFilename ) ]
        m_maxLenght = self.getMaxLenList()
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            try:
                m_listIt.getDagPath(m_objPath)  # get current Dag
                m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                # -----------------------------
                m_nodeName = m_objPath.fullPathName()  # get full string path to node
                m_nodeNamePartial = getShortName(m_nodeName)
                m_isSelected = cmds.getAttr(
                    '%s._201102_skin_isSel' % m_nodeName)
                m_saveDir = cmds.getAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName)
                m_saveDirIsDefault = cmds.getAttr(
                    '%s._201102_skin_saveDirIsDefault' % m_nodeName)
                m_skeleton = cmds.getAttr(
                    '%s._201102_skin_skeleton' % m_nodeName)
                m_skeletonIsDefault = cmds.getAttr(
                    '%s._201102_skin_skeletonIsDefault' % m_nodeName)
                if (m_saveDirIsDefault):
                    m_saveDir = m_skin_saveDirDefault
                if (m_skeletonIsDefault):
                    m_skeleton = m_skin_skeletonDefault
                m_saveDir = m_saveDir.replace('XRAY_RESOURCES_PATH', '', 1)
                m_skeleton = m_skeleton.replace('XRAY_RESOURCES_PATH', '', 1)
                m_objFilename = "%s/%s.model" % (m_saveDir, m_nodeNamePartial)
                m_skelFilename = "%s.skeleton" % m_skeleton
                # -----------------------------
                m_list = []
                m_list.append(
                    addSpacesToString(
                        m_nodeNamePartial,
                        m_maxLenght[0] -
                        len(m_nodeNamePartial)))    # 'stalker1'
                m_list.append(
                    addSpacesToString(
                        m_nodeName,
                        m_maxLenght[1] -
                        len(m_nodeName)))           # '|group1|stalker1'
                m_list.append(
                    addSpacesToString(
                        m_objFilename,
                        m_maxLenght[2] -
                        len(m_objFilename)))        # '/models/actors/stalker1.model'
                m_list.append(
                    addSpacesToString(
                        m_skelFilename,
                        m_maxLenght[3] -
                        len(m_skelFilename)))       # '/animations/skeletons/human.skeleton'
                # return string 'stalker1    |group1|stalker1
                # /models/actors/stalker1.model
                # /animations/skeletons/human.skeleton'
                m_returnHash[m_nodeName] = (
                    bool(m_isSelected),
                    m_saveDir,
                    m_skelFilename,
                    string.join(
                        m_list,
                        '    '))
            except BaseException:
                self.errorHandle.raiseMessage("m_skeletonNode.iterItems Error")
                pass
            # ------------------------------------------------------------
            if (0 == OpenMaya.MGlobal_mayaState()):  # INCREASE PROGRESS STEP
                cmds.progressBar(
                    gMainProgressBar,
                    edit=True,
                    step=100 /
                    m_selectionList.length())
            next(m_listIt)
        # ------------------------------------------------------------
        if (0 == OpenMaya.MGlobal_mayaState()):
            cmds.progressBar(
                gMainProgressBar,
                edit=True,
                endProgress=True)  # END PROGRESS
        return m_returnHash

    def getObjLabelFromNode(self, m_objInput):
        """
        INPUT:  m_obj - key value (example: '|props|box|box_01')
        RETURN: string like 'stalker1    |group1|stalker1    /models/actors/stalker1.model   /animations/skeletons/neutral_01.skeleton'
        USAGE:  my_node.getObjLabelFromNode( '|props|box|box_01' )
        """
        m_maxLenght = self.getMaxLenList()
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            try:
                m_listIt.getDagPath(m_objPath)  # get current Dag
                m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                m_nodeName = m_objPath.fullPathName()  # get full string path to node
                m_nodeNamePartial = getShortName(m_nodeName)
                m_isSelected = cmds.getAttr(
                    '%s._201102_skin_isSel' % m_nodeName)
                m_saveDir = cmds.getAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName)
                m_saveDirIsDefault = cmds.getAttr(
                    '%s._201102_skin_saveDirIsDefault' % m_nodeName)
                m_skeleton = cmds.getAttr(
                    '%s._201102_skin_skeleton' % m_nodeName)
                m_skeletonIsDefault = cmds.getAttr(
                    '%s._201102_skin_skeletonIsDefault' % m_nodeName)
                if (m_saveDirIsDefault):
                    m_saveDir = m_skin_saveDirDefault
                if (m_skeletonIsDefault):
                    m_skeleton = m_skin_skeletonDefault
                m_saveDir = m_saveDir.replace('XRAY_RESOURCES_PATH', '', 1)
                m_skeleton = m_skeleton.replace('XRAY_RESOURCES_PATH', '', 1)
                m_objFilename = "%s/%s.model" % (m_saveDir, m_nodeNamePartial)
                m_skelFilename = "%s.skeleton" % m_skeleton
                # -----------------------------
                if (m_objInput == m_nodeName):
                    m_list = []
                    m_list.append(
                        addSpacesToString(
                            m_nodeNamePartial,
                            m_maxLenght[0] -
                            len(m_nodeNamePartial)))  # 'stalker1'
                    m_list.append(
                        addSpacesToString(
                            m_nodeName,
                            m_maxLenght[1] -
                            len(m_nodeName)))  # '|group1|stalker1'
                    m_list.append(
                        addSpacesToString(
                            m_objFilename,
                            m_maxLenght[2] -
                            len(m_objFilename)))  # '/models/actors/stalker1.model'
                    m_list.append(
                        addSpacesToString(
                            m_skelFilename,
                            m_maxLenght[3] -
                            len(m_skelFilename)))  # '/animations/skeletons/human.skeleton'
                    # return string 'stalker1    |group1|stalker1
                    # /models/actors/stalker1.model
                    # /animations/skeletons/human.skeleton'
                    return string.join(m_list, '    ')
            except BaseException:
                pass
            next(m_listIt)
        return None

    def getMaxLenList(self):
        """
        RETURN: [ maxLen( getShortName( m_obj ) ), maxLen( m_obj ), maxLen( m_objFilename ), maxLen( m_skelFilename ) ]
        """
        m_maxLenght = [0, 0, 0, 0]
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            try:
                m_listIt.getDagPath(m_objPath)  # get current Dag
                m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
                m_nodeFn.attribute('_201102_skin_skins')
                m_nodeFn.attribute('_201102_skin_isSel')
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')
                m_nodeName = m_objPath.fullPathName()  # get full string path to node
                m_nodeNamePartial = getShortName(m_nodeName)
                m_isSelected = cmds.getAttr(
                    '%s._201102_skin_isSel' % m_nodeName)
                m_saveDir = cmds.getAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName)
                m_saveDirIsDefault = cmds.getAttr(
                    '%s._201102_skin_saveDirIsDefault' % m_nodeName)
                m_skeleton = cmds.getAttr(
                    '%s._201102_skin_skeleton' % m_nodeName)
                m_skeletonIsDefault = cmds.getAttr(
                    '%s._201102_skin_skeletonIsDefault' % m_nodeName)
                if (m_saveDirIsDefault):
                    m_saveDir = m_skin_saveDirDefault
                if (m_skeletonIsDefault):
                    m_skeleton = m_skin_skeletonDefault
                m_saveDir = m_saveDir.replace('XRAY_RESOURCES_PATH', '', 1)
                m_skeleton = m_skeleton.replace('XRAY_RESOURCES_PATH', '', 1)
                m_objFilename = "%s/%s.model" % (m_saveDir, m_nodeNamePartial)
                m_skelFilename = "%s.skeleton" % m_skeleton
                # -----------------------------
                # find maxLen strings
                if (m_maxLenght[0] < len(m_nodeNamePartial)):
                    m_maxLenght[0] = len(m_nodeNamePartial)
                if (m_maxLenght[1] < len(m_nodeName)):
                    m_maxLenght[1] = len(m_nodeName)
                if (m_maxLenght[2] < len(m_objFilename)):
                    m_maxLenght[2] = len(m_objFilename)
                if (m_maxLenght[3] < len(m_skelFilename)):
                    m_maxLenght[3] = len(m_skelFilename)
            except BaseException:
                pass
            next(m_listIt)
        return m_maxLenght

    def exportDataNoDBSel(self):
        self.exportData(True, True)

    def exportDataNoDBAll(self):
        self.exportData(False, True)

    def exportDataAll(self):
        self.exportData(False, False)

    def exportDataSel(self):
        self.exportData(True, False)

    def exportData(self, m_onlySelected=False, m_ndb=False):
        """
        INPUT:  m_onlySelected - boolean, if set to True export only selected objects from the list
                                          if set to False export all objects from the list
        RETURN: None if XRAY_RESOURCES_PATH directory don't exists
        """
        m_erroNumber = 0
        if (None is getRealXrayResourcesPath('skeletons')
                or None is getRealXrayResourcesPath('models')):
            self.errorHandle.raiseMessage("XRAY_RESOURCES_PATH don't exists")
            return None
        # ------------------------------------------------------------
        if (0 == OpenMaya.MGlobal_mayaState()):  # MAIN PROGRESS BAR
            gMainProgressBar = mel.eval('$tmp = $gMainProgressBar')
            cmds.progressBar(gMainProgressBar,
                             edit=True,
                             beginProgress=False,
                             isInterruptable=False,
                             maxValue=100)
            cmds.progressBar(
                gMainProgressBar,
                edit=True,
                beginProgress=True)  # BEGIN PROGRESS
        # ------------------------------------------------------------
        m_selectionList = self.getNode()
        m_listIt = OpenMaya.MItSelectionList(m_selectionList)
        while not m_listIt.isDone():
            m_objPath = OpenMaya.MDagPath()
            m_nodeFn = OpenMaya.MFnDependencyNode()
            try:
                m_listIt.getDagPath(m_objPath)  # get current Dag
                m_nodeFn.setObject(m_objPath.node())  # set MFn to MObj
                m_nodeFn.attribute('_201102_skin_skins')  # '|group1|stalker1'
                m_nodeFn.attribute('_201102_skin_isSel')  # True
                # 'XRAY_RESOURCES_PATH/animations/skeletons/human'
                m_nodeFn.attribute('_201102_skin_skeleton')
                m_nodeFn.attribute('_201102_skin_skeletonIsDefault')  # True
                # 'XRAY_RESOURCES_PATH/models/actors'
                m_nodeFn.attribute('_201102_skin_saveDir')
                m_nodeFn.attribute('_201102_skin_saveDirIsDefault')  # False
                m_obj = m_objPath.fullPathName()  # get full string path to node
                m_nodeName = m_obj
                m_nodeNamePartial = getShortName(m_nodeName)
                m_isSelected = cmds.getAttr(
                    '%s._201102_skin_isSel' % m_nodeName)
                m_saveDir = cmds.getAttr(
                    '%s._201102_skin_saveDir' %
                    m_nodeName)
                m_saveDirIsDefault = cmds.getAttr(
                    '%s._201102_skin_saveDirIsDefault' % m_nodeName)
                m_skeleton = cmds.getAttr(
                    '%s._201102_skin_skeleton' % m_nodeName)
                m_skeletonIsDefault = cmds.getAttr(
                    '%s._201102_skin_skeletonIsDefault' % m_nodeName)
                if (m_saveDirIsDefault):
                    m_saveDir = m_skin_saveDirDefault
                if (m_skeletonIsDefault):
                    m_skeleton = m_skin_skeletonDefault
                m_objNameHash = m_saveDir.replace(
                    m_xrayResourcesPathMeshes,
                    '',
                    1) + '/' + m_nodeNamePartial   # '/actors/stalker1'
                m_skeleton = m_skeleton.replace(
                    m_xrayResourcesPathSkeletons, '', 1)
                # 'stalker1    |group1|stalker1    /models/actors/stalker1.model   /animations/skeletons/human.skeleton'
                m_objString = self.getObjLabelFromNode(m_obj)
            except BaseException:
                self.errorHandle.raiseMessage(
                    "m_skinNode.exportData %s item Error" % m_obj)
            else:
                # -------------------------------
                if (not m_onlySelected):  # reload all objects
                    m_isSelected = True
                if (bool(m_isSelected)):
                    m_objFilename = getRealXrayResourcesPath(
                        'models') + m_objNameHash
                    m_skelFilename = getRealXrayResourcesPath(
                        'skeletons') + m_skeleton
                    # self.errorHandle.raiseMessage("%s" %m_skelFilename)
                    m_objFilename = m_objFilename.lower()
                    if (cmds.objExists(m_nodeName)):
                        if (isMeshHasSkinCluster(m_nodeName)):
                            try:
                                cmds.DisableAll()
                                mel.eval('doEnableNodeItems false all;')
                                cmds.xray_skin_export(
                                    ndb=m_ndb,
                                    skin_visual=m_nodeName,
                                    outfilename=m_objFilename,
                                    skeleton=m_skelFilename)
                                cmds.EnableAll()
                                mel.eval('doEnableNodeItems true all;')
                                # self.errorHandle.raiseMessage( "   == > Successful %s" %m_objFilename )
                            except BaseException:
                                self.errorHandle.raiseMessage(
                                    "ERROR   == >  Can't export %s to %s" %
                                    (m_nodeName, m_objFilename))
                                m_erroNumber = 4
                        else:
                            self.errorHandle.raiseMessage(
                                "ERROR   == > Can't find any skinCluster %s" % m_nodeName)
                            m_erroNumber = 4
                    else:
                        self.errorHandle.raiseMessage(
                            "ERROR   == > Can't find object %s in scene" % m_nodeName)
                        m_erroNumber = 4
            # ------------------------------------------------------------
            if (0 == OpenMaya.MGlobal_mayaState()):  # INCREASE PROGRESS STEP
                cmds.progressBar(
                    gMainProgressBar,
                    edit=True,
                    step=100 /
                    m_selectionList.length())
            next(m_listIt)
        # ------------------------------------------------------------
        if (0 == OpenMaya.MGlobal_mayaState()):
            cmds.progressBar(
                gMainProgressBar,
                edit=True,
                endProgress=True)  # END PROGRESS
        return m_erroNumber
