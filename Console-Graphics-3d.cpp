#include <math.h>
#include <windows.h>
#include <stdio.h>
#define PI 3.14159265358979323846

struct Dot_2D
{
    double x;
    double y;
};
struct Dot_3D
{
    double x;
    double y;
    double z;
};

const int sw = 100;
const int sh = 100;
char screen[sh][sw];
const int VERTEX_NUMBER = 8;
Dot_2D FIGURE_2D[VERTEX_NUMBER];
Dot_3D FIGURE_3D[VERTEX_NUMBER] = {
    { 25, 25, 25  },
    { 75, 25, 25  },
    { 75, 75, 25  },
    { 25, 75, 25  },
    { 25, 75, -25 },
    { 75, 75, -25 },
    { 75, 25, -25 },
    { 25, 25, -25 }
};
Dot_3D CAMERA = { 50, 50, 50 };

void DrawScreen();
void Rotate(int, double, double, double);
void ScreenSetDot(double, double, char);
void GenerateEdge(Dot_2D, Dot_2D);
void ClearScreen();
Dot_2D Convert_3d_to_2d(Dot_3D);

int main()
{
    system("mode con cols=100 lines=101");
    system("title Figures 3d");
    ClearScreen();

    // Project 3d dots on 2d screen
    for (int i = 0; i < VERTEX_NUMBER; i++)
    {
        FIGURE_2D[i] = Convert_3d_to_2d(FIGURE_3D[i]);
    }
 
    // Connect vertexes
    GenerateEdge(FIGURE_2D[0], FIGURE_2D[1]);
    GenerateEdge(FIGURE_2D[1], FIGURE_2D[2]);
    GenerateEdge(FIGURE_2D[2], FIGURE_2D[3]);
    GenerateEdge(FIGURE_2D[3], FIGURE_2D[4]);
    GenerateEdge(FIGURE_2D[4], FIGURE_2D[5]);
    GenerateEdge(FIGURE_2D[5], FIGURE_2D[6]);
    GenerateEdge(FIGURE_2D[6], FIGURE_2D[7]);
    GenerateEdge(FIGURE_2D[7], FIGURE_2D[0]);
    GenerateEdge(FIGURE_2D[3], FIGURE_2D[0]);
    GenerateEdge(FIGURE_2D[4], FIGURE_2D[7]);
    GenerateEdge(FIGURE_2D[6], FIGURE_2D[1]);
    GenerateEdge(FIGURE_2D[5], FIGURE_2D[2]);

    // Set vertexes
    for (int i = 0; i < VERTEX_NUMBER; i++)
    {
        ScreenSetDot(FIGURE_2D[i].x, FIGURE_2D[i].y, 'O');
    }

    // Draw 
    DrawScreen();

    getchar();
    return 0;
}

Dot_2D Convert_3d_to_2d(Dot_3D dot3d)
{
    Dot_2D dot2d;

    dot2d.x = dot3d.x;
    dot2d.y = dot3d.y;
    if (dot3d.z > 0)
    {
        double k = 1 + dot3d.z / CAMERA.z;
        dot2d.x = CAMERA.x + (dot3d.x - CAMERA.x) * k;
        dot2d.y = CAMERA.y + (dot3d.y - CAMERA.y) * k;
    }
    else if (dot3d.z < 0)
    {
        double k = 1 + -dot3d.z / CAMERA.z;
        dot2d.x = CAMERA.x + (dot3d.x - CAMERA.x) / k;
        dot2d.y = CAMERA.y + (dot3d.y - CAMERA.y) / k;
    }

    return dot2d;
}

void ClearScreen()
{
    for (int i = 0; i < sw; i++)
    {
        for (int j = 0; j < sh; j++)
        {
            screen[i][j] = ' ';
        }
    }
}

void DrawScreen()
{
    system("cls");
    for (int i = 0; i < sh; i++)
    {
        for (int j = 0; j < sw; j++)
        {
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
}

void ScreenSetDot(double dot_x, double dot_y, char dot)
{
    int sx = floor(dot_x);
    int sy = floor(dot_y);

    if (sx >= 0 && sy >= 0 && sx < sw && sy < sh)
    {
        screen[sy][sx] = dot;
    }
}

void GenerateEdge(Dot_2D vert1, Dot_2D vert2)
{
    double x, y; 

    // Render though X
    if (vert1.x > vert2.x)
    {
        Dot_2D temp = vert1;
        vert1 = vert2;
        vert2 = temp;
    }

    for (x = vert1.x; x < vert2.x; x++)
    {
        y = (x - vert1.x) / (vert2.x - vert1.x) * (vert2.y - vert1.y) + vert1.y;
        ScreenSetDot(x, y, '.');
    }

    // Render though Y
    if (vert1.y > vert2.y)
    {
        Dot_2D temp = vert1;
        vert1 = vert2;
        vert2 = temp;
    }

    for (y = vert1.y; y < vert2.y; y++)
    {
        x = (y - vert1.y) / (vert2.y - vert1.y) * (vert2.x - vert1.x) + vert1.x;
        ScreenSetDot(x, y, '.');
    }
}