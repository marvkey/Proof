using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{

    public class Score : Entity
    {
        Entity owner;

        private TransformComponent m_OwnerTransform;

        private TextComponent m_TextComponent;
        void OnCreate()
        {
            owner = GetOwner();
            if(owner != null)
                m_OwnerTransform = owner.GetComponent<TransformComponent>();
            m_TextComponent = GetComponent<TextComponent>();
        }

        void OnUpdate(float ts)
        { 
            if(owner == null || m_TextComponent ==null) return;

            int score = ((int)m_OwnerTransform.Location.Z);

            m_TextComponent.Text = score.ToString();
        }
    }
}
