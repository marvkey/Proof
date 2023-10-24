@echo off
pushd %~dp0
call %PROOF_DIR%\vendor\premake\premake5.exe vs2022
popd
PAUSE