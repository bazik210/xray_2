function get_project(project) {
    var folders = project.split("\/");
    var projects = DTE.Solution.Projects;
    for (var i = 1; i <= projects.Count; i++) {
        var temp_project = projects.Item(i);
        if (temp_project.Name == folders[0]) {
            for (var k = 1; k < folders.length; ++k) {
                var temp_items = temp_project.ProjectItems;
                for (var j = 1; j <= temp_items.Count; ++j) {
                    var temp_item = temp_items.Item(j);
                    if (temp_item.Name == folders[k]) {
                        temp_project = temp_item.SubProject;
                        break;
                    }
                }
            }
            return temp_project;
        }
    }
}

function add_dependency(project, project_to_depend_on) {
    DTE.Solution.SolutionBuild.BuildDependencies.Item(
        project.UniqueName
    ).AddProject(
        project_to_depend_on.UniqueName
    );
}

function OnFinish(selProj, selObj)
{
	try
	{
		var project_path = wizard.FindSymbol('PROJECT_PATH');
		var project_name = wizard.FindSymbol('PROJECT_NAME');
		wizard.AddSymbol("PROJECT_NAME_UPPER", project_name.toUpperCase() );
		
		
		var currentTime = new Date();
        var month       = currentTime.getMonth() + 1
        var day         = currentTime.getDate();
        var year        = currentTime.getFullYear();
        
        var day_str     = day;
        if (day < 10)
            day_str     = "0" + day;
        var month_str   = month;
        if (month < 10)
            month_str   = "0" + month;
            
        var today       = day_str + '.' + month_str + '.' + year;
            
        
        wizard.AddSymbol('CURRENT_DATE', today);
        wizard.AddSymbol('CURRENT_YEAR', year);

        var wshell = new ActiveXObject("WScript.Shell")
        var author = wshell.ExpandEnvironmentStrings("%xray_user_id%")

        wizard.AddSymbol('AUTHOR_NAME', author);


        var vcproj_path = project_path + "\\sources";
        selProj = CreateCustomProject(project_name, vcproj_path);
        var build = DTE.Solution.SolutionBuild;
        var configurations = build.SolutionConfigurations;
        for (var j = 1; j <= configurations.Count; ++j) {
            var configuration = configurations.Item(j);
            if (configuration.Name.indexOf("PS3") == -1)
                continue;

            configuration.Delete();
            --j;
        }

        // it is impossible(!) to remove redundant Mixed Platforms solution platform
        // :(
//        ConfigurationManager.DeletePlatform( "Mixed Platforms" );

        var InfFile = CreateCustomInfFile();
		AddFilesToCustomProj(selProj, project_name, project_path, InfFile);
		InfFile.Delete();


		// set as startup project
		//build.StartupProjects = selProj.UniqueName;

		// setup working directory
		for (var i = 1; i <= selProj.Object.Configurations.Count; ++i) {
		    var temp_item = selProj.Object.Configurations.Item(i);
		    temp_item.DebugSettings.WorkingDirectory = "$(TargetDir)";
		}

		// setup solution configuration and platform

//		wizard.OkCancelAlert("test");

//		for (var j = 1; j <= configurations.Count; ++j) {
//		    var configuration = configurations.Item(j);
//		    if ( (configuration.PlatformName != "PS3 GCC") && (configuration.PlatformName != "PS3 SNC") )
//		        continue;
//		        
//	        var contexts = configuration.SolutionContexts;
//	        var project_context = contexts.Item(selProj.UniqueName);

//	        wizard.OkCancelAlert(
//	            ""
//	            + "ProjectName: " + project_context.ProjectName + "\r\n"
//	            + "PlatformName: " + project_context.PlatformName + "\r\n"
//	            + "ConfigurationName: " + project_context.ConfigurationName + "\r\n"
//	            + "Configuration.Name: " + configuration.Name + "\r\n"
//	            + "Configuration.PlatformName: " + configuration.PlatformName + "\r\n"
//	        );
//	        wizard.OkCancelAlert("0!");
//	        var configuration_name_part = configuration.Name;
//	        var index = configuration_name_part.indexOf("(static)");
//	        if (index != -1)
//	            configuration_name_part = configuration_name_part.substr(0,index);
//	        wizard.OkCancelAlert(project_context.ConfigurationName + " => " + configuration.PlatformName + " " + configuration_name_part);
//	        project_context.ShouldBuild = false;
//	        wizard.OkCancelAlert("1! " + project_context.ProjectName );
//	        project_context.ConfigurationName = configuration.PlatformName + " " + configuration_name_part;
////	        wizard.OkCancelAlert(project_context.ConfigurationName + " => " + configuration.PlatformName + " " + configuration_name_part);
//	        wizard.OkCancelAlert("2!");
//	        
//	        project_context.ShouldBuild = true;
//	        wizard.OkCancelAlert("3!");
////	        project_context.PlatformName = "x64";
//	    }
////
////		wizard.OkCancelAlert("test2");
//		
		for (var j = 1; j <= configurations.Count; ++j) {
		    var temp_item = configurations.Item(j);
		    if (temp_item.Name == "Debug(static)" && temp_item.PlatformName == "Win32") {
		        temp_item.Activate();
		        break;
		    }
		}

		// add dependencies
		// we must do this ONLY after proper platform was setup
        add_dependency(selProj, get_project("engine/core/debug"));
        add_dependency(selProj, get_project("engine/core/logging"));
        add_dependency(selProj, get_project("engine/core/fs"));
        add_dependency(selProj, get_project("engine/core/vfs"));
        add_dependency(selProj, get_project("engine/core/core"));
		add_dependency(get_project("engine/engine"), selProj);

		selProj.Object.Save();

		wizard.OkCancelAlert(
            "X-Ray Library Wizard has been completed successfully!\r\n\r\n" +
            "Do not forget to remove \"Mixed Platforms\" platform from Configuration Manager!"
        );
    }
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}

