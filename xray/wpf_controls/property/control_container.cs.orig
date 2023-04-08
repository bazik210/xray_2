////////////////////////////////////////////////////////////////////////////
//	Created		: 19.07.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

using System;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;

namespace xray.editor.wpf_controls
{
	public class control_container
	{
		internal		control_container		( Panel panel, property_descriptor descriptor )		
		{
			m_panel			= panel;
			m_descriptor	= descriptor;
		}

		private readonly	Panel				m_panel;
		private				property_descriptor m_descriptor;

		public				Panel				panel		
		{
			get
			{
				return m_panel;
			}
		}
		public				String				category	
		{
			get
			{
				return m_descriptor.category;
			}
			set
			{
				m_descriptor.category = value;
			}
		}
		public				String				description	
		{
			get
			{
				return m_descriptor.description;
			}
			set
			{
				m_descriptor.description = value;
			}
		}

		private				void				fix_container			( )																		
		{
			if( m_panel is UniformGrid )
			{
				( (UniformGrid)m_panel ).Columns = m_panel.Children.Count;
			}
			else if( m_panel is StackPanel )
			{
				( (StackPanel)m_panel ).Orientation = Orientation.Horizontal; 
			}
		}

		public				Button				add_button				( String caption, Action<Button> click_callback )						
		{
			var button			= new Button { Content = caption };
			button.Click		+= ( o, e ) => click_callback( (Button)o );

			m_panel.Children.Add( button );

			fix_container( );

			return button;
		}
		public				ToggleButton		add_toggle_button		( String caption, Action<ToggleButton> state_changed_callback )			
		{
			var button			= new ToggleButton { Content = caption };
			button.Click		+= ( o, e ) => state_changed_callback( (ToggleButton)o );

			m_panel.Children.Add( button );

			fix_container( );

			return button;
		}
	}
}