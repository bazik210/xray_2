rem ENTER PATH TO MAYA AND PROJECT ROOT

set MAYA=Z:/Program Files (x86)/Autodesk/Maya2011
set STK2_ROOT_PATH=x:

"%MAYA%/bin/mayapy.exe" "%STK2_ROOT_PATH%/tools/maya_menu/pyth_scripts/material/changeMaterialBatch.py" --option %STK2_ROOT_PATH% --from xray_shader --to phong


