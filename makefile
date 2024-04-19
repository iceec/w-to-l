tar:
	g++ -std=c++17 -g main.cpp data_stream.cpp -o a.exe

t:
	g++ -std=c++17 -g m.cpp ./json/json.cpp ./json/parse.cpp  -I ./json -o a.exe