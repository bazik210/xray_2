////////////////////////////////////////////////////////////////////////////
//	Created		: 01.02.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef XRAY_ANIMATION_ANIMATION_MIX_INLINE_H_INCLUDED
#define XRAY_ANIMATION_ANIMATION_MIX_INLINE_H_INCLUDED

namespace xray {
namespace animation {

	inline float sum_weights( const mixing_animation_type*	blend_vector, u32 size )
	{
		mixing_animation_type const*			i = blend_vector;
		mixing_animation_type const* const	e = blend_vector + size;
		float ret = 0;
		for ( ; i != e; ++i )
			ret += ( *i ).factor();
		return ret;
	}
	
	inline void check_blends( const mixing_animation_type	*blend_vector, u32 size )
	{
		R_ASSERT_U( math::is_similar( sum_weights( blend_vector, size ), 1.f ) );
	} 

	inline	void animation_mix::add( const animation_layer &blend )
	{ 
	//	if ( m_blend_vector.empty() )
			//m_mix_time = blend.max_time();
		 
		//m_mix_time = math::min( m_mix_time, blend.max_time() );

		if ( !blend.is_additive() )
		{
			add_full( blend );
			//m_blend_additive_vector.push_back( blend );
			return;
		
		}

		add_additive( blend );
		//m_blend_vector.push_back( blend ); 

		//R_ASSERT_CMP(  sum_weights( m_blend_vector ), <=, 1.f + math::epsilon_7 );

#if 0	
		m_dbg_times.push_back(dbg_times());
#endif
	}

	inline	void		animation_mix::add_full					( const animation_layer &animation )
	{
		ASSERT( m_animations_count < max_mixing_animations );	
		m_animations[m_animations_count] = animation;
		++m_animations_count;
		
	}

	inline	void		animation_mix::add_additive				( const animation_layer &animation )
	{
		ASSERT(m_additive_animations_count < max_additive_mixing_animations );	
		m_additive_animations[m_additive_animations_count] = animation;
		++m_additive_animations_count;
	}


	inline	mixing_animation_type& animation_mix::animation_additive( u32 id )
	{
		ASSERT( id < m_additive_animations_count );
		return m_additive_animations[ id ];
	}

	inline	mixing_animation_type const& animation_mix::animation_additive	( u32 id )const
	{
		ASSERT( id < m_additive_animations_count );
		return m_additive_animations[ id ];
	}

	inline	mixing_animation_type& animation_mix::animation_full			( u32 id )
	{
		ASSERT( id < m_animations_count );
		return m_animations[ id ];
	}
	inline	mixing_animation_type const& animation_mix::animation_full		( u32 id )const
	{
		ASSERT( id < m_animations_count );	
		return m_animations[ id ];
	}


	inline void set_root_start( const float4x4 &m, mixing_animation_type	*blend_vector, u32 size )
	{
		const u32 blends_count = size;
		//frame const start_frame = matrix_to_frame( m );
		for ( u32 i = 0; i < blends_count; ++i )
				blend_vector[i].set_root_start( m );
		
	}
	
	inline void	matrix_to_frame( const float4x4 &matrix, frame &f )
	{

		f.translation	= matrix.c.xyz();
		f.rotation		= matrix.get_angles_xyz( ); //float3(0,0,0);//
		f.scale			= float3(1,1,1);
	}

	inline	void animation_mix::set_root_start( const float4x4 &f )
	{
		animation::set_root_start( f, &m_animations[0], m_animations_count );
	}

	inline	void animation_mix::get_root_matrix( float4x4 &bone_matrix, float time, const float3 &parent_scale_, float3 &scale_ )const
	{
		//buffer_vector<mixing_animation_type> v ( pvoid(m_animations), max_mixing_animations, m_animations_count );
		animation::get_bone_matrix(  bone_matrix, m_animations, m_animations_count, u32(0),
							  time, parent_scale_, scale_ );
	}

	rotation_mixing_method mixing_rotation();
	
	template< typename bone_index_type >
	inline void get_bone_matrix( float4x4 &m, const mixing_animation_type *blends, u32 blends_cnt,  bone_index_type bone, float time, const float3 &parent_scale_, float3 &scale_ )
	{
		
		ASSERT( !blends_cnt == 0 );

		//if ( blends.empty() )
			//return;

		//if ( blends[0].is_moving_root( bone ) )
		//{
		//	m = linear_angles_mix( blends, bone, time, parent_scale_, scale_ );
		//	return;
		//}

		switch( mixing_rotation( ) )
		{
			
			case rotation_mixing_angles:
				m = linear_angles_mix( blends, blends_cnt, bone, time, parent_scale_, scale_ );
			break;

			case rotation_mixing_angles_representation_reduce:
				m = linear_angles_mix_representation_reduce( blends, blends_cnt, bone, time, parent_scale_, scale_ );
			break;

			case rotation_mixing_qslim:
				m = qslim_mix( blends, blends_cnt, bone, time );
			break;

			case rotation_mixing_sasquatch:
				m = sasquatch_mix( blends, blends_cnt, bone, time );
			break;

			case rotation_mixing_multilinear:
				m = multilinear_mix( blends, blends_cnt, bone, time );
			break;
				
			default: NODEFAULT();

		}

	}

	//template< class TypeRenderer >
	//void draw_animation_mix_model( render::render_model_instance_ptr model, TypeRenderer& renderer, animation_mix const &mix, float time, float4x4 const &object_transform  )
	//{
	//	
	//	const u32 count				= mix.bone_count();
	//	buffer_vector<float4x4>		matrices( ALLOCA( count * sizeof(float4x4) ), count );
	//	mix.calculate_bones			( time, matrices );

	//	renderer.update_model		( model, object_transform);
	//	renderer.update_skeleton	( model, &matrices[1], matrices.size()-1 );
	//}

	


} // namespace animation
} // namespace xray

#endif // #ifndef XRAY_ANIMATION_ANIMATION_MIX_INLINE_H_INCLUDED
