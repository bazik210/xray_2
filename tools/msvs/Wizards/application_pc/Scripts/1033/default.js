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

		var InfFile = CreateCustomInfFile();
		AddFilesToCustomProj(selProj, project_name, project_path, InfFile);
		InfFile.Delete();

		var build = DTE.Solution.SolutionBuild;

		// set as startup project
		build.StartupProjects = selProj.UniqueName;

		// setup working directory
		for (var i = 1; i <= selProj.Object.Configurations.Count; ++i) {
		    var temp_item = selProj.Object.Configurations.Item(i);
		    temp_item.DebugSettings.WorkingDirectory = "$(TargetDir)";
		}

		// it is impossible(!) to remove redundant Mixed Platforms solution platform
		// :(

		// setup solution configuration and platform
		var configurations = build.SolutionConfigurations;
		for (var j = 1; j <= configurations.Count; ++j) {
		    var temp_item = configurations.Item(j);
		    if (temp_item.Name == "Debug(static)" && temp_item.PlatformName == "Win32") {
		        temp_item.Activate();
		        break;
		    }
		}

		// add dependencies
		// we must do this ONLY after proper platform was setup
		add_dependency(selProj, get_project("engine/engine_pc"));
		add_dependency(selProj, get_project("stalker2/game"));

		selProj.Object.Save();

		wizard.OkCancelAlert(
            "X-Ray Application PC Wizard has been completed successfully!\r\n\r\n" +
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

function CreateCustomProject(project_name, project_path) {
    try {
        var strProjTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
        var strProjTemplate = '';
        var draw_splash_screen = wizard.FindSymbol('DRAW_SPLASH_SCREEN');
        if (draw_splash_screen)
            strProjTemplate = strProjTemplatePath + '\\default.vcproj';
        else
            strProjTemplate = strProjTemplatePath + '\\default_no_splash.vcproj';

        var Solution = dte.Solution;
        var strSolutionName = "";
        if (wizard.FindSymbol("CLOSE_SOLUTION")) {
            Solution.Close();
            strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
            if (strSolutionName.length) {
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
        else {
            prj = oTarget.AddFromTemplate(strProjTemplate, project_path, strProjectNameWithExt);
        }
        return prj;
    }
    catch (e) {
        throw e;
    }
}

function CreateCustomInfFile()
{
	try
	{
		var fso, TemplatesFolder, TemplateFiles, strTemplate;
		fso = new ActiveXObject('Scripting.FileSystemObject');

		var TemporaryFolder = 2;
		var tfolder = fso.GetSpecialFolder(TemporaryFolder);
		var strTempFolder = tfolder.Drive + '\\' + tfolder.Name;

		var strWizTempFile = strTempFolder + "\\" + fso.GetTempName();

		var strTemplatePath = wizard.FindSymbol('TEMPLATES_PATH');
		var strInfFile = strTemplatePath + '\\Templates.inf';
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

function AddFilesToCustomProj(proj, project_name, project_path, InfFile) {
    try {
        var templates_path = wizard.FindSymbol('TEMPLATES_PATH');

        var draw_splash_screen = wizard.FindSymbol('DRAW_SPLASH_SCREEN');
        if (draw_splash_screen) {
            wizard.AddSymbol(
		        "OS_INCLUDE",
		        ""
		    );
            wizard.AddSymbol(
		        "PRE_INITIALIZE",
		        "	preinitialize					( );"
		    );
            wizard.AddSymbol(
		        "POST_INITIALIZE",
		        "\r\n" +
		        "	postinitialize					( );"
		    );
            wizard.AddSymbol(
		        "INITIALIZE_H",
                "private:\r\n" +
                "			void	preinitialize	( );\r\n" +
                "			void	postinitialize	( );\r\n" +
                "\r\n" +
                "private:\r\n" +
                "	HWND	m_splash_screen;" +
                "\r\n"
		    );
        }
        else {
            wizard.AddSymbol(
		        "OS_INCLUDE",
		        "#include <xray/os_include.h>		// for GetCommandLine\r\n"
		    );
            wizard.AddSymbol(
		        "PRE_INITIALIZE",
		        '\r\n	xray::engine::preinitialize		( m_game_proxy, GetCommandLine( ), "' + wizard.FindSymbol('PROJECT_NAME') + '\", __DATE__ );'
		    );
            wizard.AddSymbol("POST_INITIALIZE", "");
            wizard.AddSymbol("INITIALIZE_H", "");
        }

        var stream = InfFile.OpenAsTextStream(1, -2);
        while (!stream.AtEndOfStream) {
            var template_file = stream.ReadLine();
            if (!template_file.length) {
                continue;
            }

            var filter = '';
            var target_file = template_file;
            if (target_file.indexOf('!') != -1) {
                var temp = new Array();
                temp = target_file.split('!');
                template_file = temp[0] + temp[1];
                target_file = temp[0] + project_name;

                if (temp[1].charAt(0) != '.')
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

            if (!draw_splash_screen && (template_file.indexOf("application_splash.cpp") != -1))
                continue;

            var template_path = templates_path + '\\' + template_file;
            var target_path = project_path + '\\' + target_file;

            if (target_file.indexOf('-') != -1) {
                target_path = project_path + '\\' + target_file.substring(1);
            }

            var copy_only = false;
            var overwrite_previous_templates = true;

            if (template_path.indexOf(".ico") == -1)
                wizard.RenderTemplate(template_path, target_path, copy_only, overwrite_previous_templates);
            else
                wizard.RenderTemplate(template_path, target_path, true);

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
        }
    }
    catch (e) {
        throw e;
    }
}