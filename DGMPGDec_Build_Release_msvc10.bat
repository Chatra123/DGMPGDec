@echo off

echo -----------------------------------------------------
echo                   S T A R T
echo -----------------------------------------------------

set VisualStudioVersion=10.0
set TOOLS_VER=4.0
set MSVC_VER=msvc10

rem Set up for MSBuild
set MSBUILD_PATH=%SystemRoot%\Microsoft.NET\Framework\v4.0.30319
set MSBUILD_EXEC=%MSBUILD_PATH%\MSBuild.exe

rem [memo] verbosity-level: q[uiet] m[inimal] n[ormal] d[etailed] diag[nostic]
set BUILD_OPT=/nologo /maxcpucount:1 /toolsversion:%TOOLS_VER% /verbosity:normal /fl
set FLOG_VERBOSITY=normal

rem List up the target project
set TGT_PROJECT=DGMPGDec
set TGT_SLN=.\msvc\%TGT_PROJECT%.sln
set TGT_PLATFORM=Win32
set TGT_CONFIG=Release

rem Set up NASMPATH (Path termination must be a separator character.)
rem set NASMPATH=<YourNasmPath>\

rem Build
for %%c in (%TGT_CONFIG%  ) do (
for %%p in (%TGT_PLATFORM%) do (
    echo -Build-  %%c^|%%p
    "%MSBUILD_EXEC%" %TGT_SLN% /p:Configuration=%%c;Platform=%%p %BUILD_OPT% /flp:logfile=_build_%TGT_PROJECT%_%%c_%%p_%MSVC_VER%.log;verbosity=%FLOG_VERBOSITY%
))

rem Clean up
set VisualStudioVersion=
set TOOLS_VER=
set MSVC_VER=
set MSBUILD_PATH=
set MSBUILD_EXEC=
set BUILD_OPT=
set FLOG_VERBOSITY=
set TGT_PROJECT=
set TGT_SLN=
set TGT_PLATFORM=
set TGT_CONFIG=
set NASMPATH=

echo -----------------------------------------------------
echo                   F I N I S H
echo -----------------------------------------------------
