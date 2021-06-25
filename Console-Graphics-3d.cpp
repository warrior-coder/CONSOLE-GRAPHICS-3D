#include "CG3d.h"

int main()
{
    Figure_3d FIGURE("Figures-data/cube.dat");
    Console_Screen SCREEN(100, 100, 50, 50, 50);

    while(true)
    {
        SCREEN.clear();
        FIGURE.rotate_3d(0, 2, 0, 50, 50, 0);
        SCREEN.set_figure(FIGURE);
        SCREEN.print();
    }

    return 0;
}