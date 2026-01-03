
using System;
using System.Collections.Generic;
using Proof;

namespace NullState
{

    public enum TaskStatus
    {
        Inactive,// Created, not started
        Running,// Started, updating every frame
        Completed,// Finished successfully
        Failed,// Ended unsuccessfully
        Cancelled // Manually stopped / interrupted
    };
    public class Task : Entity
    {
            
        protected TaskStatus m_TaskState = TaskStatus.Inactive;
        public TaskStatus State {get{return m_TaskState;}}
    }
    public class TaskManager : Entity
    {
        List<Task> m_Tasks;

        void OnCreate()
        {
            m_Tasks = new List<Task>();
            {
                Entity memTask = World.CreateEntity("MemoryInjection", this);
                memTask.AddScript<MemoryInjection>();
                m_Tasks.Add(memTask.GetScript<MemoryInjection>());
            }

            {
                Entity memTask = World.CreateEntity("TraceRemoval", this);
                memTask.AddScript<TraceRemoval>();
                m_Tasks.Add(memTask.GetScript<TraceRemoval>());
            }
        }


        void OnUpdate(float dt)
        {

        }
    }
}