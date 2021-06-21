#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define PI 3.14159265358979323846

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
struct Dot_2d
{
    double x;
    double y;
};
struct Dot_3d
{
    double x;
    double y;
    double z;
};
struct Edge
{
    int begin;
    int end;
};
struct Rotate_3d_Speed
{
    int x;
    int y;
    int z;
};
struct Figure_3d
{
    int vertex_number;
    int edge_number;
    Dot_2d* vertexes_projection;
    Dot_3d* vertexes;
    Edge* edges;
    char dot_edge = (char)249;
    char dot_vertex = (char)219;
    Rotate_3d_Speed rotate_speed;

    void read_from_file(const char* fname)
    {
        FILE* fp;

        fopen_s(&fp, fname, "rt");
        if (fp)
        {
            // Read vertexes
            fscanf_s(fp, "%d", &vertex_number);

            vertexes = new Dot_3d[vertex_number];
            vertexes_projection = new Dot_2d[vertex_number];

            float vert_x, vert_y, vert_z;
            for (int i = 0; i < vertex_number; i++)
            {
                fscanf_s(fp, "%f %f %f", &vert_x, &vert_y, &vert_z);
                vertexes[i].x = vert_x;
                vertexes[i].y = vert_y;
                vertexes[i].z = vert_z;
            }

            // Read edges
            fscanf_s(fp, "%d", &edge_number);

            edges = new Edge[edge_number];

            for (int i = 0; i < edge_number; i++)
            {
                fscanf_s(fp, "%d %d", &edges[i].begin, &edges[i].end);
            }

            for (int i = 0; i < 3; i++)
            {
                fscanf_s(fp, "%d %d %d", &rotate_speed.x, &rotate_speed.y, &rotate_speed.z);
            }

        }
        else printf("File open error");
    }

    void Rotate_Z(Dot_3d& vert, double deg_rot, double rot_x0, double rot_y0)
    {
        if (deg_rot == 0) return;

        double deg_res, deg0;

        // Move rotation axis to center
        vert.x -= rot_x0;
        vert.y -= rot_y0;

        // Switch to polar coordinate system
        double r = sqrt(vert.x * vert.x + vert.y * vert.y);
        if (r)
        {
            if (vert.x > 0) deg0 = asin(vert.y / r);
            else deg0 = PI - asin(vert.y / r);
            deg_res = deg0 + deg_rot / 180 * PI;

            // Get result coordinates in rectangle system
            vert.x = r * cos(deg_res);
            vert.y = r * sin(deg_res);
        }

        // Move rotation axis from center
        vert.x += rot_x0;
        vert.y += rot_y0;
    }

    void Rotate_Y(Dot_3d& vert, double deg_rot, double rot_x0, double rot_z0)
    {
        if (deg_rot == 0) return;

        double deg_res, deg0;

        // Move rotation axis to center
        vert.x -= rot_x0;
        vert.z -= rot_z0;

        // Switch to polar coordinate system
        double r = sqrt(vert.x * vert.x + vert.z * vert.z);
        if (r)
        {
            if (vert.x > 0) deg0 = asin(vert.z / r);
            else deg0 = PI - asin(vert.z / r);
            deg_res = deg0 + deg_rot / 180 * PI;

            // Get result coordinates in rectangle system
            vert.x = r * cos(deg_res);
            vert.z = r * sin(deg_res);
        }

        // Move rotation axis from center
        vert.x += rot_x0;
        vert.z += rot_z0;
    }

    void Rotate_X(Dot_3d& vert, double deg_rot, double rot_y0, double rot_z0)
    {
        if (deg_rot == 0) return;

        double deg_res, deg0;

        // Move rotation axis to center
        vert.y -= rot_y0;
        vert.z -= rot_z0;

        // Switch to polar coordinate system
        double r = sqrt(vert.y * vert.y + vert.z * vert.z);
        if (r)
        {
            if (vert.y > 0) deg0 = asin(vert.z / r);
            else deg0 = PI - asin(vert.z / r);
            deg_res = deg0 + deg_rot / 180 * PI;

            // Get result coordinates in rectangle system
            vert.y = r * cos(deg_res);
            vert.z = r * sin(deg_res);
        }

        // Move rotation axis from center
        vert.y += rot_y0;
        vert.z += rot_z0;
    }

    void ratate_3d(int rot_x = 50, int rot_y = 50, int rot_z = 0)
    {
        for (int i = 0; i < vertex_number; i++)
        {
            Rotate_X(vertexes[i], rotate_speed.x, rot_y, rot_z);
            Rotate_Y(vertexes[i], rotate_speed.y, rot_x, rot_z);
            Rotate_Z(vertexes[i], rotate_speed.z, rot_x, rot_y);
        }
    }

    void clear()
    {
        delete[] vertexes;
        delete[] vertexes_projection;
        delete[] edges;
    }
};

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- SCREEN -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
struct Console_Screen
{
    const int w = 100;
    const int h = 100;
    char table[100 * 101 + 1]; // one extra column for '\n' and last char for '\0'
    Dot_3d camera = { 50, 50, 50 };

    void clear()
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                table[i * 101 + j] = ' ';
            }
            table[i * 101 + 100] = '\n';
        }
        table[100 * 101] = '\0';
    }

    void print()
    {
        system("cls");
        printf("%s", table);
    }

    void set_dot(double dot_x, double dot_y, char dot)
    {
        int sx = floor(dot_x);
        int sy = floor(dot_y);

        if (sx >= 0 && sy >= 0 && sx < w && sy < h)
        {
            table[sy * 101 + sx] = dot;
        }
    }

    void set_edge(Dot_2d vert1, Dot_2d vert2, char edge_dot)
    {
        double x, y;

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

    Dot_2d convert_dot_3d_to_2d(Dot_3d dot3d)
    {
        Dot_2d dot2d;
        double k;

        if (dot3d.z > 0) k = 1 + dot3d.z / camera.z;
        else k = 1 / (1 + -dot3d.z / camera.z);

        dot2d.x = camera.x + (dot3d.x - camera.x) * k;
        dot2d.y = camera.y + (dot3d.y - camera.y) * k;

        return dot2d;
    }

    void set_figure(Figure_3d FIGURE)
    {
        // Projection 3d on 2d SCREEN
        for (int i = 0; i < FIGURE.vertex_number; i++) FIGURE.vertexes_projection[i] = convert_dot_3d_to_2d(FIGURE.vertexes[i]);

        // Set edges
        for (int i = 0; i < FIGURE.edge_number; i++) set_edge(FIGURE.vertexes_projection[FIGURE.edges[i].begin], FIGURE.vertexes_projection[FIGURE.edges[i].end], FIGURE.dot_edge);

        // Set vertexes
        for (int i = 0; i < FIGURE.vertex_number; i++) set_dot(FIGURE.vertexes_projection[i].x, FIGURE.vertexes_projection[i].y, FIGURE.dot_vertex);
    }
};


Figure_3d FIGURE_1;
Console_Screen SCREEN;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- MAIN -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int main()
{
    // Console settings
    system("mode con cols=100 lines=101");
    system("title Figures 3d");
    system("color 0F");

    // Read figure data
    FIGURE_1.read_from_file("figure0.dat");

    // Figure drawing
    while (true)
    {
        SCREEN.clear();
        FIGURE_1.ratate_3d();
        SCREEN.set_figure(FIGURE_1);
        SCREEN.print();

        if (_kbhit()) if (_getch() == 27) break;
        Sleep(5);
    }

    FIGURE_1.clear();
    return 0;
}