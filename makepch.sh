clang -x c++-header -fsanitize=address -pthread --std=c++20 -Wno-deprecated-volatile --include-directory=include include/mypch.h -o include/mypch.h.pch
