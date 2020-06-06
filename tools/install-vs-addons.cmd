set "PATH=C:\Windows;C:\Windows\System32;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\Git\bin"
cd %~dp0
call powershell -File .\install_llvm-win64.ps1

REM Download Visual Studio LLVM extension required for clang build to succeed
call download.cmd https://llvmextensions.gallerycdn.vsassets.io/extensions/llvmextensions/llvm-toolchain/1.0.363769/1560930595399/llvm.vsix

REM Install optional components required for ARM build - vs2017-BuildTools
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools" (
  "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vs_installer.exe" modify --installPath "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools" -q ^
	--add Microsoft.VisualStudio.Component.VC.ATL ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM64 --passive --wait
  "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\VSIXInstaller.exe" /q /a llvm.vsix
)

REM Install optional components required for ARM build - vs2017-Enterprise
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise" (
  "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vs_installer.exe" modify --installPath "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise" -q ^
	--add Microsoft.VisualStudio.Component.VC.ATL ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM64 --passive --wait
  "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\VSIXInstaller.exe" /q /a llvm.vsix
)

REM Install optional components required for ARM build - vs2019-Enterprise
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise" (
  "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vs_installer.exe" modify --installPath "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise" -q ^
	--add Microsoft.VisualStudio.Component.VC.ATL ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM ^
	--add Microsoft.VisualStudio.Component.VC.ATL.ARM64 --passive --wait
  "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\VSIXInstaller.exe" /q /a llvm.vsix
)

REM Ignore failures if components have been already installed
EXIT /b 0
