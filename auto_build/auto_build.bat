@echo off
set start_time=%time%  

set GIT_URL=https://github.com/KWYangESMT/wise_sdk.git
set ESMT_TOOL_PATH=C:\workspace\tools\eclipse-embedcpp-esmt
set ECLIPSE_PATH=%ESMT_TOOL_PATH%\eclipse\
set TOOL_CHAIN_PATH=%ESMT_TOOL_PATH%\arm-gnu-toolchain-12.3\bin
set BUILD_TOOL_PATH=%ESMT_TOOL_PATH%\build-tools-2.12\2.12-20190422-1053\bin\

PATH=%PATH%;%TOOL_CHAIN_PATH%;%BUILD_TOOL_PATH%;

set WORKING_FOLDER=%cd%\
set BUILD_ROOT=%WORKING_FOLDER%\
set SOURCE_FOLDER=%BUILD_ROOT%\source\
set GIT_ROOT=%SOURCE_FOLDER%\wise_sdk\
set WS_PATH=%BUILD_ROOT%\workspace\

set PROJECT1_NAME=AppLoader
set PROJECT1_ROOT=%GIT_ROOT%\app\%PROJECT1_NAME%\

set PROJECT2_NAME=WISERadioLib
set PROJECT2_ROOT=%GIT_ROOT%\app\%PROJECT2_NAME%\

set PROJECT3_NAME=project_template
set PROJECT3_ROOT=%GIT_ROOT%\app\%PROJECT3_NAME%\

set PROJECT4_NAME=WISEDemoApp
set PROJECT4_ROOT=%GIT_ROOT%\app\%PROJECT4_NAME%\

set REMOVE_FOLDERS=%PROJECT1_ROOT%\eclipse\.settings ^
                    %PROJECT1_ROOT%\eclipse\ER8130_TC04 ^
                    %PROJECT2_ROOT%\eclipse\.settings ^
                    %PROJECT2_ROOT%\eclipse\ER8130_TC04 ^
                    %PROJECT3_ROOT%\eclipse\.settings ^
                    %PROJECT3_ROOT%\eclipse\ER8130_TC04 ^
                    %PROJECT4_ROOT%\eclipse\.settings ^
                    %PROJECT4_ROOT%\eclipse\ER8130_TC04 ^
                    %WS_PATH%

if exist %GIT_ROOT%\ (
  echo Cleaning...
  for %%a in (%REMOVE_FOLDERS%) do (
      if exist %%a (
          echo removing %%a
          rmdir %%a /S /Q
      )
  )
) else (
  echo Cloning source...
  mkdir %SOURCE_FOLDER%
  cd %SOURCE_FOLDER%
  git clone %GIT_URL%
)

mkdir %WS_PATH%

echo Updating...
cd %GIT_ROOT%
git reset --hard HEAD
git pull
git checkout main
cd %BUILD_ROOT%

echo Start %PROJECT1_NAME%...
%ECLIPSE_PATH%\eclipsec.exe -nosplash ^
                                -application org.eclipse.cdt.managedbuilder.core.headlessbuild ^
                                -import %PROJECT1_ROOT%\eclipse\ ^
                                -data %WS_PATH% ^
                                -build %PROJECT1_NAME%

exit /b

echo Start %PROJECT2_NAME%...
%ECLIPSE_PATH%\eclipsec.exe -nosplash ^
                                -application org.eclipse.cdt.managedbuilder.core.headlessbuild ^
                                -import %PROJECT2_ROOT%\eclipse\ ^
                                -data %WS_PATH% ^
                                -build %PROJECT2_NAME%

echo Start %PROJECT3_NAME%...
%ECLIPSE_PATH%\eclipsec.exe -nosplash ^
                                -application org.eclipse.cdt.managedbuilder.core.headlessbuild ^
                                -import %PROJECT3_ROOT%\eclipse\ ^
                                -data %WS_PATH% ^
                                -build %PROJECT3_NAME%

echo Start %PROJECT4_NAME%...
%ECLIPSE_PATH%\eclipsec.exe -nosplash ^
                                -application org.eclipse.cdt.managedbuilder.core.headlessbuild ^
                                -import %PROJECT4_ROOT%\eclipse\ ^
                                -data %WS_PATH% ^
                                -build %PROJECT4_NAME%
                                                                
set end_time=%time% 

call :CalculateElapsedTime %start_time% %end_time%  
exit /b

:CalculateElapsedTime  
set start=%1  
set end=%2  

for /f "tokens=1-4 delims=:." %%a in ("%start%") do (  
    set /a start_seconds=%%a * 3600 + %%b * 60 + %%c  
)  
for /f "tokens=1-4 delims=:." %%a in ("%end%") do (  
    set /a end_seconds=%%a * 3600 + %%b * 60 + %%c  
)  

set /a elapsed_seconds=end_seconds-start_seconds  

if %elapsed_seconds% lss 0 (  
    set /a elapsed_seconds+=86400  
)  

echo Elapsed Time: %elapsed_seconds% seconds  
exit /b 0
          
@echo on

