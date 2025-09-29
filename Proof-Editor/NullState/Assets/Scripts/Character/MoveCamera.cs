using System;
using Proof;

namespace NullState
{
    public class MoveCamera : Entity
    {
        public Entity CameraPosition;
        void OnUpdate(float ts)
        {
            Location = CameraPosition.Transform.WorldTransform.Location;
        }
    }
}
