#include "Camera.h"

void Camera::Init()
{
    m_yaw = 0.0;
    m_pitch = 0.0;

    SetPos(0, 0, 0);
}

void Camera::Refresh()
{
    // Camera parameter according to Riegl's co-ordinate system
    // x/y for flat, z for height
    m_lx = cos(m_yaw) * cos(m_pitch);
    m_ly = sin(m_pitch);
    m_lz = sin(m_yaw) * cos(m_pitch);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_x, m_y, m_z, m_x + m_lx, m_y + m_ly, m_z + m_lz, 0.0,1.0,0.0);

    //printf("Camera: %f %f %f Direction vector: %f %f %f\n", m_x, m_y, m_z, m_lx, m_ly, m_lz);
}

void Camera::SetPos(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;

    Refresh();
}

void Camera::GetPos(float &x, float &y, float &z)
{
    x = m_x;
    y = m_y;
    z = m_z;
}

void Camera::GetDirectionVector(float &x, float &y, float &z)
{
    x = m_lx;
    y = m_ly;
    z = m_lz;
}

void Camera::RotateYaw(float angle)
{
    const float limit = 89.0 * M_PI / 180.0;

    m_yaw += angle;

    if(m_yaw < -limit)
        m_yaw = -limit;

    if(m_yaw > limit)
        m_yaw = limit;

    Refresh();
}

void Camera::RotatePitch(float angle)
{
    const float limit = 89.0 * M_PI / 180.0;

    m_pitch += angle;

    if(m_pitch < -limit)
        m_pitch = -limit;

    if(m_pitch > limit)
        m_pitch = limit;

    Refresh();
}

void Camera::SetYaw(float angle)
{
    m_yaw = angle;

    Refresh();
}

void Camera::SetPitch(float angle)
{
    m_pitch = angle;

    Refresh();
}

float Camera::GetYaw()
{
    return m_yaw;
}

float Camera::GetPitch()
{
    return m_pitch;
}
