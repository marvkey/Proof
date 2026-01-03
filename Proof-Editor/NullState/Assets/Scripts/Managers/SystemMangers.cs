using Proof;

namespace NullState
{
    public class SystemMangers  : Entity
    {
        GlitchManager m_GlitchManager;

        public GlitchManager GlitchManager
        {
            get
            {
                if (m_GlitchManager == null)
                    m_GlitchManager = GetScript<GlitchManager>();

                return m_GlitchManager;
            }
        }      
    }
}