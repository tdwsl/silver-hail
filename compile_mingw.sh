SDL2=~/SDL2-2.0.20
SDL2_mixer=~/SDL2_mixer-2.0.4
mingw=i686-w64-mingw32

$mingw-gcc \
-I$SDL2/$mingw/include -I$SDL2/$mingw/include/SDL2 -L$SDL2/$mingw/lib \
-I$SDL2_mixer/$mingw/include -L$SDL2_mixer/$mingw/lib \
-std=gnu99 -Isource source/*.c \
-w -Wl,-subsystem,windows \
-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lSDL2_mixer \
-o hail.exe

cp $SDL2/$mingw/bin/SDL2.dll .
cp $SDL2_mixer/$mingw/bin/SDL2_mixer.dll .
cp $SDL2_mixer/$mingw/bin/libmpg123-0.dll .