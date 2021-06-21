#include <cmath>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
constexpr auto PI = 3.14159265358979323846;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- 
struct Dot_2d
{
    float x;
    float y;
};
struct Dot_3d
{
    float x;
    float y;
    float z;
};
struct Edge
{
    int begin;
    int end;
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
    Dot_3d rotate_3d_speed;
    Dot_3d rotate_3d_center;

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
            for (int i = 0; i < vertex_number; i++) fscanf_s(fp, "%f %f %f", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z);

            // Read edges
            fscanf_s(fp, "%d", &edge_number);
            edges = new Edge[edge_number];
            for (int i = 0; i < edge_number; i++) fscanf_s(fp, "%d %d", &edges[i].begin, &edges[i].end);

            // Read rotate speed
            fscanf_s(fp, "%f %f %f", &rotate_3d_speed.x, &rotate_3d_speed.y, &rotate_3d_speed.z);

            // Read rotate center
            fscanf_s(fp, "%f %f %f", &rotate_3d_center.x, &rotate_3d_center.y, &rotate_3d_center.z);
        }
        else printf("File open error");
    }

    void dot_rotate_3d(Dot_3d& dot, float deg_rot_x, float deg_rot_y, float deg_rot_z, float rot_x0, float rot_y0, float rot_z0)
    {
        float r, deg0;

        // Move rotation axis to center
        dot.x -= rot_x0;
        dot.y -= rot_y0;
        dot.z -= rot_z0;

        if (deg_rot_x)
        {
            // Switch to polar coordinate system
            r = sqrt(dot.y * dot.y + dot.z * dot.z);
            if (r)
            {
                if (dot.z > 0) deg0 = acos(dot.y / r);
                else deg0 = -acos(dot.y / r);

                // Get result coordinates in rectangle system
                dot.y = r * cos(deg0 + deg_rot_x / 180 * PI);
                dot.z = r * sin(deg0 + deg_rot_x / 180 * PI);
            }
        }
        if (deg_rot_y)
        {
            r = sqrt(dot.x * dot.x + dot.z * dot.z);
            if (r)
            {
                if (dot.z > 0) deg0 = acos(dot.x / r);
                else deg0 = -acos(dot.x / r);

                dot.x = r * cos(deg0 + deg_rot_y / 180 * PI);
                dot.z = r * sin(deg0 + deg_rot_y / 180 * PI);
            }
        }
        if (deg_rot_z)
        {
            r = sqrt(dot.x * dot.x + dot.y * dot.y);
            if (r)
            {
                if (dot.y > 0) deg0 = acos(dot.x / r);
                else deg0 = -acos(dot.x / r);

                dot.x = r * cos(deg0 + deg_rot_z / 180 * PI);
                dot.y = r * sin(deg0 + deg_rot_z / 180 * PI);
            }
        }

        // Move rotation axis from center
        dot.x += rot_x0;
        dot.y += rot_y0;
        dot.z += rot_z0;
    }

    void translate_3d(float dx, float dy, float dz)
    {
        for (int i = 0; i < vertex_number; i++)
        {
            vertexes[i].x += dx;
            vertexes[i].y += dy;
            vertexes[i].z += dz;
        }
        rotate_3d_center.x += dx;
        rotate_3d_center.y += dy;
        rotate_3d_center.z += dz;
    }

    void rotate_3d()
    {
        for (int i = 0; i < vertex_number; i++) dot_rotate_3d(vertexes[i], rotate_3d_speed.x, rotate_3d_speed.y, rotate_3d_speed.z, rotate_3d_center.x, rotate_3d_center.y, rotate_3d_center.z);
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
            for (int j = 0; j < w; j++) table[i * 101 + j] = ' ';
            table[i * 101 + 100] = '\n';
        }
        table[100 * 101] = '\0';
    }

    void print()
    {
        system("cls");
        printf("%s", table);
    }

    void set_dot(float dot_x, float dot_y, char dot)
    {
        int sx = floor(dot_x);
        int sy = floor(dot_y);

        if (sx >= 0 && sy >= 0 && sx < w && sy < h) table[sy * 101 + sx] = dot;
    }

    void set_edge(Dot_2d vert1, Dot_2d vert2, char edge_dot)
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

    Dot_2d convert_dot_3d_to_2d(Dot_3d dot3d)
    {
        Dot_2d dot2d;
        float k;

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
    int dist = 0;
    int dz = -1;
    while (!_kbhit() || _getch() != 27)
    {
        SCREEN.clear();
        FIGURE_1.rotate_3d();
        FIGURE_1.translate_3d(0, 0, dz);
        dist += dz;
        if (dist == 70 || dist == -150) dz *= -1;

        SCREEN.set_figure(FIGURE_1);
        SCREEN.print();
    }

    FIGURE_1.clear();
    return 0;
}