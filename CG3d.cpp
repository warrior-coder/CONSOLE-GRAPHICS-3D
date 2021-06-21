#include "CG3d.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
Figure_3d::Figure_3d(const char* FigureDataFileName)
{
    FILE* fp;
    vertexes = nullptr;
    edges = nullptr;
    vertexes_projection = nullptr;

    fopen_s(&fp, FigureDataFileName, "rt");
    if (fp)
    {
        // Read vertexes
        fscanf_s(fp, "%d", &vertex_number);
        vertexes = new Dot_3d[vertex_number];
        vertexes_projection = new Dot_2d[vertex_number];
        for (int i = 0; i < vertex_number; i++) fscanf_s(fp, "%f %f %f", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z);

        // Read edges
        fscanf_s(fp, "%d", &edge_number);
        edges = new Edge[edge_number];
        for (int i = 0; i < edge_number; i++) fscanf_s(fp, "%d %d", &edges[i].begin, &edges[i].end);
    }
    else vertex_number = edge_number = 0;

    dot_edge = (char)249;
    dot_vertex = (char)219;
}

Figure_3d::~Figure_3d()
{
    delete[] vertexes;
    delete[] vertexes_projection;
    delete[] edges;
}

void Figure_3d::dot_rotate_3d(Dot_3d& dot, float deg_rot_x, float deg_rot_y, float deg_rot_z, float rot_x0, float rot_y0, float rot_z0)
{
    float r, deg0;

    // Move rotation axis to center
    dot.x -= rot_x0;
    dot.y -= rot_y0;
    dot.z -= rot_z0;

    if (deg_rot_x)
    {
        // Switch to polar coordinate system
        r = sqrt(dot.y*dot.y + dot.z*dot.z);
        if (r)
        {
            if (dot.z > 0) deg0 = acos(dot.y / r);
            else deg0 = -acos(dot.y / r);
            
            // Get result coordinates in rectangle system
            dot.y = r * cos(deg0 + deg_rot_x/180*PI);
            dot.z = r * sin(deg0 + deg_rot_x/180*PI);
        }
    }
    if (deg_rot_y)
    {
        r = sqrt(dot.x*dot.x + dot.z*dot.z);
        if (r)
        {
            if (dot.z > 0) deg0 = acos(dot.x / r);
            else deg0 = -acos(dot.x / r);
            
            dot.x = r * cos(deg0 + deg_rot_y/180*PI);
            dot.z = r * sin(deg0 + deg_rot_y/180*PI);
        }
    }
    if (deg_rot_z)
    {
        r = sqrt(dot.x*dot.x + dot.y*dot.y);
        if (r)
        {
            if (dot.y > 0) deg0 = acos(dot.x / r);
            else deg0 = -acos(dot.x / r);
            
            dot.x = r * cos(deg0 + deg_rot_z/180*PI);
            dot.y = r * sin(deg0 + deg_rot_z/180*PI);
        }
    }

    // Move rotation axis from center
    dot.x += rot_x0;
    dot.y += rot_y0;
    dot.z += rot_z0;
}

void Figure_3d::translate_3d(float dx, float dy, float dz)
{
    for (int i = 0; i < vertex_number; i++)
    {
        vertexes[i].x += dx;
        vertexes[i].y += dy;
        vertexes[i].z += dz;
    }
}

void Figure_3d::rotate_3d(float deg_rot_x, float deg_rot_y, float deg_rot_z, float rot_x0, float rot_y0, float rot_z0)
{
    for (int i = 0; i < vertex_number; i++) dot_rotate_3d(vertexes[i], deg_rot_x, deg_rot_y, deg_rot_z, rot_x0, rot_y0, rot_z0);
}

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- SCREEN -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void Console_Screen::clear()
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++) table[i*101+j] = dot_clear;
        table[i*101+100] = '\n';
    }
    table[100*101] = '\0';
}
    
void Console_Screen::print()
{
    system("cls");
    printf("%s", table);
}

void Console_Screen::set_dot(float dot_x, float dot_y, char dot)
{
    int sx = floor(dot_x);
    int sy = floor(dot_y);

    if (sx >= 0 && sy >= 0 && sx < w && sy < h) table[sy*101+sx] = dot;
}

void Console_Screen::set_edge(Dot_2d vert1, Dot_2d vert2, char edge_dot)
{
    float x, y;

    // Render though X
    if (vert1.x > vert2.x)
    {
        Dot_2d temp = vert1;
        vert1 = vert2;
        vert2 = temp;
    }
    for (x = vert1.x; x < vert2.x; x++)
    {
        y = (x - vert1.x) / (vert2.x - vert1.x) * (vert2.y - vert1.y) + vert1.y;
        set_dot(x, y, edge_dot);
    }

    // Render though Y
    if (vert1.y > vert2.y)
    {
        Dot_2d temp = vert1;
        vert1 = vert2;
        vert2 = temp;
    }
    for (y = vert1.y; y < vert2.y; y++)
    {
        x = (y - vert1.y) / (vert2.y - vert1.y) * (vert2.x - vert1.x) + vert1.x;
        set_dot(x, y, edge_dot);
    }
}

Dot_2d Console_Screen::convert_dot_3d_to_2d(Dot_3d& dot3d)
{
    Dot_2d dot2d;
    float k;

    if (dot3d.z > 0) k = 1 + dot3d.z / camera.z;
    else k = 1 / (1 + -dot3d.z / camera.z);

    dot2d.x = camera.x + (dot3d.x - camera.x) * k;
    dot2d.y = camera.y + (dot3d.y - camera.y) * k;

    return dot2d;
}

void Console_Screen::set_figure(Figure_3d& FIGURE)
{
    // Projection 3d on 2d plane
    for (int i = 0; i < FIGURE.vertex_number; i++)
    {
        FIGURE.vertexes_projection[i] = convert_dot_3d_to_2d(FIGURE.vertexes[i]);
    }

    // Set edges
    for (int i = 0; i < FIGURE.edge_number; i++)
    {
        set_edge(FIGURE.vertexes_projection[FIGURE.edges[i].begin], FIGURE.vertexes_projection[FIGURE.edges[i].end], FIGURE.dot_edge);
    }

    // Set vertexes
    for (int i = 0; i < FIGURE.vertex_number; i++)
    {
        set_dot(FIGURE.vertexes_projection[i].x, FIGURE.vertexes_projection[i].y, FIGURE.dot_vertex);
    }
}

Console_Screen::Console_Screen(int screen_width, int screen_height, float camera_x, float camera_y, float camera_z)
{
    // Screen settings
    w = screen_width;
    h = screen_height;
    int size = screen_height * (screen_width + 1) + 1; // One extra column for '\n' and last char for '\0'
    table = new char[size];
    camera = { camera_x , camera_y, camera_z };
    dot_clear = ' ';

    // Console settings
    system("mode con cols=100 lines=101");
    system("title Figures 3d");
    system("color 0F");
}

Console_Screen::~Console_Screen()
{
    delete[] table;
}