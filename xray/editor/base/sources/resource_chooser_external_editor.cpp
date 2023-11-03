////////////////////////////////////////////////////////////////////////////
//	Created		: 02.12.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "resource_chooser_external_editor.h"
#include "resource_chooser.h"

namespace xray{
namespace editor_base{

void resource_chooser_external_editor::run_editor ( property^ prop )
{
	System::String^ current = nullptr;
	String^ result = nullptr;

	if (prop->value)
		current = safe_cast<System::String^>(prop->value);

	if (resource_chooser::choose(additional_data, current, "", result, true)) 
	{
		prop->value = result;
	}
}

} // namespace editor_base
} // namespace xray