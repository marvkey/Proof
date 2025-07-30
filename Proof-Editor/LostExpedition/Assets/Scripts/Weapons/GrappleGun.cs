using System;
using Proof;
namespace LostExpedition
{
    class GrappleGun : Gun
    {
        private Vector3 grapplePoint;
        public Transform gunTip, camera, player;
        private float maxDistance = 100f;
        private SpringJoint joint;
    }

}
