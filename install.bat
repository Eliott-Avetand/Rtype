@ECHO OFF
cls
ECHO [95mInstalling R-Type...[0m
ECHO [96mCreating Build directory...[0m
MKDIR "build\"
CD "build\"
ECHO [92mBuild directory succesfully created![0m
ECHO [96mCompiling R-Type...[0m
pip install conan==1.57
conan install .. --build=missing
cmake .. -G "Visual Studio 17 2022"
ECHO [96mBuilding R-Type...[0m
cmake --build . --config Release
ECHO [96mCompilation done![0m
CD ..
ECHO [96mMoving binary to root directory...[0m
MOVE build\bin\r-type_client.exe .
MOVE build\bin\r-type_server.exe .
ECHO [1;92mInstall done![0m