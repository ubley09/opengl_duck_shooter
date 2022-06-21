#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glut.h>
#include <string>
#include "vector"
#include <SOIL/SOIL.h>
#include <cstring>

using namespace std;

class Object
{
public:
    Object();
    void Init(string fname);
    void loadObj();
    GLuint getMesh();
    void refreshPosRot();
    void setPos(float x, float y, float z);
    void setYawPitch(float yaw, float pitch);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);
    void setYaw(float yaw);
    void setPitch(float pitch);
    float getPosX();
    float getPosY();
    float getPosZ();
    string getObjName();
    void setObjName(string obj_name);
    void loadTexture(string tname, bool transparency);
    void togglePicking(int red);
    int getPicking();
    void loadText();
    void setText(string text);
    void setcSinY(float cSinY);
    void setDuckRngVar(float cSiny, float multiplySinRng, float multiplyOutSinRng, float addOutSinRng, float duckSpeedRng);
    float getcSinY();
    float getMultiplySinRng();
    float getMultiplyOutSinRng();
    float getAddOutSinRng();
    float getDuckSpeedRng();
private:
    string obj_name;
    string fname;
    GLuint mesh;
    float posX, posY, posZ;
    float yaw, pitch;
    vector<GLuint> textures;
    int red;
    bool is_text;
    string text;

    float cSinY=0;
    float multiplySinRng=1;
    float multiplyOutSinRng=1;
    float addOutSinRng=0;
    float duckSpeedRng=1;
};

#endif // OBJECT_H
