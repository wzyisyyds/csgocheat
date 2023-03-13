cd /d "D:\MyProject\MyDriver2" &msbuild "MyDriver2.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform="x64" /p:SolutionDir="D:\MyProject\MyDriver2" 
exit %errorlevel% 