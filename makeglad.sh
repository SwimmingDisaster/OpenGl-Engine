echo 'obj/vendor/GLAD/glad.o'

if [ "$1" == "Debug" ]; 
then 
	cd obj/vendor/GLAD/ && clang -c ../../../src/vendor/GLAD/glad.c -I ../../../include/ -I ../../../include/vendor
else 
	cd obj/vendor/GLAD/ && clang -O3 -c ../../../src/vendor/GLAD/glad.c -I ../../../include/ -I ../../../include/vendor
fi


