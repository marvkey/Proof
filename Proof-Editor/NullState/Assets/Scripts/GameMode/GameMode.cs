using Proof;

namespace NullState
{
    public class GameMode : Entity
    {
        public TaskManager TaskManager;
        void OnCreate()
        {
            Entity task = World.CreateEntity("TaskManager", Proof.Transform.Zero);

            task.AddScript<TaskManager>();
            TaskManager = task.GetScript<TaskManager>();
        }
    }
}