
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "main.h"
#include "fstree.h"
#include "text.h"
#include "vis.h"
#include "imago/image.h"
#include "stereo.h"

#ifndef GL_BGRA
#define GL_BGRA     0x80e1
#endif

#define TRANSITION_TIME 1.0
#define PROGRAM_NAME "FSN: Filesystem Visualizer 1.0 - ported by genBTC"

static float cam_theta = 25, cam_phi = 25, cam_dist = 25;
static float cam_y = 25;
static Vector3 cam_from, cam_targ;
static unsigned int cam_motion_start;

static float mouse_x, mouse_y;
static Ray mouse_ray;
static int xsz, ysz;
static FSNode *clicked_node;
static bool hover_file_info;
Dir *root;  //shared in main.cpp :
//  all of these are :
const char* root_dirname;
const char* data_dirname;
int stereo;
unsigned int fontrm, fonttt, fonttt_sm;
unsigned int scope_tex;

std::string udir(getenv("USERPROFILE"));

const char *find_data_file(const char *fname)
{
    static char buf[2048];
    char *cwd = getcwd(0, 0);
    FILE *fp;

    sprintf(buf, "%s/data/%s", cwd, fname);
    delete cwd;
    if((fp = fopen(buf, "rb"))) {
        fclose(fp);
        return buf;
    }
    return fname;
}

