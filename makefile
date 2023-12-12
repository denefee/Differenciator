CC = g++

OBJ_FOLDER = release/

diff:
	$(CC) main.cpp file_analize.cpp reader.cpp diffirentiator.cpp diff_tex.cpp diff_tree.cpp utilities.cpp -o $(OBJ_FOLDER)prog