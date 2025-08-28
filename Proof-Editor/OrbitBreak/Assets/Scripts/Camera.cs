using System;
using Proof;

namespace OrbitBreak
{
    public class Camera : Entity
    {
        float Duration = 0.2f;
        float Magnitude = 0.2f;

        Vector3 originalPos;
        float timer = 0f;
        bool shaking = false;

        void OnCreate()
        {
            originalPos = Transform.Location;
        }

        public void Shake(float duration, float magnitude)
        {
            Duration = duration;
            Magnitude = magnitude;
            timer = 0f;
            shaking = true;
        }

        void OnUpdate(float dt)
        {
            if (!shaking) return;

            timer += dt;
            if (timer < Duration)
            {
                float x = Proof.Random.Float(-1f, 1f) * Magnitude;
                float y = Proof.Random.Float(-1f, 1f) * Magnitude;
                Transform.Location = originalPos + new Vector3(x, y, 0);
            }
            else
            {
                Transform.Location = originalPos;
                shaking = false;
            }
        }
    }
}
