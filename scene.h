#ifndef SCENE_H
#define SCENE_H

#include "vector"
#include "object.h"

using namespace std;

class Scene
{
public:
    Scene();
    string getSceneName();
    void loadScene(char *fname, bool first_start);
    Object& getObject(int i);
    int getObjectsSize();
    void addObject(Object obj);
    void removeObject(int i);
private:
    string scene_name;
    vector<Object> objects;
};

#endif // SCENE_H
