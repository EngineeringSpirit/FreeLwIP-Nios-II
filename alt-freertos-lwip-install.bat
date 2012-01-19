@ echo off
@ rem save our work directory
@ for /f %%i in ('cd') do set past=%%i

@ rem apperently not.. check if we have SOPC_KIT_NIOS2
@ cd %SOPC_KIT_NIOS2% 2> tmp.txt
@ if %errorlevel% equ 0 (
	@ cd ..
	@ for /f %%i in ('cd') do set nios=%%i
	@ cd "%past%"
	
	@ goto exec_nios_shell
)

@ rem check if QUARTUS_ROOTDIR is set
@ cd %QUARTUS_ROOTDIR% 2> tmp.txt
@ if %errorlevel% equ 0 (
	@ cd ..
	@ for /f %%i in ('cd') do set nios=%%i
	@ cd "%past%"
	
	@ goto exec_nios_shell
)

@ rem apperently not.. ask the user for the path...
@ set /p nios="Where is Altera installed? "

:exec_nios_shell
@ rem get cygwin paths from install directory and quartus toolchain directory
@ for /f %%i in ('%nios%\quartus\bin\cygwin\bin\pwd.exe') do set cyginst=%%i

@ cd "%nios%" 2> tmp.txt
@ for /f %%i in ('%nios%\quartus\bin\cygwin\bin\pwd.exe') do set cygnios=%%i
@ cd "%past%" 2> tmp.txt

@ del tmp.txt

@ echo Launching the Nios II Command Shell, please standby....

@ rem execute the install script for great succes!
"%nios%\nios2eds\Nios II Command Shell.bat" "%cyginst%/alt-freertos-lwip-do-install.sh" "%cygnios%" "%cyginst%"