void disp()
{
    unsigned int msec = glutGet(GLUT_ELAPSED_TIME);

    float t = (msec - cam_motion_start) / 1000.0 / TRANSITION_TIME;
    if(t > 1.0) {
        t = 1.0;
    }
    Vector3 cam_pos = lerp(cam_from, cam_targ, t);

    if(stereo) {
        glDrawBuffer(GL_BACK_LEFT);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    stereo_proj_matrix(stereo ? VIEW_LEFT : VIEW_CENTER);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    stereo_view_matrix(stereo ? VIEW_LEFT : VIEW_CENTER);
    glTranslatef(0, 0, -cam_dist);
    glRotatef(cam_phi, 1, 0, 0);
    glRotatef(cam_theta, 0, 1, 0);
    glTranslatef(-cam_pos.x, -cam_pos.y, -cam_pos.z);

    render();

    if(stereo) {
        glDrawBuffer(GL_BACK_RIGHT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        stereo_proj_matrix(VIEW_RIGHT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        stereo_view_matrix(VIEW_RIGHT);
        glTranslatef(0, 0, -cam_dist);
        glRotatef(cam_phi, 1, 0, 0);
        glRotatef(cam_theta, 0, 1, 0);
        glTranslatef(-cam_pos.x, -cam_pos.y, -cam_pos.z);

        render();
    }

    glutSwapBuffers();
    assert(glGetError() == GL_NO_ERROR);

    if(t < 1.0) {
        glutPostRedisplay();
    }
}

void render()
{
    float lpos[] = {-0.5, 1, 0.5, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    draw_env();
    root->draw();

    FSNode *sel = get_selection();
    if((sel && hover_file_info) || clicked_node) {
        if(!hover_file_info) {
            sel = clicked_node;
        }

        if(dynamic_cast<File*>(sel)) {
            draw_file_stats((File*)sel);
        }
    }
}

Ray calc_mouse_ray(int x, int y)
{
    Ray ray;

    double mvmat[16], proj[16];
    int viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, viewport);

    double res_x, res_y, res_z;
    gluUnProject(x, y, 0.0, mvmat, proj, viewport, &res_x, &res_y, &res_z);

    ray.origin = Vector3(res_x, res_y, res_z);

    gluUnProject(x, y, 1.0, mvmat, proj, viewport, &res_x, &res_y, &res_z);
    ray.dir = Vector3(res_x, res_y, res_z) - ray.origin;

    return ray;
}

void reshape(int x, int y)
{
    xsz = x;
    ysz = y;
    glViewport(0, 0, x, y);

    stereo_proj_param(50.0, (float)x / (float)y, 0.5, 500.0);
}

void changedir(std::string dirname)
{
    root = new Dir;
    root_dirname = dirname.c_str();
    build_tree(root, root_dirname);
    root->layout();
    glutPostRedisplay();
}

void changedir(const char *dirname)
{
    root = new Dir;
    root_dirname = dirname;
    build_tree(root, root_dirname);
    root->layout();
    glutPostRedisplay();
}

void goUpdir( )
{
    root = new Dir;
    std::string dirstr(root_dirname);
    size_t found = dirstr.find_last_of("/\\");
    root_dirname = (dirstr.substr(0,found)).c_str();
    build_tree(root, root_dirname);
    root->layout();
    glutPostRedisplay();
}

//Char codes:
#define BACKSPACE   8
#define ESCAPEKEY   27
#define MINUS 45
#define PLUS 61
#define SPACEBAR    ' '

//Scancodes:
#define ENTER       13  // for this to print "ENTER", 2 key presses are needed
#define PAGE_UP     73
#define HOME        71
#define END         79
#define PAGE_DOWN   81
#define UP_ARROW    72
#define LEFT_ARROW  75
#define DOWN_ARROW  80
#define RIGHT_ARROW 77

void keyb(unsigned char key, int x, int y)
{
    //char *homedir = ;
    switch(key) {
    case GLUT_KEY_HOME: //change to userdir
        changedir((udir+"\\Desktop").c_str());
        break;
    case ESCAPEKEY:
        exit(0);
        break;
    case GLUT_KEY_PAGE_UP:  //change to up a dir
        goUpdir();
        break;
    case MINUS:
        cam_dist += 2.0;
        glutPostRedisplay();
        break;
    case PLUS:
        cam_dist -= 2.0;
        glutPostRedisplay();
        if(cam_dist < 0) cam_dist = 0;
        break;
    case BACKSPACE:
        changedir("C:/Users/Default");
        break;
    case PAGE_UP:
        changedir("");
        break;
    case ' ':
        hover_file_info = true;
        clicked_node = 0;
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void keyb_up(unsigned char key, int x, int y)
{
    if(key == SPACEBAR) {
        hover_file_info = false;
        glutPostRedisplay();
    }
}

#define DOUBLE_CLICK_INTERVAL   400
static int bnstate[16];

static int prev_x = -1, prev_y;
void mouse(int bn, int state, int x, int y)
{
    const auto specialKey = glutGetModifiers();
    static unsigned int prev_left_click;
    static int prev_left_x, prev_left_y;

    bnstate[bn] = state == GLUT_DOWN ? 1 : 0;
    if(state == GLUT_DOWN) {
        if(bn == GLUT_LEFT_BUTTON) {
            unsigned int msec = glutGet(GLUT_ELAPSED_TIME);
            int dx = abs(x - prev_left_x);
            int dy = abs(y - prev_left_y);

            if(msec - prev_left_click < DOUBLE_CLICK_INTERVAL && dx < 3 && dy < 3) {
                double_click(x, y);
                prev_left_click = 0;
            }
            else {
                prev_left_click = msec;
                prev_left_x = x;
                prev_left_y = y;
            }
        }

        if(bn == 3) {
            cam_dist -= 4;
            if (specialKey == GLUT_ACTIVE_SHIFT)
                cam_dist -= 6;
            glutPostRedisplay();
            if(cam_dist < 0) cam_dist = 0;
        }
        else if(bn == 4) {
            if (specialKey == GLUT_ACTIVE_SHIFT)
                cam_dist += 6;
            cam_dist += 4;
            glutPostRedisplay();
        }
        else {
            prev_x = x;
            prev_y = y;
        }
    }
    else {
        if(bn == GLUT_LEFT_BUTTON) {
            if(x == prev_left_x && y == prev_left_y) {
                clicked_node = get_selection();
                glutPostRedisplay();
            }
        }

        prev_x = -1;
    }
}

void motion(int x, int y)
{
    if(bnstate[0]) {
        cam_theta += (x - prev_x) * 0.5;
        cam_phi += (y - prev_y) * 0.5;

        if(cam_phi < 5) cam_phi = 5;
        if(cam_phi > 90) cam_phi = 90;

        glutPostRedisplay();
    }

    if(bnstate[1]) {
        cam_y += (prev_y - y) * 0.1;
        glutPostRedisplay();
    }

    if(bnstate[2]) {
        cam_dist += (y - prev_y) * 0.1;
        glutPostRedisplay();
    }

    prev_x = x;
    prev_y = y;
}

void passive_motion(int x, int y)
{
    mouse_x = (float)x / (float)xsz;
    mouse_y = (float)y / (float)ysz;

    mouse_ray = calc_mouse_ray(x, ysz - y);
    if(root->pick(mouse_ray)) {
        glutPostRedisplay();
    }

    if(hover_file_info) {
        glutPostRedisplay();
    }
}

void double_click(int x, int y)
{
    FSNode *selnode = get_selection();

    if(selnode) {
        cam_from = cam_targ;
        cam_targ = selnode->get_vis_pos();
        cam_motion_start = glutGet(GLUT_ELAPSED_TIME);
        glutPostRedisplay();
    }
}

unsigned int load_texture(const char *fname)
{
    void *img;
    int width, height;
    unsigned int tex;

    if(!((img = load_image(fname, &width, &height)))) {
        fprintf(stderr, "failed to load image: %s\n", fname);
        return 0;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img);

    free_image(img);
    return tex;
}
