////////////////////////////////////////////////////////////////////////////
//	Created		: 29.01.2010
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TREE_NODE_H_INCLUDED
#define TREE_NODE_H_INCLUDED

#include "tree_node_type.h"

using namespace System;
using namespace System::Windows::Forms;

namespace xray{
namespace editor{
namespace controls{

public ref class tree_node : public TreeNode
{
	typedef TreeNode super;
public:
	tree_node(String^ name)
		:super(name)
	{
		Name = name;
		m_image_index_collapsed		= -1;
		m_image_index_expanded		= -1;
	}
	tree_node(String^ name, int image_index, int selected_image_index)
		:super(name, image_index, selected_image_index)
	{
		Name = name;
		m_image_index_collapsed		= -1;
		m_image_index_expanded		= -1;
	}
private:
	bool					m_first_expand_processed;
	bool					get_selected_impl			( );
	void					set_selected_impl			( bool b );
public:
	property bool			Selected{
		bool		get()		{return get_selected_impl();}
		void		set(bool b)	{return set_selected_impl(b);}
	
	}

	property Drawing::Color fore_color_
	{
		Drawing::Color get(){return m_fore_color;}
	}

	Drawing::Color			m_fore_color;
	Drawing::Color			m_back_color;

	tree_node_type			m_node_type;
	int						m_image_index_collapsed;
	int						m_image_index_expanded;

public:
	property tree_node^	Parent
	{
		virtual tree_node^		get() new {return safe_cast<tree_node^>(TreeNode::Parent);}
	}
	property TreeNodeCollection^ nodes
	{
		TreeNodeCollection^		get(){return TreeNode::Nodes;}
	}
	property bool	first_expand_processed{
		void					set(bool value){m_first_expand_processed = value;}
		bool					get(){return m_first_expand_processed;}
	}
public:
	void			Nodes					( ){}
	tree_node^		add_node_single			( String^ name);
	tree_node^		add_node_single			( String^ name, int image_index);
	tree_node^		add_node_file_part		( String^ name);
	tree_node^		add_node_group			( String^ name);
	tree_node^		add_node_group			( String^ name, int image_index_expanded, int image_index_collapsed );

}; // class tree_node

}//namespace controls
}//namespace editor
}//namespace xray
#endif // #ifndef EX_TREE_NODE_H_INCLUDED