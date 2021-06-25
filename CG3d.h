#pragma once

#ifndef _CG3D_H_
#define _CG3D_H_

#include <cmath>
#include <stdio.h>
#define PI 3.14159265358979323846

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
typedef struct
{
    float x;
    float y;
} Dot_2d;

typedef struct
{
    float x;
    float y;
    float z;
} Dot_3d;

typedef struct
{
    int begin;
    int end;
} Edge;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
class Figure_3d
{
public:
    int vertex_number;
    int edge_number;
    Dot_2d* vertexes_projection;
    Dot_3d* vertexes;
    Edge* edges;
    char dot_edge;
    char dot_vertex;

    void dot_rotate_3d(Dot_3d&, float, float, float, float, float, float);
    void translate_3d(float, float, float);
    void rotate_3d(float deg_rot_x, float deg_rot_y, float deg_rot_z, float rot_x0, float rot_y0, float rot_z0);

    Figure_3d(const char*);
    ~Figure_3d();
};

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- SCREEN -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
class Console_Screen
{
public:
    int w;
    int h;
    char* table;
    Dot_3d camera;
    char dot_clear;

    Dot_2d convert_dot_3d_to_2d(Dot_3d&);
    void clear();
    void print();
    void set_dot(float, float, char);
    void set_edge(Dot_2d, Dot_2d, char);
    void set_figure(Figure_3d&);

    Console_Screen(int screen_width, int screen_height, float camera_x, float camera_y, float camera_z);
    ~Console_Screen();
};

#endif