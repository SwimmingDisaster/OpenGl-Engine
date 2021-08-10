echo 'obj/mypch.o'
if [ "$1" == "Debug" ]; 
then 
	#clang -x c++-header -fsanitize=address -fno-omit-frame-pointer -pthread --std=c++20 -Wno-deprecated-volatile -I include/ -I include/code -I include/vendor include/mypch.h -o include/mypch.h.pch
	clang -x c++-header -fno-omit-frame-pointer -pthread --std=c++20 -Wno-deprecated-volatile -I include/ -I include/code -I include/vendor include/mypch.h -o include/mypch.h.pch
else 
	clang -x c++-header -fno-omit-frame-pointer -pthread --std=c++20 -O3 -Wno-deprecated-volatile -I include/ -I include/code -I include/vendor include/mypch.h -o include/mypch.h.pch
fi
