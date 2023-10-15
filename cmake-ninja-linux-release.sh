clear
echo --------------------------------------------------------------------------------
COLUMNS=$(tput cols)
title="CRUCIBLE ENGINE"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
COLUMNS=$(tput cols)
title="Ninja GENERATION FOR x86 - x64"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
COLUMNS=$(tput cols)
title="RELEASE BUILD"
printf "%*s\n" $(((${#title}+$COLUMNS)/2)) "$title"
echo --------------------------------------------------------------------------------
#cd ..
rm -rf build > /dev/null 2>&1
mkdir build > /dev/null 2>&1
cd build
echo Generating files
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DONATIVE=ON .. > log.txt ||(
echo ERROR Generating files
cd ..
rmdir /Q /S build
#read -rsp $'Press any key to continue...\n' -n1 key
)
#read -rsp $'Press any key to continue...\n' -n1 key
echo Files generated Successfully
