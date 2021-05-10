mkdir -p  bin/linux/Debug
mkdir -p  bin/linux/Relase

mkdir obj
mkdir obj/assets
mkdir obj/core
mkdir obj/GLAD
mkdir obj/ImGUI
mkdir obj/utils
mkdir obj/imguizmo

mkdir -p obj/ecs/other
mkdir -p yaml-cpp/contrib

./makepch.sh
./makeglad.sh
./make.sh
