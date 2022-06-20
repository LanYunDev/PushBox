#include"function.h"

Maps map;

Maps *gameMap(int level);

Interfs Interf;

Interfs *interf(int scene);

undo withdraw[1000];
IMAGE img_all[7];

int main() {

    loadImg();
    init();

    initmap();
    interf(1);


    while (1) {

        update();

        render();

    }
    return 0;
}