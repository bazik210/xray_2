////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef INDEXED_POLYGON_RENDERABLE_H_INCLUDED
#define INDEXED_POLYGON_RENDERABLE_H_INCLUDED

namespace xray {

namespace render {
namespace debug {
	class renderer;
} //namespace debug
} // namespace render
namespace model_editor {
class indexed_polygon;
void	render_indexed_polygon	( indexed_polygon const* polygon, render::scene_ptr const& scene, render::debug::renderer& r, math::color const& c, bool solid = false, bool double_sided = false );

} // namespace model_editor
} // namespace xray

#endif // #ifndef INDEXED_POLYGON_RENDERABLE_H_INCLUDED