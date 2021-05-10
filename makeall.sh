mkdir -p  bin/linux/Debug
mkdir -p  bin/linux/Relase

mkdir -p obj
mkdir -p obj/assets
mkdir -p obj/GLAD
mkdir -p obj/ImGUI
mkdir -p obj/utils
mkdir -p obj/imguizmo

mkdir -p obj/ecs/other
mkdir -p obj/yaml-cpp/contrib
mkdir -p obj/core/other

./makepch.sh
./makeglad.sh
./make.sh
