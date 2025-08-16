using System;
using Proof;

namespace LostExpedition
{
    public class Weapon : InventoryItem
    {
        

        void OnUpdate(float deltaTime)
        {
            base.OnUpdate(deltaTime);
            HandleInventorySwap(deltaTime);
        }


        float m_InventorySwapTimer = 0.0f;
        float m_InventorySwapDuration = 0.25f;
        bool m_SwappingIn = false;
        bool m_IsSwapping = false;

        void HandleInventorySwap(float deltaTime)
        {
            if (!m_IsSwapping)
                return;

            m_InventorySwapTimer += deltaTime;
            float t = Mathf.Clamp(m_InventorySwapTimer / m_InventorySwapDuration, 0, 1);
            float smoothed = Mathf.SmoothStep(0, 1, t);

            float fromAngle = 0.0f;
            float toAngle = 90.0f;

            if (m_SwappingIn)
            {
                fromAngle = 90.0f;
                toAngle = 0.0f;
            }

            float angle = Mathf.Lerp(fromAngle, toAngle, smoothed);
            RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(angle), 0.0f, 0.0f));

            if (t >= 1.0f)
            {
                m_IsSwapping = false;
            }
        }
        public override void SetActiveInventory()
        {
            m_SwappingIn = true;
            m_IsSwapping = true;
            m_InventorySwapTimer = 0.0f;
            GetComponent<MeshComponent>().Visible = true;
        }

        public override void SetDeactiveInventory()
        {
            m_SwappingIn = false;
            m_IsSwapping = true;
            m_InventorySwapTimer = 0.0f;

            GetComponent<MeshComponent>().Visible = false;
        }
    }
}
