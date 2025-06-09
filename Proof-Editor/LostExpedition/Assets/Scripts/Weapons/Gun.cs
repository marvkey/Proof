
using System;
using Proof;

namespace LostExpedition
{

    public class Gun : Weapon
	{
        public float Damage = 10.0f;
		public float FireRate = 0.2f;
		public float ReloadTime = 1.5f;
        public float maxDistance = 100f;
		public int MagazineSize = 30;
        public float SpreadAngle = 5f; 

        public float RecoilAngle = 5.0f; // How much to tilt upwards (degrees)
        private int Ammo = 30;

        public Prefab MuzzleFlash;
        public Entity MuzzleFlashPos;
        float m_TimeSinceLastShot;

        private float m_RecoilTimer = 0.0f;
        private float m_RecoilCurrent = 0.0f;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_TimeSinceLastShot = (FireRate) *2; // want it to be 100% ready to fire on first shot
        }

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
            m_TimeSinceLastShot += deltaTime;
            HandleRecoil(deltaTime);
        }

        
		public Entity Fire(TransformComponent playerTransform, TransformComponent cameraTransform) // player hit
		{
            if (!CanFire())
                return null;

            Entity e = Shoot(playerTransform, cameraTransform);
            m_TimeSinceLastShot = 0.0f;
            Ammo--;
            m_RecoilTimer = FireRate; // reset recoil timer


            if (e.GetScript<HealthComponent>() != null)
            {
                e.GetScript<HealthComponent>().TakeDamage(Damage);
            }

            if(MuzzleFlash != null)
            {
                Entity outMuzzle = World.Instantiate(MuzzleFlash,MuzzleFlashPos.Location,this);
                World.DeleteEntity(outMuzzle,true, FireRate); // delete after 2 seconds
            }
            return e;

        }

		Entity Shoot(TransformComponent playerTransform, TransformComponent cameraTransform)
		{
            RaycastData data = new RaycastData();
            data.Origin = playerTransform.Location;
            data.Direction = ApplyBulletSpread(cameraTransform.Forward).Normalized;
            data.MaxDistance = maxDistance;
            data.ExcludedEntities = new ulong[2];
            data.ExcludedEntities[0] = GetParent().ID; // don't hit self
            data.ExcludedEntities[1] = ID; // don't hit self

            if (Physics.RayCast(data, out RaycastHit hit))
            {
                return hit.Entity;
            }
            return null;
        }

        bool CanFire()
		{
            return Ammo > 0 && m_TimeSinceLastShot >= FireRate;
        }

        /// <summary>
        /// Returns a stability factor between 0.5 and 1 based on how long it's been since the last shot.
        /// - If you shoot exactly at the fire rate, this returns 0.5 (maximum spread).
        /// - If you wait twice as long as the fire rate, it returns 1 (fully stable, no spread).
        /// </summary>
        float GetShotStability()
        {
            float ratio = Mathf.Clamp(m_TimeSinceLastShot / FireRate, 1f, 2f);
            return ratio / 2f;
        }

        Vector3 ApplyBulletSpread(Vector3 forward)
        {
            float actualspreadAngle = SpreadAngle * (1.0f - GetShotStability()); // e.g., max 5 degrees

            // Generate random offset within a cone
            Vector3 right = Vector3.Cross(forward, new Vector3(0, 1, 0)).Normalized;
            Vector3 up = Vector3.Cross(right, forward).Normalized;

            float angleRad = Mathf.DegreesToRadians(actualspreadAngle);
            float randX = Proof.Random.Float(-1.0f, 1.0f);
            float randY = Proof.Random.Float(-1.0f, 1.0f);

            Vector3 finalSpread = forward +
                               right * randX * Mathf.Tan(angleRad) +
                               up * randY * Mathf.Tan(angleRad);

            return finalSpread.Normalized;
        }

        void ApplyRecoilRotation(float rotation)
        {
            RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(rotation), 0.0f, 0.0f));
        }

        void HandleRecoil(float deltaTime)
        {
            if (m_RecoilTimer > 0.0f)
            {
                m_RecoilTimer -= deltaTime;
                float t = 1.0f - (m_RecoilTimer / FireRate);

                t = Mathf.Clamp(t, 0.0f, 1.0f); // ensure t is between 0 and 1

                // Use smoother falloff (cubic or exponential)
                float falloff = 1.0f - Mathf.SmoothStep(0.0f,1.0f,t); // falloff factor, 1 at start, 0 at end
                m_RecoilCurrent = RecoilAngle * falloff; // smooth falloff
                ApplyRecoilRotation(m_RecoilCurrent);
            }
            else
            {
                m_RecoilCurrent = 0.0f;
            }
        }

    }
}
