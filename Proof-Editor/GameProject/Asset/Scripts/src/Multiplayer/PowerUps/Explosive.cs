using Proof;

namespace Game
{
    class Explosive : OpponentPowerUp
    {
        public float ForceStrentght = 3000.0f;
        protected void OnCreate()
        {
            Image = new Texture(13596771993452602895);

            GetComponent<TransformComponent>().Scale = new Vector(10);
            GetComponent<TransformComponent>().Location = new Vector(GetComponent<TransformComponent>().Location.X, GetComponent<TransformComponent>().Location.Y + 8, GetComponent<TransformComponent>().Location.Z);
        }
        protected override void ApplyPowerUp()
        {
            if (m_Owner == null || m_CollidedPlayer == null) return;

            m_CollidedPlayer.GetComponent<ParticleSystemComponent>().GetParticle(1).Play();
            m_CollidedPlayer.GetComponent<RigidBodyComponent>().AddForce(new Vector(0, ForceStrentght, 0), ForceMode.Impule);
            M_PowerupApplied = true;
        }

    }
}
