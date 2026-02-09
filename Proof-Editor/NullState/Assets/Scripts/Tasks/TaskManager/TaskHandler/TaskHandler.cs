namespace NullState
{
    public interface TaskHandler
    {
        void OnUpdate(float dt);
        bool IsComplete { get; }

        
    }

    public class MemoryInjectionHandler : TaskHandler
    {
        private float _progress;
        private float _injectRate;   // percent per second
        private float _maxRate;

        public float Progress01 => _progress;
        public bool IsComplete => _progress >= 1.0f;

        MemoryInjection m_Task;

        public MemoryInjectionHandler(MemoryInjection task, float injectRate = 0.25f)
        {
            m_Task = task;

            _injectRate = injectRate;
            _maxRate = injectRate;
            _progress = 0.0f;
        }

        public void OnUpdate(float dt)
        {
            _progress += _injectRate * dt;

            // Clamp
            if (_progress > 1.0f)
                _progress = 1.0f;
        }

        public void Boost(float amount)
        {
        }

        public void ResetRate()
        {
            _injectRate = _maxRate;
        }
    }

}