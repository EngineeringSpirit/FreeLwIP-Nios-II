@ echo off
@ rem save our work directory
@ for /f %%i in ('cd') do set past=%%i

@ rem check if SOPC_KIT_NIOS2 is set
@ cd %SOPC_KIT_NIOS2% 2> tmp.txt
@ if %errorlevel% equ 0 (
	 %SOPC_KIT_NIOS2:~0,2%
	 cd ..
	 for /f %%i in ('cd') do set nios=%%i
	 cd "%past%"
	 %past:~0,2%
	
	 goto exec_nios_shell
)


@ rem apperently not.. check if we have QUARTUS_ROOTDIR
@ cd %QUARTUS_ROOTDIR% 2> tmp.txt
@ if %errorlevel% equ 0 (
	 %QUARTUS_ROOTDIR:~0,2%
	 cd ..
	 for /f %%i in ('cd') do set nios=%%i
	 cd "%past%"
	 %past:~0,2%
	
	 goto exec_nios_shell
)

@ rem apperently not.. ask the user for the path...
@ set /p nios="Where is Altera installed? "

:exec_nios_shell
@ rem get cygwin paths from install directory and quartus toolchain directory
@ for /f %%i in ('%nios%\quartus\bin\cygwin\bin\pwd.exe') do set cyginst=%%i

@ cd "%nios%" 2> tmp.txt
@ %nios:~0,2%
@ for /f %%i in ('%nios%\quartus\bin\cygwin\bin\pwd.exe') do set cygnios=%%i
@ cd "%past%" 2> tmp.txt
@ %past:~0,2%

@ del tmp.txt

@ echo Launching the Nios II Command Shell, please standby....

@ rem execute the install script for great succes!
@"%nios%\nios2eds\Nios II Command Shell.bat" "%cyginst%/alt-freertos-lwip-do-install.sh" "%cygnios%" "%cyginst%"