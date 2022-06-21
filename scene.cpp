#include "scene.h"

#include "object.h"

Scene::Scene()
{

}

void Scene::loadScene(char *fname, bool first_start)
{
    objects.clear();
    FILE *fp;
    int read;
    GLfloat x, y, z, yaw, pitch;
    char str0[20];
    char str1[20];

    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    while(!(feof(fp)))
    {
        read=fscanf(fp,"%s %s %f %f %f %f %f", str0, str1, &x, &y, &z, &yaw, &pitch);
        if(read==2 && string(str0)=="sn")
        {
            scene_name = str1;
        }
        if(read==7 && string(str0)=="obj")
        {
           Object obj;
           obj.Init(str1);
           obj.loadObj();
           obj.setPos(x, y ,z);
           obj.setYawPitch(yaw, pitch);
           if (!first_start)
               obj.togglePicking(0);
           this->addObject(obj);
        }
    }
    fclose(fp);
}

string Scene::getSceneName()
{
    return scene_name;
}

Object& Scene::getObject(int i)
{
    return objects.at(i);
}

int Scene::getObjectsSize()
{
    return objects.size();
}

void Scene::addObject(Object obj)
{
    objects.push_back(obj);
}

void Scene::removeObject(int i)
{
    objects.erase(objects.begin() + i);
}
