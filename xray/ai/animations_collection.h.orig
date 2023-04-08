////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_AI_ANIMATION_TYPES_H_INCLUDED
#define XRAY_AI_ANIMATION_TYPES_H_INCLUDED

namespace xray {
namespace ai {

struct animations_collection
{
	animations_collection				( resources::managed_resource_ptr const& new_clip, pcstr filename );

	resources::managed_resource_ptr		clip;
	fs_new::virtual_path_string			name;
}; // struct animations_collection

struct animation_collection_wrapper
{
	inline animation_collection_wrapper	( animations_collection const* const collection ) :
		animation						( collection ),
		next							( 0 )
	{
	}
	
	animations_collection const*		animation;
	animation_collection_wrapper*		next;
};

typedef intrusive_list< animation_collection_wrapper,
						animation_collection_wrapper*,
						&animation_collection_wrapper::next >	animations_type;

typedef fixed_vector< animations_collection const*, 32 >		animations_collection_type;

} // namespace ai
} // namespace xray

#endif // #ifndef XRAY_AI_ANIMATION_TYPES_H_INCLUDED