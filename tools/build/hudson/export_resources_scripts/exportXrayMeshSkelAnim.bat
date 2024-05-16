rem ENTER PATH TO MAYA AND PROJECT ROOT

set MAYA=c:/Program Files/Autodesk/Maya2011
set STK2_ROOT_PATH=d:/hudson/jobs/Stalker_2-Polling/workspace

"%MAYA%/bin/mayapy.exe" "%STK2_ROOT_PATH%/tools/maya_menu/pyth_scripts/export/exportXRayBatch.py" --option %STK2_ROOT_PATH% --mesh --skeleton --anim