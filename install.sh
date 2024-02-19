#!/bin/bash

RED='\033[0;31m'
BLUE='\033[0;34m'
PURPLE='\033[0;95m'
CYAN='\033[0;96m'
GREEN='\033[0;92m'
YELLOW='\033[0;93m'
WHITE='\033[1;37m'
NC='\033[0m'
BACKWHITE='\033[7m'

clear
echo -e "${PURPLE}Installing R-Type...${NC}"
echo -e "${CYAN}Creating Build directory...${NC}"
mkdir "build/" && echo -e "${GREEN}Build directory succesfully created!${NC}" || echo -e "${YELLOW}Build directory already created!${NC}"

sudo pip install conan==1.57 && sudo conan install . --build=missing -c tools.system.package_manager:mode=install --profile ./.conan/profile

cd build
echo -e "${CYAN}Compiling R-Type...${NC}"
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
echo -e "${CYAN}Building R-Type...${NC}"
cmake --build . --config Release
echo -e "${GREEN}R-Type succesfully built!${NC}"
cd ..

mv ./build/bin/r-type_client ./r-type_client
mv ./build/bin/r-type_server ./r-type_server

echo -e "${BACKWHITE}INSTALLATION DONE!${NC}"
