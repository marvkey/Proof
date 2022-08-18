using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public static class Log
    {
        enum LogType:int
        {
            ERROR,
            WARN,
            INFO,
            TRACE,
            CRITICAL
        };
        public static void Error(string message, params object[] args) => Log_Internal(LogType.ERROR, string.Format(message, args));
        public static void Warn(string message, params object[] args) => Log_Internal(LogType.WARN, string.Format(message, args));
        public static void Info(string message, params object[] args) => Log_Internal(LogType.INFO, string.Format(message, args));
        public static void Trace(string message, params object[] args) => Log_Internal(LogType.TRACE, string.Format(message, args));
        public static void Critical(string message, params object[] args) => Log_Internal(LogType.CRITICAL, string.Format(message, args));

        public static void Error(object obj) => Log_Internal(LogType.ERROR, obj.ToString());
        public static void Warn(object obj) => Log_Internal(LogType.WARN, obj.ToString());
        public static void Trace(object obj) => Log_Internal(LogType.TRACE, obj.ToString());
        public static void Critical(object obj) => Log_Internal(LogType.CRITICAL, obj.ToString());
        public static void Info(object obj) => Log_Internal(LogType.INFO, obj.ToString());
        
        private static void Log_Internal(LogType logType, string message)
        {
            InternalCalls.Log_Message((int)logType, message);
        }
    }
}
