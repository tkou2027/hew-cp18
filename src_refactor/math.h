#pragma once
#include <cmath>

namespace hoge
{
	class Math
	{
	public:
		static constexpr float EPSILON{ 1e-6 };
        
        static float Lerp(float a, float b, float t)
        {
            return a * (1 - t) + b * t;
        }

        // for color
        static int Lerp(int a, int b, float t)
        {
            float res = a * (1 - t) + b * t;
            return RoundToInt(res);
        }

        static float IfZero(float x) {
            return x > -EPSILON && x < EPSILON;
        }

        static int RoundToInt(float x)
        {
            return static_cast<int>(x + 0.5f);
        }

        static float Round(float x, float t) {
            if (IfZero(t)) {
                return x;
            }
            float t_inv = 1.0f / t;
            return (float)((int)(x * t_inv + 0.5)) * t;
        }

        // round to 0.5
        static float RoundHalf(float x) {
            return roundf(x * 2.0f) / 2.0f;
        }

        static float Sign(float x) {
            return x > EPSILON ? 1 : (x < -EPSILON ? -1 : 0);
        }

        static float Clamp(float x, float min, float max) {
            return x < min ? min : (x > max ? max : x);
        }

        static int Clamp(int min, int x, int max) {
            return x < min ? min : (x > max ? max : x);
        }
	};
}