using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public static class PersistentDataStorage
    {
        public static void SaveDataRaw(string textData, byte[] data)
        {
            //InternalCalls.PersistentDataStorage_SaveData(textData, data, data.Length);

        }
        public static void SaveData<T> (string name, T data) where T : struct
        {
            int size = Marshal.SizeOf<T>();
            IntPtr dataPtr = Marshal.AllocHGlobal(size);
            try
            {
               Marshal.StructureToPtr(data, dataPtr, false);
               InternalCalls.PersistentDataStorage_SaveData(name, dataPtr, (ulong)size);
            }
            finally
            {
                Marshal.FreeHGlobal(dataPtr);
            }
        }

        public static T? LoadData<T>(string name) where T : struct
        {
            //if (!InternalCalls.PersistentDataStorage_HasData(name))
               // return null;

            IntPtr dataPtr = InternalCalls.PersistentDataStorage_LoadData(name, Marshal.SizeOf<T>());

            if (dataPtr == IntPtr.Zero)
            {
                return null;
            }

            try
            {
                // Attempt to marshal the data
                T result = Marshal.PtrToStructure<T>(dataPtr);
                return result;
            }
            catch (Exception ex)
            {
                Log.Error($"Error while loading data for '{name}': {ex.Message}");
                return null; // Return null on error
            }
        }
    }
}
