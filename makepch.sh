echo 'obj/mypch.o'
clang -x c++-header -fno-omit-frame-pointer -pthread --std=c++20 -Wno-deprecated-volatile -I include/ -I include/code -I include/vendor include/mypch.h -o include/mypch.h.pch
