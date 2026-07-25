#include "Animation.h"
#include "raylib.h"
#include <cmath>

float Animation::EaseInOut(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

float Animation::Pulse(float speed)
{
    return 0.75f + 0.25f * std::sin(GetTime() * speed);
}

float Animation::Bounce(float speed, float amount)
{
    return std::sin(GetTime() * speed) * amount;
}

float Animation::Wave(float speed, float amount)
{
    return std::sin(GetTime() * speed) * amount;
}