function CreateCustomProject(project_name, project_path)
{
	try
	{
		//var strProjTemplatePath = wizard.FindSymbol('PROJECT_TEMPLATE_PATH');
		var strProjTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
		var strProjTemplate = '';
		strProjTemplate = strProjTemplatePath + '\\default.vcproj';

		var Solution = dte.Solution;
		var strSolutionName = "";
		if (wizard.FindSymbol("CLOSE_SOLUTION"))
		{
			Solution.Close();
			strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
			if (strSolutionName.length)
			{
				var strSolutionPath = project_path.substr(0, project_path.length - project_name.length);
				Solution.Create(strSolutionPath, strSolutionName);
			}
		}

		var strProjectNameWithExt = '';
		strProjectNameWithExt = project_name + '.vcproj';

		var oTarget = wizard.FindSymbol("TARGET");
		var prj;
		if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)  // vsWizardAddSubProject
		{
			var prjItem = oTarget.AddFromTemplate(strProjTemplate, strProjectNameWithExt);
			prj = prjItem.SubProject;
		}
		else
		{
			prj = oTarget.AddFromTemplate(strProjTemplate, project_path, strProjectNameWithExt);
		}
		return prj;
	}
	catch(e)
	{
		throw e;
	}
}

function CreateCustomInfFile()
{
	try
	{
		var fso, TemplatesFolder, TemplateFiles, template_file;
		fso = new ActiveXObject('Scripting.FileSystemObject');

		var TemporaryFolder = 2;
		var tfolder = fso.GetSpecialFolder(TemporaryFolder);
		var strTempFolder = tfolder.Drive + '\\' + tfolder.Name;

		var strWizTempFile = strTempFolder + "\\" + fso.GetTempName();

		var project_path = wizard.FindSymbol('TEMPLATES_PATH');
		var strInfFile = project_path + '\\Templates.inf';
		wizard.RenderTemplate(strInfFile, strWizTempFile);

		var WizTempFile = fso.GetFile(strWizTempFile);
		return WizTempFile;
	}
	catch(e)
	{
		throw e;
	}
}

function FindFileInProject(proj, fileName) {
    try {
        var files = proj.Object.Files;
        for (i = 1; i <= files.Count; i++) {
            var file = files.Item(i);
            if (file == null)
                continue;
            if (file.Name == fileName) {
                return file;
            }
        }
        return null;
    } catch (e) {
        throw e;
    }
}

