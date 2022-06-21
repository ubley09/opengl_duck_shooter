#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <math.h>
#include <GL/glut.h>

class Camera
{
public:
    Camera() { Init(); }
    ~Camera(){}

    void Init();
    void Refresh();
    void SetPos(float x, float y, float z);
    void GetPos(float &x, float &y, float &z);
    void GetDirectionVector(float &x, float &y, float &z);
    void SetYaw(float angle);
    void SetPitch(float angle);
    float GetYaw();
    float GetPitch();

    // Navigation
    void RotateYaw(float angle);
    void RotatePitch(float angle);

private:
    float m_x, m_y, m_z;   // Position
    float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
    float m_yaw, m_pitch; // Various rotation angles
};

#endif
