
set build_server=http://aurora.gsc-game.kiev.ua
set temp_post_data=temp_post_data.txt
set repository_path=sample_path
set revision=2

rem json={"parameter"%3A+[{"name"%3A+"repository_path"%2C+"value"%3A+"sample_path"}%2C+{"name"%3A+"revision"%2C+"value"%3A+"2"}]%2C+""%3A+[""%2C+""]} 

echo json={"parameter"^%%3A+[{"name"^%%3A+"repository_path"^%%2C+"value"^%%3A+"%repository_path%"}^%%2C+{"name"^%%3A+"revision"^%%2C+"value"^%%3A+"%revision%"}]^%%2C+""^%%3A+[""^%%2C+""]} > %temp_post_data%

curl.exe -c temp_cookies -G -d j_username=buildstation -d j_password=buildpass %build_server%/j_acegi_security_check
curl.exe -b temp_cookies -d @%temp_post_data% %build_server%/job/Stalker_2-PostCommitHook/build

