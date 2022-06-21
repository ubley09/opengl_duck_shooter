//#include <iostream>
#include <SOIL/SOIL.h>
#include <GL/glut.h>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Camera.h"
#include "object.h"
#include "scene.h"
#include "timer.h"

using namespace std;

Camera g_camera;
Scene scene;
Object* obj;

Mix_Chunk *shootSound = NULL;
Mix_Chunk *duckQuack = NULL;

bool g_key[256];
bool g_fps_mode = false;

const float g_rotation_speed = M_PI/180*0.022;

GLuint texture[10];

struct GameGlobalStruct
{
    int points = 0;
    Timer duck_spawn_timer;
    Timer game_timer;
}; GameGlobalStruct game;

struct Window
{
    int width=0, height=0;
}; Window window;

struct FrameTime
{
    int t, old_t;
    float dt;
}; FrameTime frametime;

struct Mouse
{
    float x,y;
    int ap = false;
    bool picking = false;
}; Mouse mouse;

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void close()
{
    //Free the sound effects
    Mix_FreeChunk( shootSound );
    Mix_FreeChunk( duckQuack );
    shootSound = NULL;
    duckQuack = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
    exit(1);
}

void keyboard(unsigned char key, int x, int y) {

    if(key == ' ') {
//        g_fps_mode = !g_fps_mode;

//        if(g_fps_mode) {
//            glutSetCursor(GLUT_CURSOR_NONE);
//            glutWarpPointer(window.width/2, window.height/2);
//        }
//        else {
//            glutSetCursor(GLUT_CURSOR_INHERIT);
//        }
    }

    if(key==27) {
        if (scene.getSceneName() == "menu")
        {
            close();
        }
        else
        {
            scene.loadScene("scene/menu.scn", 1);
            glutSetCursor(GLUT_CURSOR_INHERIT);
            g_camera.Init();
            g_camera.SetPos(-10,0,0);
            g_fps_mode = false;
        }
    }

    g_key[key] = true;

//    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void mouseMove(int x, int y) {
    static bool just_warped = false;

    if(just_warped) {
        just_warped = false;
        return;
    }

    if(g_fps_mode) {
        int dx = x - window.width/2;
        int dy = y - window.height/2;

        if(dx) {
            g_camera.RotateYaw(g_rotation_speed*dx);
        }

        if(dy) {
            g_camera.RotatePitch(-g_rotation_speed*dy);
        }

        glutWarpPointer(window.width/2, window.height/2);

        just_warped = true;
    }
}

