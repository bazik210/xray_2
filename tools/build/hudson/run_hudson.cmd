if exist corrected_hudson_custom_workspaces goto :skip_correcting_workspaces

%~dp0correct_hudson_custom_workspace.exe Stalker_2-Publish
%~dp0correct_hudson_custom_workspace.exe Stalker_2-Polling-XraySources

echo delete this file if you want to recorrect custom workspace pathes >> corrected_hudson_custom_workspaces

:skip_correcting_workspaces

set devenv_path=C:\Program Files (x86)\VS2008\Common7\IDE

:run_again

java -Xms128m -Xmx1024m -DHUDSON_HOME="%cd%" -Xss256k -jar hudson.war --httpPort=80 --httpDoHostnameLookups

goto:run_again