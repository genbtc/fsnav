#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
#include <cstdio>
#include "main.h"
#include "fstree.h"
#include "text.h"
#include "stereo.h"

extern unsigned int fontrm, fonttt, fonttt_sm;
extern unsigned int scope_tex;
extern const char* root_dirname;
extern const char* data_dirname;
extern int stereo;
#define PROGRAM_NAME "FSN: Filesystem Visualizer 1.0 - ported by genBTC"
extern Dir *root;

int main(int argc, char **argv)
{
    fprintf(stderr, "FSNav Starting! Loading OpenGL...\n");
    glutInitWindowSize(1920, 1080);
    glutInit(&argc, argv);

    if (parse_args(argc, argv) == -1)
        return 1;
    if (glutMainCompose() == 1)
        return 1;
    glutMainLoop();
    return 0;
}

int glutMainCompose()
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE | (stereo ? GLUT_STEREO : 0));
    glutCreateWindow(PROGRAM_NAME);

    glutDisplayFunc(disp);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyb);
    glutKeyboardUpFunc(keyb_up);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive_motion);

    glEnable(GLUT_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    set_layout_param(LP_FILE_SIZE, 0.5);
    set_layout_param(LP_FILE_SPACING, 0.1);
    set_layout_param(LP_FILE_HEIGHT, 0.1);

    set_layout_param(LP_DIR_SIZE, 0.5 + 0.2);
    set_layout_param(LP_DIR_SPACING, 0.5);
    set_layout_param(LP_DIR_HEIGHT, 0.1);
    set_layout_param(LP_DIR_DIST, 5.0);

    root = new Dir;
    if (!build_tree(root, root_dirname)) {
        return 1;
    }
    root->layout();

    if (!((fontrm = create_font(find_data_file("kerkis.pfb"), 32)))) {
        return 1;
    }
    bind_font(fontrm);
    set_text_color(0.95, 0.9, 0.8, 1.0);

    if (!((fonttt = create_font(find_data_file("courbd.ttf"), 16)))) {
        return 1;
    }
    bind_font(fonttt);
    set_text_color(1.0, 1.0, 1.0, 1.0);

    if (!((fonttt_sm = create_font(find_data_file("courbd.ttf"), 14)))) {
        return 1;
    }
    bind_font(fonttt_sm);
    set_text_color(1.0, 1.0, 1.0, 1.0);

    const unsigned int font_texid = get_font_texture();
    glBindTexture(GL_TEXTURE_2D, font_texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (!((scope_tex = load_texture(find_data_file("scope.png"))))) {
        return 1;
    }

    glEnable(GL_NORMALIZE);
    glEnable(GL_LINE_SMOOTH);

    stereo_focus_dist(25.0);
    return 0;
}

int parse_args(int argc, char **argv)
{
    int i;

    for (i = 1; i<argc; i++) {
        if (argv[i][0] == '-' && argv[i][2] == 0) {
            switch (argv[i][1]) {
            case 's':
                stereo = !stereo;
                break;

            default:
                fprintf(stderr, "invalid option: %s\n", argv[i]);
                return -1;
            }
        }
        else {
            if (root_dirname) {
                fprintf(stderr, "unexpected argument: %s\n", argv[i]);
                return -1;
            }
            root_dirname = argv[i];
        }
    }

    if (!root_dirname) {
        root_dirname = "C:/Users/Default";
    }
    return 0;
}