////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define XRAY_STATIC_LIBRARIES

#define XRAY_LOG_MODULE_INITIATOR	"[!output PROJECT_NAME]"
#include <xray/extensions.h>

#include "[!output PROJECT_NAME]_memory.h"

namespace std {
	class ostream;
	class istream;
} // namespace std

// what this is for?
#define NT_PLUGIN

#include <xray/os_include.h>

#pragma warning(disable:4995)
#include <maya/MGlobal.h> 
#include <maya/MPxLocatorNode.h> 
#include <maya/MPxDragAndDropBehavior.h>
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/M3dView.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDagNode.h>
#include <maya/MPxManipContainer.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlugArray.h>
#include <maya/MAnimUtil.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItDag.h>
#include <maya/MFnSet.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnFreePointTriadManip.h>
#include <maya/MFnDistanceManip.h>
#include <maya/MMatrix.h>
#include <maya/MPxCommand.h>
#include <maya/MPxTransform.h>
#include <maya/MDGModifier.h>
#include <maya/MDagModifier.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MNodeMessage.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MFnExpression.h>
#include <maya/MFnIkJoint.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItGeometry.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>
#include <maya/MArgList.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFileObject.h>
#include <maya/MEulerRotation.h>
#include <maya/MAnimControl.h>

MStatus lock_attribute(const MFnDependencyNode& depNode, MStringArray attributes);

#endif // #ifndef PCH_H_INCLUDED