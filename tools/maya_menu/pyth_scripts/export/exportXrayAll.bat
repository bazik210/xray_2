rem ENTER PATH TO MAYA AND PROJECT ROOT

call exportXrayMeshSkelAnim.bat
@if not "%errorlevel%"=="0" @(
  @echo exportXrayMeshSkelAnim.bat failed with %errorlevel% code
  @goto:eof
)
call exportXraySkinAnimAdd.bat 
@if not "%errorlevel%"=="0" @(
  @echo exportXraySkinAnimAdd.bat failed with %errorlevel% code
  @goto:eof
)
