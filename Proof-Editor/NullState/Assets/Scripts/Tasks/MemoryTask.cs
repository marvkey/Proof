using System;
using Proof;

namespace NullState
{
    public class MemoryInjection : Task
    {

        public MemoryInjection() : base(TaskType.MemoryInjection)
        {
        }
        [ShowInEditor("InjectionRate")] float m_InjectionRate = 0.25f;
        float ProggressRate = 0.0f;

        public void OnUpdate(float deltaTime)
        {

        }

        public void InjectMemory()
        {
            if(m_TaskState == TaskStatus.Completed)
                return;

            ProggressRate += World.GetDeltaTime() * m_InjectionRate;

            m_TaskState = TaskStatus.Running;
            if(ProggressRate > 1.0f)
            {
                m_TaskState = TaskStatus.Completed;
            }
        }
    }

    public class TraceRemoval : Task
    {        
        public TraceRemoval() : base(TaskType.TraceRemoval)
        {
        }
        [ShowInEditor("CleanUpRate")] float m_CleanUpRate = 0.25f;
        [ShowInEditor("MInTraceLevel")] float m_MinTraceLevel = 0.0f;
        [ShowInEditor("MaxTraceLevel")] float m_MaxTraceLevel = 100.0f;
        float m_TraceLevel;

        public float TraceLevel
        {
            get { return m_TraceLevel; }
            set
            {
                m_TraceLevel = Mathf.Clamp(value, m_MinTraceLevel, m_MaxTraceLevel);
            }
        }

        public void AddTrace(float trace)
        {
            TraceLevel += trace;
        }

        bool m_StartRemoveTrace = false;
        public void RemoveTrace()
        {
            m_StartRemoveTrace = true;

            m_TaskState = TaskStatus.Running;
            TraceLevel -= m_CleanUpRate * World.GetDeltaTime();

        }
    }
}