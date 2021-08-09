if [ "$1" == "Debug" ]; 
then 
	make -j3 -f Debug
else 
	make -j3 -f Release
fi
