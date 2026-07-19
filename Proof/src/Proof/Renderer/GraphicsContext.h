#pragma once
#include "Proof/Core/Core.h"

namespace Proof
{

    enum class GPUType : uint8_t
    {
        Unknown,    // GPU type could not be determined.
        Integrated, // GPU is integrated into the CPU or motherboard.
        Discrete,   // Dedicated standalone GPU with its own video memory.
        Virtual,    // Virtual GPU exposed by a virtual machine.
        CPU         // Graphics operations are performed by the CPU.
    };

    struct GraphicsAPIVersion
    {
        uint32_t Major = 0; // Major API version, such as 1 in Vulkan 1.3.
        uint32_t Minor = 0; // Minor API version, such as 3 in Vulkan 1.3.
        uint32_t Patch = 0; // Patch or revision number of the API version.
    };

    struct GPUInfo
    {
        // Human-readable GPU name, such as "NVIDIA GeForce RTX 3050".
        std::string Name;

        // Human-readable installed graphics driver version.

        // Raw driver version supplied by the graphics backend.
        uint32_t DriverVersion = 0;

        // Human-readable GPU manufacturer name, such as NVIDIA, AMD, or Intel.
        std::string VendorName;

        // Numeric identifier for the GPU manufacturer.
        // Examples:
        // NVIDIA = 0x10DE
        // AMD    = 0x1002
        // Intel  = 0x8086
        uint32_t VendorID = 0;

        // Numeric identifier for the specific GPU model or chipset.
        // This is not a unique serial number.
        uint32_t DeviceID = 0;

        // General hardware category of the GPU.
        GPUType Type = GPUType::Unknown;

        // Name of the graphics API currently being used.
        // Examples: "Vulkan", "DirectX 12", "Metal", or "OpenGL".
        std::string GraphicsAPI;

        // Version of the graphics API supported or currently being used.
        GraphicsAPIVersion APIVersion;

        // Maximum number of resource or descriptor sets that can be bound
        // to the graphics pipeline at the same time.
        uint32_t MaxBoundResourceSets = 0;

        // Minimum byte alignment required when placing constant or uniform
        // buffer data at an offset inside a larger GPU buffer.
        uint64_t MinimumConstantBufferAlignment = 0;

        std::string GetVendorName() const
        {
            switch (VendorID)
            {
            case 0x10DE: return std::string("NVIDIA");

            case 0x1002:
            case 0x1022: return std::string("AMD");

            case 0x8086: return std::string("Intel");
            case 0x106B: return std::string("Apple");
            case 0x13B5: return std::string("ARM");
            case 0x5143: return std::string("Qualcomm");
            case 0x1010: return std::string("Imagination Technologies");
            case 0x1414: return std::string("Microsoft");

            default: return {};
            }
        }
        std::string GetAPIVersionString() const
        {
            if (APIVersion.Major == 0 &&
                APIVersion.Minor == 0 &&
                APIVersion.Patch == 0)
            {
                return {};
            }

            return fmt::format(
                "{}.{}.{}",
                APIVersion.Major,
                APIVersion.Minor,
                APIVersion.Patch
            );
        }
        // Returns a newly created driver-version string each time.
        // The exact decoding can be changed by each graphics backend.
        std::string GetDriverVersion() const
        {
            if (DriverVersion == 0)
                return {};

            return fmt::format("{}", DriverVersion);
        }

        // Returns the device ID as a newly created hexadecimal string.
        std::string GetDeviceIDString() const
        {
            if (DeviceID == 0)
                return {};

            return fmt::format("0x{:04X}", DeviceID);
        }
    };


    class Proof_API GraphicsContext : public RefCounted {
    public:
        static Count<GraphicsContext> Create();
        virtual ~GraphicsContext(){}
        virtual GPUInfo GetGPUInfo() = 0;
    };
}