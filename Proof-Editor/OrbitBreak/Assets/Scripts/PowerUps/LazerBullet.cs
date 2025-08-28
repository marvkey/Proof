using Proof;
using System;


namespace OrbitBreak
{
    public class LazerBullet : Entity
    {
        void OnCreate()
        {
            TriggerEnterEvent += OnTriggerEnter;
        }

        void OnTriggerEnter(Entity other)
        {
            Log.Info("Lazer Bullet Trigger Enter: " + other.Name);
            if (other.GetComponent<TagComponent>().HasSubTag("Enemy"))
            {

                Prefab explison = World.TryFindEntityByTag("CameraAndScenmanager").GetScript<GameManager>().ExplosionLazer;
                if (explison != null)
                {
                    Entity e =  World.Instantiate(explison, Transform.Location);
                    World.DeleteEntity(e, true, 2.0f);  
                }
                World.DeleteEntity(this);
                World.DeleteEntity(other);

                
            }
        }
    }
}