void Reshape (int w, int h)
{
    window.width = w;
    window.height = h;

    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection

    glLoadIdentity ();
    gluPerspective(45, (float)window.width/window.height, 0.01f, 1000.0f);
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void picking(int x, int y)
{
    unsigned char pixel[4];
    glReadPixels(x, window.height-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    if (pixel[1] == 0 && pixel[2] == 0)
        for (int i = 0; i < scene.getObjectsSize(); i++)
        {
            obj = &scene.getObject(i);
            if (obj->getPicking() == pixel[0])
            {
                //cout << "\n" << obj->getObjName() << ":Clicked\n";
                Object tobj;
                switch (str2int(obj->getObjName().c_str()))
                {
                    case str2int("duck"):
                        game.points += 150;
                        scene.removeObject(i);
                        Mix_PlayChannel( -1, duckQuack, 0 );
                    break;
                    case str2int("exitButton"):
                        close();
                    break;
                    case str2int("menuButton"):
                    scene.loadScene("scene/menu.scn", 0);
                        g_camera.Init();
                        g_camera.SetPos(-10,0,0);
                        g_fps_mode = false;
                    break;
                    case str2int("startButton"):
                        scene.loadScene("scene/game.scn", 1);
                        g_fps_mode = true;
                        glutSetCursor(GLUT_CURSOR_NONE);
                        glutWarpPointer(window.width/2, window.height/2);
                        tobj.setObjName("Points"); tobj.Init("-");
                        tobj.setText("Points:" + to_string(game.points));
                        tobj.setPos(-1,0.85,-0.1);
                        scene.addObject(tobj);
                        tobj.setObjName("Time"); tobj.Init("-");
                        tobj.setText("Time:" + to_string(6));
                        tobj.setPos(0.5,0.85,-0.1);
                        scene.addObject(tobj);
                        game.points = 0;
                        game.duck_spawn_timer.setTimeRemaining(2);
                        game.game_timer.setTimeRemaining(61);
                    break;
                }
                return;
            }
        }
        else if (scene.getSceneName() == "game")
            game.points -= 50;
}

void display()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)window.width/window.height, 0.01f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Reset transformations
    glLoadIdentity();

    g_camera.Refresh();

    glPushMatrix();

    switch (str2int(scene.getSceneName().c_str())) {
        case str2int("game"):
            if (game.duck_spawn_timer.calculaTimeRemaining(frametime.dt))
            {
                game.duck_spawn_timer.setTimeRemaining(1);
                Object tobj;
                tobj.Init("obj/duck.robj");
                tobj.togglePicking(-1);
                tobj.setDuckRngVar(1 + rand() % 8, rand() % 6, rand() % 4, 1 + rand() % 20, 1.5 + rand() % 4);
                tobj.setPos(-6 + (rand() % 35), 1, -40 + rand() % 20);
                scene.addObject(tobj);
            }
            if (game.game_timer.calculaTimeRemaining(frametime.dt))
            {
                scene.loadScene("scene/endgame.scn", 1);
                g_fps_mode = false;
                glutSetCursor(GLUT_CURSOR_INHERIT);
                Object tobj; tobj.setObjName("Points"); tobj.Init("-");
                tobj.setText("Points:" + to_string(game.points));
                tobj.setPos(-0.25,0.3,-0.1);
                scene.addObject(tobj);
                g_camera.Init();
                g_camera.SetPos(-10,0,0);
            }
        break;
        default:
        break;
    }
    for (int i = 0; i < scene.getObjectsSize(); i++)
    {
        obj = &scene.getObject(i);
        switch (str2int(obj->getObjName().c_str())) {
            case str2int("Points"):
            break;
            case str2int("Time"):
            break;
            case str2int("crosshair"):
            break;
            case str2int("duck"):
                glPushMatrix();
                obj->setcSinY(obj->getcSinY() + obj->getMultiplySinRng() * frametime.dt);
                obj->setPos(obj->getPosX(),
                            sinf(obj->getcSinY()) * obj->getMultiplyOutSinRng() + obj->getAddOutSinRng(),
                            obj->getPosZ() + obj->getDuckSpeedRng() * frametime.dt);
                obj->refreshPosRot();
                if (mouse.picking)
                {
                    obj->togglePicking(i);
                }
                glCallList(obj->getMesh());
                glPopMatrix();
            break;
            case str2int("grass"):
                glPushMatrix();
                obj->refreshPosRot();
                glScalef(35,35,35);
                glCallList(obj->getMesh());
                glPopMatrix();
            break;
            default:
                glPushMatrix();
                obj->refreshPosRot();
                if (mouse.picking)
                {
                    obj->togglePicking(i);
                }
                glCallList(obj->getMesh());
                glPopMatrix();
            break;
        }
    }

    if (mouse.picking)
    {
        picking(mouse.x, mouse.y);
        mouse.picking = false;
        for (int i = 0; i < scene.getObjectsSize(); i++)
        {
            obj = &scene.getObject(i);
            switch (str2int(obj->getObjName().c_str())) {
                case str2int("crosshair"):
                break;
                case str2int("grass"):
                break;
                default:
                    obj->togglePicking(i);
                break;
            }
        }
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, 0.01, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    for (int i = 0; i < scene.getObjectsSize(); i++)
    {
        obj = &scene.getObject(i);
        switch (str2int(obj->getObjName().c_str()))
        {
        case str2int("Points"):
            glPushMatrix();
            obj->refreshPosRot();
            glLineWidth(3);
            glScalef(0.001,0.001,0.001);
            obj->setText("Points:" + to_string(game.points));
            obj->loadObj();
            glCallList(obj->getMesh());
            glPopMatrix();
        break;
        case str2int("Time"):
            glPushMatrix();
            obj->refreshPosRot();
            glLineWidth(3);
            glScalef(0.001,0.001,0.001);
            obj->setText("Time:" + to_string((int)game.game_timer.getTimeRemaining()));
            obj->loadObj();
            glCallList(obj->getMesh());
            glPopMatrix();
            break;
        case str2int("crosshair"):
            glPushMatrix();
            obj->refreshPosRot();
            glScalef(0.1,0.1,0.1);
            glCallList(obj->getMesh());
            glPopMatrix();
        break;
        }
    }
    if (!mouse.ap)
        glutSwapBuffers();
    else
        mouse.ap = false;
}

void mouseButton(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN) {
        if(button == GLUT_LEFT_BUTTON) {
            mouse.x = x;
            mouse.y = y;
            mouse.ap = true;
            mouse.picking = true;
            if (scene.getSceneName()  == "game")
                Mix_PlayChannel( -1, shootSound, 0 );
        }
    }
    else if(state == GLUT_UP) {
        if(button == GLUT_LEFT_BUTTON) {
        }
    }
    glutPostRedisplay();
}

void idle()
{
    /* Delta time in seconds. */
    frametime.t = glutGet(GLUT_ELAPSED_TIME);
    frametime.dt = (frametime.t - frametime.old_t) / 1000.0;
    frametime.old_t = frametime.t;

    glutPostRedisplay();
}

void init()
{
    frametime.old_t = glutGet(GLUT_ELAPSED_TIME);
    scene.loadScene("scene/menu.scn", 1);
    g_camera.SetPos(-10,0,0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)window.width/window.height, 0.01f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.5,0.7,1);
}

int main(int argc, char **argv) {
    // init GLUT and create window
    window.width=1280, window.height=720;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(window.width,window.height);
    glutCreateWindow("OpenGL Duck Shooter");

    // register callbacks
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);

    //Initialize SDL
    if( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return 0;
    }

    //Initialize SDL_mixer
   if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 1, 2048 ) < 0 )
   {
       printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
       return 0;
   }

   //Load sound effects
   shootSound = Mix_LoadWAV( "sound/shoot.mp3" );
   if( shootSound == NULL )
   {
       printf( "Failed to load shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
       return 0;
   }
   duckQuack = Mix_LoadWAV( "sound/duckQuack.mp3" );
   if( duckQuack == NULL )
   {
       printf( "Failed to load duckQuack sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
       return 0;
   }

    init();

    glutMainLoop();
    return 0;
}



