////////////////////////////////////////////////////////////////////////////
//	Created		: 01.07.2010
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.ComponentModel;

namespace xray.editor.wpf_controls.property_editors.item
{
	/// <summary>
	/// Interaction logic for color_range_editor.xaml
	/// </summary>
	public partial class color_range_editor
	{

		#region | Initialize |


		public color_range_editor()
		{
			InitializeComponent();

			m_color_ramp.key_added				+= m_color_ramp_key_added;
			m_color_ramp.key_modified			+= m_color_ramp_key_modified;
			m_color_ramp.key_removed			+= m_color_ramp_key_removed;
			m_color_ramp.key_modification_done	+= m_color_ramp_key_modification_done;

			m_variant_combo_box.m_combo_box.SelectionChanged += m_combo_box_selection_changed;

			DataContextChanged		+= color_range_editor_DataContextChanged;
		}


		#endregion

		#region |   Fields   |


		Double min_color_offset;
		Double max_color_offset;

		color_curve m_edited_curve;
		Dictionary<GradientStop, color_curve_key> m_key_assiciations = new Dictionary<GradientStop, color_curve_key>();


		#endregion

		#region |   Methods  |


		private void color_range_editor_DataContextChanged	(object sender, DependencyPropertyChangedEventArgs e)	
		{
			if( DataContext == null )
				return;
			var property = (property)this.DataContext;
			var collection_properties = TypeDescriptor.GetProperties(property.value);

			property.sub_properties = property_extractor.extract( property.values, property, (property_extractor.settings)TemplatedParent.GetValue( property_extractor.SettingsProperty ) );

			//create value editors
			property data_source_property = null;
			property color_property = null;
			property input_type_property = null;

			foreach (property sub_property in property.sub_properties)
			{
				if (sub_property.name == "DataSource")
					data_source_property = sub_property;
				else if(sub_property.name == "ColorRamp")
					color_property = sub_property;
				else if (sub_property.name == "Input")
					input_type_property = sub_property;
			}

			m_variant_combo_box.DataContext = data_source_property;


			m_input_combo_box.DataContext = input_type_property;

			var desc = (DescriptionAttribute)property.descriptors[0].Attributes[ typeof( DescriptionAttribute ) ];
			item_text_panel.ToolTip = desc == null ? null : desc.Description;
		}
		private void m_combo_box_selection_changed			(object sender, SelectionChangedEventArgs e)			
		{
			var property = (property)this.DataContext;
			property color_property = null;

			foreach (property sub_property in property.sub_properties)
			{
				if (sub_property.name == "ColorRamp")
				{
					color_property = sub_property;
					break;
				}
			}

			Object container = property.value;
			var properties = TypeDescriptor.GetProperties(container);
			foreach (PropertyDescriptor prop_descriptor in properties)
			{
				if (prop_descriptor.DisplayName == "ColorRamp")
				{
					color_property.descriptors.Clear();
					color_property.descriptors.Add(prop_descriptor);
					break;
				}
			}

			m_key_assiciations.Clear();
			if(m_color_ramp.gradient != null)
				m_color_ramp.gradient.GradientStops.Clear();

			var color_value = color_property.value;
			if (color_value is color_curve)
			{
				color_curve curve = (color_curve)color_value;
				m_edited_curve = curve;

				min_color_offset = curve.min_time;
				max_color_offset = curve.max_time;

				Double range_width = max_color_offset - min_color_offset;

				foreach (var key in curve.keys)
				{
					var drawing_color	= key.color;
					var color			= Color.FromArgb(drawing_color.A, drawing_color.R, drawing_color.G, drawing_color.B);
					var new_stop		= m_color_ramp.add_key((key.position-min_color_offset)/range_width, color);
					m_key_assiciations.Add(new_stop, key);
				}
			}
			m_color_ramp.invalidate();
		}
		private void expand_collapse_Click					(object sender, RoutedEventArgs e)						
		{
			var border = body;
			var property_item = (property)this.DataContext;

			if (border.Visibility == Visibility.Collapsed)
			{
				property_item.is_expanded = true;
				expand.Visibility = Visibility.Collapsed;
				collapse.Visibility = Visibility.Visible;
				border.Visibility = Visibility.Visible;
			}
			else
			{
				property_item.is_expanded = false;
				expand.Visibility = Visibility.Visible;
				collapse.Visibility = Visibility.Collapsed;
				border.Visibility = Visibility.Collapsed;
			}
		}

		private void m_color_ramp_key_removed				(object sender, color_ramp.key_event_args e)			
		{
			m_edited_curve.keys.Remove(m_key_assiciations[e.stop]);
			m_edited_curve.fire_edit_completed();
		}
		private void m_color_ramp_key_modified				(object sender, color_ramp.key_event_args e)			
		{
			var color		= e.stop.Color;
			var range_width = max_color_offset - min_color_offset;
			var key			= m_key_assiciations[e.stop];
			key.color		= color;
			key.position	= (Single)(e.stop.Offset*range_width-min_color_offset);
		}
		private void m_color_ramp_key_added					(object sender, color_ramp.key_event_args e)			
		{
		  	var color = e.stop.Color;
			var range_width = max_color_offset - min_color_offset;
			var key = new color_curve_key(
				(Single)(e.stop.Offset*range_width-min_color_offset),
				color
			);

			m_edited_curve.keys.Add( key );
			m_key_assiciations.Add(e.stop, key);
			m_edited_curve.fire_edit_completed();
		}
		
		private void m_color_ramp_key_modification_done		(object sender, color_ramp.key_event_args e)			
		{
			m_edited_curve.fire_edit_completed();
		}


		#endregion

	}
}
