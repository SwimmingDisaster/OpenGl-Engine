echo 'obj/vendor/GLAD/glad.o'
cd obj/vendor/GLAD/ && clang -c ../../../src/vendor/GLAD/glad.c -I ../../../include/ -I ../../../include/vendor
