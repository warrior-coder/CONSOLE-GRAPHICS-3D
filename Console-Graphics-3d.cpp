#include "CG3d.h"

int main()
{
    Figure_3d FIGURE("figure0.dat");
    Console_Screen SCREEN(100, 100, 50, 50, 50);

    for (int i = 0; i < 1080; i++)
    {
        SCREEN.clear();
        FIGURE.rotate_3d(0, 5, 0, 50, 50, 0);
        SCREEN.set_figure(FIGURE);
        SCREEN.print();
    }

    return 0;
}