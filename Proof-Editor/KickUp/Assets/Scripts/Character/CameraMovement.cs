using System;
using Proof;

namespace KickUp
{
    public class CameraMovement : Entity
    {
        [Seperator]
        public Entity Target;

        [Seperator]
        public float MouseSensitivity = 2.5f;
        public float CameraDistance = 8f;
        public float CameraHeight = 3f;

        public float MinPitch = -10f;
        public float MaxPitch = 45f;

        private float m_Yaw;
        private float m_Pitch = 20f;

        public void AddCameraInput(float mouseX, float mouseY)
        {
            m_Yaw += mouseX * MouseSensitivity;
            m_Pitch -= mouseY * MouseSensitivity;

            m_Pitch = Mathf.Clamp(
                m_Pitch,
                MinPitch,
                MaxPitch
            );
        }

        private void LateUpdate()
        {
            UpdateCamera();
        }

        private void UpdateCamera()
        {
            if (Target == null)
                return;

            Quaternion rotation = Quaternion.EularToQuat(
                m_Pitch,
                m_Yaw,
                0f
            );

            Vector3 targetPosition =
                Target.Location +
                Mathf.Up * CameraHeight;

            Vector3 cameraPosition =
                targetPosition -
                rotation *
                Mathf.Forward *
                CameraDistance;

            Location = cameraPosition;
            
            
            RotationQuat = Quaternion.LookRotation(targetPosition.Normalized, Mathf.Up);
        }
    }
}