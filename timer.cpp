#include "timer.h"

Timer::Timer(){}

bool Timer::calculaTimeRemaining(float delta_time)
{
    if (timeRemaining > 0)
    {
        timeRemaining -= delta_time;
        return false;
    }
    else
        return true;
}

bool Timer::isExpired()
{
    return timeRemaining > 0 ? false : true;
}

float Timer::getTimeRemaining()
{
    return timeRemaining;
}

void Timer::setTimeRemaining(float timeRemaining)
{
    this->timeRemaining = timeRemaining;
}
