@echo off
REM Build LMX Runtime Supporting Software Binary Files

REM =================================================================================
REM How to use this file:
REM ---------------------
REM
REM Run this batch file within a Visual Studio Command Prompt window to
REM build binary versions of LMX runtime libraries.
REM
REM N.B. On Windows 7 and later, if running the batch file in C:\Program Files etc,
REM be sure to run the Visual Studio Command Prompt as Administrator.  (Right-click
REM on the Command Prompt short-cut in the Start Menu and click "Run as 
REM Administrator")
REM
REM You can customise this file by setting the VSVER, H_PATH and OUT_PATH variables
REM below.
REM
REM You can also customise the behavior using command line parameters as follows:
REM
REM To set the Visual Studio version so, for example:
REM     build-libs.bat vs2017x64
REM
REM To set the path for included .h files on the command line do:
REM     build-libs.bat /h_path <include_path>
REM
REM To set the path for output on the command line do:
REM     build-libs.bat /out_path <out_path>
REM
REM These options can be combined, for example:
REM     build-libs.bat vs2017x64 /h_path ../include /out_path ../bin
REM =================================================================================

REM Set version of Visual Studio you are using to build with. e.g.: set VSVER=2013
set VSVER=vsUnknown

REM Set path variables as required (Paths must including trailing /)
set H_PATH=../include/
set OUT_PATH=../win32/

REM Select which files you wish to be compiled.  These are the core files
set CPP_FILES=lmxparse.cpp lmxtypes.cpp

REM Include these src files if you require regex processing (REM out if not)
set CPP_FILES=%CPP_FILES% lmxregex.cpp lmxunicode.cpp

REM Include these src files if you require SOAP processing (REM out if not)
set CPP_FILES=%CPP_FILES% lmxsoap.cpp

REM Adjust compilation flags if required
set CL_FLAGS=/I %H_PATH% /O2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "LMX_DEBUG_CAPTURE_ERROR=1"
set CL_FLAGS=%CL_FLAGS% /EHsc /W3 /nologo /WX /Z7
set DLL_FLAGS=/D "_USRDLL" /D "LMX_MAKE_DLL" /D "LMX_RT_SOURCE" /link /DLL /incremental:no

REM End of configuration section

REM Parse command-line parameters
:param_parse
if "%1" == "" goto param_parse_end
if "%1" == "/h_path" set H_PATH=%~2/& shift & goto param_parse_next
if "%1" == "/out_path" set OUT_PATH=%~2/& shift & goto param_parse_next
if "%1" NEQ "" set VSVER=%1& goto param_parse_next
:param_parse_next
shift
goto param_parse
:param_parse_end

if %VSVER% == vsUnknown set /p VSVER=Enter Visual Studio version (e.g. vs2017x64): 

if "%OUT_PATH%" NEQ "" if not exist "%OUT_PATH%" mkdir "%OUT_PATH%

REM Build RC
rc lmxver.rc
echo Finished building lmxver.res

REM Build MD
cl %CL_FLAGS% /MD /Fe"%OUT_PATH%lmx-MD-%VSVER%.dll" %CPP_FILES% lmxver.res %DLL_FLAGS%
echo Finished building lmx-MD-%VSVER%.dll

REM Build MDd
cl %CL_FLAGS% /MDd /Fe"%OUT_PATH%lmx-MDd-%VSVER%.dll" %CPP_FILES% lmxver.res %DLL_FLAGS% /DEBUG
echo Finished building lmx-MDd-%VSVER%.dll

REM Build MT
del lmx*.obj
for %%F in (%CPP_FILES%) do cl /c %CL_FLAGS% /MT %%F
lib /OUT:"%OUT_PATH%lmx-MT-%VSVER%.lib" lmx*.obj
echo Finished building lmx-MT-%VSVER%.lib

REM Build MTd
del lmx*.obj
for %%F in (%CPP_FILES%) do cl /c %CL_FLAGS% /MTd %%F
lib /OUT:"%OUT_PATH%lmx-MTd-%VSVER%.lib" lmx*.obj
echo Finished building lmx-MTd-%VSVER%.lib

REM Clean up
del lmx*.obj
del lmxver.res
