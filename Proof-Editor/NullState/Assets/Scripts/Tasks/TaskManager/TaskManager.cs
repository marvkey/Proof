
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

    public enum TaskType
    {
        None = 0,
        MemoryInjection,
        TraceRemoval
    }
    public abstract class Task : Entity
    {
        protected TaskStatus m_TaskState = TaskStatus.Inactive;
        public TaskStatus State => m_TaskState;

        public TaskType Type { get; protected set; }

        protected Task(TaskType type)
        {
            Type = type;
        }
    }
    public class TaskManager : Entity
    {

        void OnCreate()
        {
        }


        void OnUpdate(float dt)
        {

        }
    }
}