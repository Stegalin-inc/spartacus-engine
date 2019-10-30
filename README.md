# spartacus-engine
<p align="center"><img src="https://i.imgur.com/xCSFUOT.png"></p> 
Game engine for risc like games(similar to europa universalis, age of civilizations)

## How to compile?
I use mingw compiler. Use command " g++ engine.cpp -std=c++17 -lopengl32 -lgdi32 -lglu32" to compile.

## How to run?
Place 3 bmp images into folder with exe file. First two is "heightmap.bmp" and "world_normal.bmp".I take it from europa universalis files. And also add "grass.bmp" for grass textue. You can download it anywhere in the internet.
## What already done?
I made Lexer, which can read from file or string and return tokens. It need for simple interpreter and read json-like objects. In json-like objects stored ui templates. Parser of this object realised in Parser class. It take Lexer as parameter and returns Node object which represent node of json like object. 
Interpreter need for make relative coordinates and sizes of ui elements. Interpreter language is forth like language. You can whith this language say to children ui elements be plased in the middle of parent element. You can see it in gui.txt file.
Also i made simple reflection. You can get some class variable by name in runtime. And i made very simple worldmap class, now its just demo.
