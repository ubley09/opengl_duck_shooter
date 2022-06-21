#ifndef TIMER_H
#define TIMER_H


class Timer
{ 
public:
    Timer();
    bool calculaTimeRemaining(float delta_time);
    bool isExpired();
    float getTimeRemaining();
    void setTimeRemaining(float timeRemaining);
private:
    float timeRemaining;

};

#endif // TIMER_H
