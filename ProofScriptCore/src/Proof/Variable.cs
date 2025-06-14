using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{

   public enum VariableTypes
    {
        None = 0,
		Bool,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
		String
    };

    [StructLayout(LayoutKind.Sequential)]
    struct VariableRaw
    {
        public ulong VariableUUID;
        public int Type; // VariableTypes enum
        public ulong StorageHandle;

        public VariableRaw(ulong variableUUID =0, VariableTypes type = VariableTypes.None, ulong storagehandle = 0)
        {
            VariableUUID = variableUUID;
            Type = (int)type;
            StorageHandle = storagehandle;
        }
    }
    [StructLayout(LayoutKind.Sequential)]
    public class Variable
    {
        public ulong VariableUUID;
        public VariableTypes Type; // VariableTypes enum
        public ulong StorageHandle;

        public Variable(ulong variableUUID, VariableTypes type, ulong storageHandle)
        {
            VariableUUID = variableUUID;
            Type = type;
            StorageHandle = storageHandle;
        }

        public void SetData<T> (T data)
        {
            int size = Marshal.SizeOf<T>();
            IntPtr dataPtr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(data, dataPtr, false);

                VariableRaw variableRaw = new VariableRaw(VariableUUID, Type, StorageHandle);
                InternalCalls.ProofScriptVariable_SetValue(variableRaw, dataPtr,(ulong)size);
            }
            finally
            {
                Marshal.FreeHGlobal(dataPtr);
            }
        }

        public T GetData<T>()
        {
            int size = Marshal.SizeOf<T>();
            VariableRaw variableRaw = new VariableRaw(VariableUUID, Type, StorageHandle);

            IntPtr dataPtr = InternalCalls.ProofScriptVariable_GetValue(variableRaw);
            if (dataPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to retrieve variable data.");
            }
            try
            {
                return Marshal.PtrToStructure<T>(dataPtr);
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException($"Error while retrieving variable data: {ex.Message}", ex);
            }
        }
    }
}

