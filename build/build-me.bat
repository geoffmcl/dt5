@setlocal
@set TMPBGN=%TIME%

@set TMPPRJ=dt5
@echo Setup for 32-bit %TMPROJ% build
@set TMPLOG=bldlog-1.txt
@set TMPSRC=..
@REM ###########################################
@REM NOTE: VERY Specific install location
@REM ###########################################
@set TMPINST=C:\MDOS
@REM ###########################################

@REM set TMP3RD=F:\Projects\software
@REM set BOOST_ROOT=X:\install\msvc100\boost
@REM if NOT EXIST %BOOST_ROOT%\nul goto NOBOOST

@call chkmsvc %TMPPRJ%
@REM call setupqt32
@REM if EXIST build-cmake.bat (
@REM call build-cmake
@REM if ERRORLEVEL 1 goto NOBCM
@REM )


@REM Nothing below need be touched..
@REM if NOT EXIST F:\nul goto NOXD
@REM if NOT EXIST %TMPSRC%\nul goto NOSRC
@REM if NOT EXIST %TMP3RD%\nul goto NO3RD
@REM if NOT EXIST %BOOST_ROOT%\nul goto NOBOOST
@if NOT EXIST %MPINST%\nul goto NOINST
@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOSRC2

@set TMPOPTS=-DCMAKE_INSTALL_PREFIX=%TMPINST%
@REM set TMPOPTS=%TMPOPTS% -DCMAKE_PREFIX_PATH:PATH=%TMP3RD%

:RPT
@if "%~1x" == "x" goto GOTCMD
@set TMPOPTS=%TMPOPTS% %1
@shift
@goto RPT
:GOTCMD

@echo Building %TMPPRJ% begin %TMPBGN% > %TMPLOG%
@echo All output to %TMPLOG%...

@REM echo Set ENV BOOST_ROOT=%BOOST_ROOT% >> %TMPLOG%

@echo Doing 'cmake %TMPSRC% %TMPOPTS%' out to %TMPLOG%
@echo Doing 'cmake %TMPSRC% %TMPOPTS%' >> %TMPLOG%
@cmake %TMPSRC% %TMPOPTS% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing 'cmake --build . --config Debug'
@echo Doing 'cmake --build . --config Debug'  >> %TMPLOG%
@cmake --build . --config Debug  >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@echo Doing: 'cmake --build . --config Release'
@echo Doing: 'cmake --build . --config Release'  >> %TMPLOG%
@cmake --build . --config Release  >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3

@REM type %TMPLOG%
@fa4 "***" %TMPLOG%
@echo.
@echo Appears a successful build... see %TMPLOG%
@call elapsed %TMPBGN%
@REM echo.
@REM echo No install at this time
@REM goto END

@echo Proceed with an install - Debug then Release
@echo.
@echo *** CONTINUE? *** Only Ctrl+C aborts... all other keys continue...
@pause

@REM echo Doing: 'cmake --build . --config Debug --target INSTALL'
@REM echo Doing: 'cmake --build . --config Debug --target INSTALL' >> %TMPLOG%
@REM cmake --build . --config Debug --target INSTALL >> %TMPLOG% 2>&1
@REM if ERRORLEVEL 1 goto ERR4

@echo Doing: 'cmake --build . --config Release --target INSTALL'
@echo Doing: 'cmake --build . --config Release --target INSTALL' >> %TMPLOG% 2>&1
@cmake --build . --config Release --target INSTALL >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR5
@echo.
@fa4 " -- " %TMPLOG%
@echo.
@call elapsed %TMPBGN%
@echo.
@echo All done... build and install to %TMPINST%
@echo See %TMPLOG% for details...
@echo.
@goto END

:ERR1
@echo ERROR: Cmake config or geneation FAILED!
@goto ISERR

:ERR2
@echo ERROR: Cmake build Debug FAILED!
@goto ISERR

:ERR3
@echo ERROR: Cmake build Release FAILED!
@goto ISERR

:ERR4
@echo ERROR: Cmake install debug FAILED!
@goto ISERR

:ERR5
@echo ERROR: Cmake install release FAILED!
@goto ISERR

:NOXD
@echo Error: X:\ drive NOT found!
@goto ISERR
 
:NOSRC
@echo Error: No %TMPSRC% found!
@goto ISERR

:NO3RD
@echo Error: No directory %TMP3RD% found!
@goto ISERR

:NOINST
@echo Error: No INSTALL directory %TMPINST% found! *** FIX ME ***
@echo This vriable should be set to whereever you want this simple dt5 app installed
@echo Obviously if you want it available everywhere that path should be in your PATH
@goto ISERR

:NOBOOST
@echo Error: Boost directory %BOOST_ROOT% not found!
@goto ISERR
 
:NOSRC2
@echo Error: File %TMPSRC%\CMakeLists.txt not found!
@goto ISERR

:NOBCM
@echo Error: Running build-cmake.bat caused an error!
@goto ISERR

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof

