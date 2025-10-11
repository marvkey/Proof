using Proof;
using System;
using System.Reflection.Emit;
using System.Runtime.InteropServices;

namespace NullState
{
    public class Turret : Entity
    {
        Entity m_Target;

        public Entity Muzzle;
        public Prefab Bullet;
        public Prefab MuzzleFlash;
        public bool RocketTurret = false;

        public float MuzzleFlashTime = 0.5f;
        public float Range = 15.0f;
        public float RotationSpeed = 5.0f;
        public bool HasTarget => m_Target != null;

        [ToolTip("When a player is locked on how long before shooting")]
        public float WaitTimeBeforeShoot = 1.0f;

        [SeperatorAttribute]
        public float FireRate = 5.0f;
        public float BulletSpeed = 100.0f;

        [SeperatorAttribute]
        public float RaySpreadAngle = 5.0f; 
        public int NumRays = 7;

        float m_LockOnPlayer;
        bool LockedOnPlayer => m_LockOnPlayer > WaitTimeBeforeShoot;
        float m_TimeSinceLastShot;

        void OnCreate()
        {
            InvokeRepeat(SphereCheck, 1.0f);
            m_TimeSinceLastShot = (FireRate) * 2; // want it to be 100% ready to fire on first shot
        }

        void OnUpdate(float deltaTime)
        {
            m_TimeSinceLastShot += deltaTime;
            LookAtTarget(deltaTime);
            Shoot();


        }
        void LookAtTarget(float deltaTime)
        {
            if(HasTarget == false)
            {
                m_LockOnPlayer = 0.0f;
                return;
            }


            Vector3 direction = (m_Target.Transform.Location - Transform.Location).Normalized;

            // rotating aroudnthis so keep it at 0
            // so only rottate aruond y axis
            direction.y = 0; 

            Quaternion targetRotation = Quaternion.LookRotation(direction, Mathf.Up);

            GetComponent<RigidBodyComponent>().Rotation = Quaternion.Slerp(GetComponent<RigidBodyComponent>().Rotation, targetRotation, deltaTime * RotationSpeed);
            m_LockOnPlayer += deltaTime;
        }

        void Shoot()
        {
            if (HasTarget == false)
                return;

            if (LockedOnPlayer == false)
                return;

            if (!CanFire())
                return;

            Vector3 dir, hitPos;
            if (DetectTarget(out dir, out hitPos) == false)
                return;

            Entity lazer = World.Instantiate(Bullet,Muzzle.WorldTransform.Location);
            Entity muzzleFlsh = World.Instantiate(MuzzleFlash, Muzzle.WorldTransform.Location);
            World.DeleteEntity(muzzleFlsh, true, MuzzleFlashTime);
            lazer.GetComponent<RigidBodyComponent>().Rotation = Quaternion.LookRotation(dir, Mathf.Up);

            if (lazer.HasScript<HomingMissile>())
            {
                HomingMissile missile = lazer.GetScript<HomingMissile>();

                missile.TargetEntity = m_Target;
            }
            if (lazer.HasComponent<RigidBodyComponent>() == true && RocketTurret == true)
            {

                var body = lazer.GetComponent<RigidBodyComponent>();
                body.Velocity = dir * BulletSpeed;

            }
            m_TimeSinceLastShot = 0.0f;
        }
        void SphereCheck()
        {
           Entity[] entities = Physics.OverlapSphere(Transform.Location, Range);

            if (entities.Length == 0)
            {
                if(m_Target != null)
                    Log.Trace("Target Lost: " + m_Target.Name);

                m_Target = null;
                return;
            }

            foreach (Entity entity in entities)
            {
                if(entity.HasScript<Character>())
                {
                    if(m_Target == entity)
                        return;
                    m_Target = entity;
                    Log.Trace("Target Acquired: " + m_Target.Name);
                    return;
                }
            }

            if (m_Target != null)
                Log.Trace("Target Lost: " + m_Target.Name);

            m_Target = null;
        }

        bool CanFire()
        {
            return m_TimeSinceLastShot >= FireRate;
        }
        bool DetectTarget(out Vector3 hitDirection, out Vector3 hitPoint)
        {
            hitDirection = Vector3.Zero;
            hitPoint = Vector3.Zero;

            if (m_Target == null)
                return false;

            if(Muzzle == null)
            {
                Log.Error("Turret needs a muzzle");
                return false;
            }
            
            Vector3 origin = Muzzle.Transform.WorldTransform.Location;

            Vector3 targetPos = m_Target.Transform.Location + new Vector3(0, 1.5f, 0); // aim chest/head height
            float coneAngle = Mathf.DegreesToRadians(5f); // vision cone

            for (int i = 0; i < NumRays; i++)
            {
                // Add small random deviation

                Vector3 dir = RandomDirectionInCone((targetPos - origin).Normalized, coneAngle);

                //Vector3 dir = (targetPos - origin).Normalized;
                //dir += RandomSpread(dir, RaySpreadAngle);
                //dir = dir.Normalized;

                RaycastData ray = new RaycastData();
                ray.Origin = origin;
                ray.Direction = dir;
                ray.MaxDistance = Range;

                if (Physics.RayCast(ray, out RaycastHit hit))
                {
                    if (hit.EntityID == m_Target.ID)
                    {
                        hitDirection = dir;
                        hitPoint = hit.Position;

                        DebugRenderer.DrawRay(origin, dir, Range, Colors.Red);
                        return true; // clear hit to player
                    }
                }
            }

            return false;
        }

        Vector3 RandomDirectionInCone(Vector3 forward, float angle)
        {
            // pick a random rotation within the cone angle
            float u = Proof.Random.Float(0f, 1f);
            float v = Proof.Random.Float(0f, 1f);

            float theta = 2f * Mathf.PI * u;     // azimuth
            float phi = angle * Mathf.Sqrt(v);   // elevation (denser center)

            // spherical to cartesian
            float x = Mathf.Sin(phi) * Mathf.Cos(theta);
            float y = Mathf.Sin(phi) * Mathf.Sin(theta);
            float z = Mathf.Cos(phi);

            // direction in local space
            Vector3 localDir = new Vector3(x, y, z);
             
            // rotate localDir to align with forward
            Quaternion rot = Quaternion.FromToRotation(Mathf.Back, forward);
            return rot * localDir;
        }


        Vector3 RandomSpread(Vector3 dir, float angle)
        {
            Vector3 randomVec = new Vector3(
                Proof.Random.Float(-angle, angle),
                Proof.Random.Float(-angle, angle),
                Proof.Random.Float(-angle, angle)
            );

            return randomVec * 0.01f; // convert to degrees small ofsfset
        }
    }
}
