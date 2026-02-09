using Proof;

namespace NullState
{
    public class NullVeil : ContinousPowerUp
    {
        protected override void OnEnabled()
        {
            // TODO: hook into detection/trace systems
        }

        protected override void OnDisabled(bool forced)
        {
        }

        protected override void OnWhileEnabled(float dt)
        {
        }
        
    }
}