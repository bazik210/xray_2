////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "grass_instance.h"
#include "grass_template.h"

namespace xray {
namespace render {



grass_instance::grass_instance(u32 const				in_id, 
							   grass_template*			in_tempalate, 
							   math::color const&		in_color, 
							   math::float4x4 const&	in_transform):
	m_index(in_id),
	m_template(in_tempalate),
	m_color(in_color),
	m_transform(in_transform)
{
	
}

} // namespace render
} // namespace xray

