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
		String,
        AssetKey
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

            if (typeof(T) == typeof(string))
            {
                SetStringData(data as string);
                return;
            }

            if (typeof(Asset).IsAssignableFrom(typeof(T)))
            {
                Asset asset = data as Asset;

                if(!asset.ID.IsValid())
                {
                    return;
                }
                else
                {
                    AssetKey key = new AssetKey
                    {
                        ID = asset?.ID ?? AssetID.Invalid,
                        Type = asset.Type
                    };
                    SetData(key);
                }

                return;
            }

            int size = Marshal.SizeOf<T>();
            IntPtr dataPtr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(data, dataPtr, false);

                VariableRaw variableRaw = new VariableRaw(VariableUUID, Type, StorageHandle);
                InternalCalls.ProofScriptVariable_SetValue(variableRaw, dataPtr,(uint)size);
            }
            finally
            {
                Marshal.FreeHGlobal(dataPtr);
            }
        }

        public T GetData<T>()
        {
            if (typeof(T) == typeof(string))
            {
                string strValue = GetStringData(); // call your actual string method
                return (T)(object)strValue;
            }
            if (typeof(Asset).IsAssignableFrom(typeof(T)))
            {
                AssetKey key = GetData<AssetKey>();

                if (!key.ID.IsValid() || key.Type == AssetType.None)
                    throw new InvalidOperationException($"Invalid AssetKey for type {typeof(T).Name}: ID is invalid or type is None.");

                return (T)Activator.CreateInstance(typeof(T), key.ID);
            }

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
        private void SetStringData(string value)
        {
            // Convert to null-terminated UTF-8 byte array
            byte[] bytes = System.Text.Encoding.UTF8.GetBytes(value + '\0');

            IntPtr dataPtr = Marshal.AllocHGlobal(bytes.Length);
            try
            {
                Marshal.Copy(bytes, 0, dataPtr, bytes.Length);

                VariableRaw variableRaw = new VariableRaw(VariableUUID, Type, StorageHandle);
                InternalCalls.ProofScriptVariable_SetValue(variableRaw, dataPtr, (uint)bytes.Length);
            }
            finally
            {
                Marshal.FreeHGlobal(dataPtr);
            }
        }


        private string GetStringData()
        {
            VariableRaw variableRaw = new VariableRaw(VariableUUID, Type, StorageHandle);
            IntPtr dataPtr = InternalCalls.ProofScriptVariable_GetValue(variableRaw);

            if (dataPtr == IntPtr.Zero)
                return string.Empty;

            // Manually read null-terminated UTF-8 string
            List<byte> bytes = new List<byte>();
            int offset = 0;
            byte current;

            do
            {
                current = Marshal.ReadByte(dataPtr, offset);
                if (current != 0)
                    bytes.Add(current);
                offset++;
            } while (current != 0);

            return System.Text.Encoding.UTF8.GetString(bytes.ToArray());
        }
    }
}