function AddFilesToCustomProj(proj, project_name, project_path, InfFile)
{
	try {
		var templates_path = wizard.FindSymbol('TEMPLATES_PATH');
		var stream = InfFile.OpenAsTextStream(1, -2);
		while (!stream.AtEndOfStream) {
		    var template_file = stream.ReadLine();
		    if ( !template_file.length ) {
		        continue;
		    }

		    var filter = '';
		    var target_file = template_file;
		    if (target_file.indexOf('!') != -1) {
		        var temp = new Array();
		        temp = target_file.split('!');
		        template_file = temp[0] + temp[1];
		        target_file = temp[0] + project_name;

		        if ( temp[1].charAt(0) != '.' )
		            target_file = target_file + "_";

		        target_file = target_file + temp[1];
		        
		        filter = stream.ReadLine();
		    }
		    else
		        filter = '';

		    if ((filter == '') && target_file.indexOf('+') != -1) {
		        var temp2 = new Array();
		        temp2 = target_file.split('+');
		        template_file = temp2[0] + temp2[1];
		        target_file = temp2[0] + temp2[1];
		        filter = stream.ReadLine();
		    }

		    var template_path = templates_path + '\\' + template_file;
		    var target_path = project_path + '\\' + target_file;

		    if (target_file.indexOf('-') != -1) {
		        target_path = project_path + '\\' + target_file.substring(1);
		    }

	        var copy_only = false;
	        var overwrite_previous_templates = true;
	        wizard.RenderTemplate(template_path, target_path, copy_only, overwrite_previous_templates);

//	        wizard.OkCancelAlert(
//              ""
//	            + "template_path: " + template_path + "\r\n"
//	            + "target_path: " + target_path + "\r\n"
//	            + "filter: " + filter + "\r\n"
//	        );
	        if (filter != '') {
	            if (filter == "!") {
	                proj.Object.AddFile(target_path);
	                continue;
	            }
	            var filters = proj.Object.Filters;
	            var found = false;
	            for (idx = 1; idx <= filters.Count; ++idx) {
	                var current_filter = filters.Item(idx)
	                if (current_filter.Name == filter) {
	                    current_filter.AddFile(target_path);
	                    found = true;
	                    break;
	                }
	            }

	            if (!found) {
	                wizard.OkCancelAlert(
	                    "filter not found!\r\n"
	                    + "template_path: " + template_path + "\r\n"
	                    + "target_path: " + target_path + "\r\n"
	                    + "filter: " + filter + "\r\n"
	                );
	            }
	        }
		}

		stream.Close();

		var pch_file = FindFileInProject(proj, "pch.cpp");
		if (!pch_file)
		    return;
		    
		var pch_file_confs = pch_file.FileConfigurations;
		for (j = 1; j <= pch_file_confs.Count; j++) {
		    var tmp_conf = pch_file_confs.Item(j);
		    var tmp_ctool = tmp_conf.Tool;
		    tmp_ctool.UsePrecompiledHeader = pchOption.pchCreateUsingSpecific;
		    if ( tmp_conf.Name == "PS3 GCC Debug|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g -Wall -Wextra -mwarn-lhs -mwarn-microcode -fno-exceptions"
		    	tmp_ctool.PrecompiledHeaderFile	= "$(InputName).h.gch/$(ProjectName)_$(ConfigurationName).h.gch"
		    } else if ( tmp_conf.Name == "PS3 GCC Release|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g -Wall -Wextra -mwarn-lhs -mwarn-microcode -fno-exceptions -O2"
		    	tmp_ctool.PrecompiledHeaderFile	= "$(InputName).h.gch/$(ProjectName)_$(ConfigurationName).h.gch"
		    } else if ( tmp_conf.Name == "PS3 GCC Master Gold|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g -Wall -Wextra -mwarn-lhs -mwarn-microcode -fno-exceptions -O2"
		    	tmp_ctool.PrecompiledHeaderFile	= "$(InputName).h.gch/$(ProjectName)_$(ConfigurationName).h.gch"
		    } else if ( tmp_conf.Name == "PS3 SNC Debug|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g --create_pch=\"$(IntDir)/$(TargetName).pch\""
		    	tmp_ctool.PrecompiledHeaderFile	= "$(IntDir)/$(TargetName).pch"
		    } else if ( tmp_conf.Name == "PS3 SNC Release|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g -O2 --create_pch=\"$(IntDir)/$(TargetName).pch\""
		    	tmp_ctool.PrecompiledHeaderFile	= "$(IntDir)/$(TargetName).pch"
		    } else if ( tmp_conf.Name == "PS3 SNC Master Gold|Win32" ) {
		    	tmp_ctool.AdditionalOptions 	= "-g -O2 --create_pch=\"$(IntDir)/$(TargetName).pch\""
		    	tmp_ctool.PrecompiledHeaderFile	= "$(IntDir)/$(TargetName).pch"
		    }
		}
    }
	catch(e)
	{
		throw e;
	}
}