#include "object.h"


using namespace std;

Object::Object()
{
}

void Object::Init(string fname)
{
    this->fname = fname;
    posX=0,posY=0,posZ=0;
    yaw=0, pitch=0;
    red = -1;
    is_text = false;
    mesh=glGenLists(1);
}

void Object::refreshPosRot()
{
    glTranslatef(posX,posY,posZ);
    glRotatef(-yaw, 0, 1, 0);
    glRotatef(pitch, 0, 0, 1);
}

void Object::loadObj()
{
    if (is_text)
    {
        loadText();
        return;
    }

    FILE *fp;
    int read;
    GLfloat x, y, z;
    char str0[20];
    char str1[20];


    fp=fopen(fname.c_str(),"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname.c_str());
        exit(1);
    }
    glNewList(mesh, GL_COMPILE);
    {
        if (red == -1)
        {
            while(!(feof(fp)))
            {
                read=fscanf(fp,"%s %s %f %f %f",str0,str1,&x,&y,&z);
                if(read==3 && string(str0)=="texture")
                {
                    this->loadTexture(str1, x);
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, textures.back());
                    if (x == 1)
                    {
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                    } else
                    {
                        break;
                    }
                }
            }
            fp=fopen(fname.c_str(),"r");
        }

        glPushMatrix();
        glBegin(GL_QUADS);
        if (red == -1)
            glNormal3f(-1,0,0);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%s %s %f %f %f",str0,str1,&x,&y,&z);
            if(read==2 && string(str0)=="on")
            {
                obj_name = str1;
            }
            if(read==5 && string(str0)=="c")
            {
                if (red == -1)
                {
                    glColor3f(x,y,z);
                } else
                {
                    glColor3ub(red,0,0);
                }
            }
            if(read==4 && string(str0)=="t" && red == -1)
            {
                glTexCoord2f(x, y);
            }
            if(read==5 && string(str0)=="v")
            {
                glVertex3f(x,y,z);
            }

        }
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }
    glEndList();
    fclose(fp);
}

GLuint Object::getMesh()
{
    return mesh;
}

void Object::setPos(float x, float y, float z)
{
    posX = x;
    posY = y;
    posZ = z;
}
void Object::setYawPitch(float yaw, float pitch)
{
    this->yaw=yaw;
    this->pitch=pitch;
}
void Object::setPosX(float x)
{
    posX = x;
}
void Object::setPosY(float y)
{
    posY = y;
}
void Object::setPosZ(float z)
{
    posZ = z;
}
void Object::setYaw(float yaw)
{
    this->yaw=yaw;
}
void Object::setPitch(float pitch)
{
    this->pitch=pitch;
}

float Object::getPosX()
{
    return posX;
}
float Object::getPosY()
{
    return posY;
}
float Object::getPosZ()
{
    return posZ;
}

string Object::getObjName()
{
    return obj_name;
}

void Object::setObjName(string obj_name)
{
    this->obj_name = obj_name;
}

void Object::loadTexture(string tname, bool transparency)
{
    textures.push_back(SOIL_load_OGL_texture
                       (
                           tname.c_str(),
                           transparency ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                           ));
}

void Object::togglePicking(int red_p)
{
    if (red == -1)
    {
        red = red_p;
    }
    else
    {
        red = -1;
    }
    loadObj();
}

int Object::getPicking()
{
    return red;
}

void Object::loadText()
{
    char chtext[50];
    strcpy(chtext, text.c_str());
    glNewList(mesh, GL_COMPILE);
    {
        glPushMatrix();
        for (int i = 0; i < int(strlen(chtext)); i++)
        {
            glColor3f(1,1,0);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, chtext[i]);
        }
        glPopMatrix();
    }
    glEndList();
}

void Object::setText(string text)
{
    this->text = text;
    is_text = true;
}

void Object::setcSinY(float cSinY)
{
    this->cSinY = cSinY;
}

void Object::setDuckRngVar(float cSiny, float multiplySinRng, float multiplyOutSinRng, float addOutSinRng, float duckSpeedRng)
{
    this->cSinY = cSinY;
    this->multiplySinRng = multiplySinRng;
    this->multiplyOutSinRng = multiplyOutSinRng;
    this->addOutSinRng = addOutSinRng;
    this->duckSpeedRng = duckSpeedRng;
}

float Object::getcSinY()
{
    return cSinY;
}
float Object::getMultiplySinRng()
{
    return multiplySinRng;
}
float Object::getMultiplyOutSinRng()
{
    return multiplyOutSinRng;
}
float Object::getAddOutSinRng()
{
    return addOutSinRng;
}
float Object::getDuckSpeedRng()
{
    return duckSpeedRng;
}

