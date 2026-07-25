#pragma once

class Animation
{
public:
    static float EaseInOut(float t);
    static float Pulse(float speed = 3.0f);
    static float Bounce(float speed = 2.0f, float amount = 10.0f);
    static float Wave(float speed = 2.0f, float amount = 20.0f);
};