
@echo off
setlocal enabledelayedexpansion
set LIC_PREFIX=xc32fpp

echo Detecting Operating System
ver | findstr /i "5\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_XP
ver | findstr /i "5\.2\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_XP
ver | findstr /i "6\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Vista
ver | findstr /i "6\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.2\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.3\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.4\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "10\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
goto warn_and_exit

:ver_Win7
echo OS Version: Windows 7/8/8.1/10
goto set_Vista_Win7

:ver_Vista
echo OS Version: Windows Vista
goto set_Vista_Win7

:ver_XP
echo OS Version: Windows XP
goto set_XP

:warn_and_exit
echo Operating system:
ver
echo ... is not supported.
goto end

:set_XP
set DIR="%ALLUSERSPROFILE%\Application Data\Microchip\xclm\license\"
goto makelicense

:set_Vista_Win7
set DIR="%PROGRAMDATA%\Microchip\xclm\license\"
goto makelicense

:makelicense
if not exist %DIR% goto nodirfound
    for /L %%G in (1,1,1000) do (
set FILE=%DIR%%LIC_PREFIX%-%%G.lic
if not exist "!FILE!" goto endloop
)

:endloop
echo Creating license file %FILE%
echo This XC32 C++ Free License was created by Microchip Web Activation >> %FILE%
echo Please do not edit the contents of this file >> %FILE%
echo Created on 3/24/2016 09:02:48 AM >> %FILE%
echo LICENSE microchip swxc32-fpp 1.1 permanent uncounted >> %FILE%
echo   hostid=6067209bcb0c _ck=b5fefcccf1 sig=^"60PG45156FKT1BC623VDUSC3T260 >> %FILE%
echo   GEPNER3AK1822M084SP50RYBS54N3DP0P1E0UUXKK0VABS3G^" >> %FILE%


goto end

:nodirfound
echo The folder %DIR% does not exist.
echo Please install an XC compiler, then rerun the script.
goto end

:end
pause
