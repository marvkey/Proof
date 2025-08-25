
@echo off
pushd %~dp0
call %PROOF_DIR%\vendor\premake\premake5.exe vs2022

REM Check the error level
if %errorlevel% neq 0 (
    echo An error occurred.
    pause
)

popd

		