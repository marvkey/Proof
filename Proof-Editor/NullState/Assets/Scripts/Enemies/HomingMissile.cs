using Proof;
using System;
using System.Threading;

namespace NullState
{
    public class HomingMissile : Entity
    {

        public Entity TargetEntity;
        RigidBodyComponent m_RigidBody;

        [SeperatorAttribute]
        [ShowInEditorAttribute] float Speed = 43.0f;
        [ShowInEditorAttribute] float RotationSpeed = 95.0f;

        [SeperatorAttribute]
        [ShowInEditorAttribute("MaxDistancePredict")] private float m_MaxDistancePredict = 100;
        [ShowInEditorAttribute("MinDistancePredict")] private float m_MinDistancePredict = 5;
        [ShowInEditorAttribute("MaxTimePrediction")] private float m_MaxTimePrediction = 5;

        private Vector3 m_StandardPrediction, m_DeviatedPrediction;

        [SeperatorAttribute]
        [ShowInEditorAttribute("DeviationAmount")] private float m_DeviationAmount = 50;
        [ShowInEditorAttribute("DeviationSpeed")] private float m_DeviationSpeed = 2;

        [SeperatorAttribute]
        public Prefab ExplosionPrefab;

        void OnCreate()
        {
            m_RigidBody = GetComponent<RigidBodyComponent>();
            CollisionEnterEvent += OnCollisionEnter;
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
            if(m_RigidBody ==null)
            {
                Log.Info($"Rigid Body {Name}  cannot be null");
                return;
            }
            if (TargetEntity == null)
            {
                Log.Info($"Target entity {Name}  cannot be null");
                return;
            }
            m_RigidBody.Velocity = -Transform.Forward * Speed;

            var leadTimePercentage = Mathf.InverseLerp(m_MinDistancePredict, m_MaxDistancePredict, Vector3.Distance(Transform.Location, TargetEntity.Transform.Location));

            PredictMovement(leadTimePercentage);

            AddDeviation(leadTimePercentage);

            RotateRocket();

        }

        void PredictMovement(float leadTimePercentage)
        {
            var predictionTime = Mathf.Lerp(0, m_MaxTimePrediction, leadTimePercentage);

            RigidBodyComponent targetRigidBody = TargetEntity.GetComponent<RigidBodyComponent>();
            m_StandardPrediction = targetRigidBody.Location + targetRigidBody .Velocity * predictionTime;
        }

        void AddDeviation(float leadTimePercentage)
        {
            var deviation = new Vector3(Mathf.Cos(Application.GetTime() * m_DeviationSpeed), 0, 0);

            var predictionOffset = Transform.TransformDirection(deviation) * m_DeviationAmount * leadTimePercentage;

            m_DeviatedPrediction = m_StandardPrediction + predictionOffset;
        }

        private void RotateRocket()
        {
           // var heading = m_DeviatedPrediction - Transform.Location;
            var heading = TargetEntity.Location - Transform.Location;

            //var rotation = Quaternion.LookRotation(heading,Mathf.Up);
            m_RigidBody.Rotation= Quaternion.LookRotation(heading,Mathf.Up);

           // m_RigidBody.Rotation = Quaternion.Slerp(m_RigidBody.Rotation, rotation,0.5f);
           // m_RigidBody.Rotate(Quaternion.RotateTowards(Transform.RotationQuat, rotation, RotationSpeed * World.GetDeltaTime()));
        }

        private void OnCollisionEnter(Entity e)
        {
            if (ExplosionPrefab != null)
            {
                World.Instantiate(ExplosionPrefab, Transform.Location);
            }

            if(HasScript<DamageComponent>())
            {
                var damageComp = GetScript<DamageComponent>();
                damageComp.DealDamage(e);
            }



            Log.Trace($"{Name} Rockdet hit {e.Name}");
            Destroy(this);
        }
    }
}
