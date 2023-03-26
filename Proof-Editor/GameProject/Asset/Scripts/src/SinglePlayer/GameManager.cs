using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public class GameManager : Entity
    {
        public Entity Player;

        public Prefab Obstacle;
        public Prefab Plane;

        private float time = 1000;
        void OnCreate()
        {

        }

        void OnUpdate(float ts)
        {
            if (Player == null || Obstacle == null) return;
            if(time == 1000)
            { 

                Log.Info("new Enitty spawn");

                Entity ent = World.Instanciate(Obstacle, new Vector(0));
                ent.Name = "Just got Created";
                time = 0;
            }
            time+=100;

        }

    }
}
