#pragma once
#include <memory>
#include <iostream>
#include "SmartPointer.h"

#define PROOF_EXPAND_MACRO(x) x
#define PROOF_STRINGIFY_MACRO(x) #x
#define Proof_API
// Add this when locating a file, but not fore built in functions it is already automatically added
inline const std::string ProofCurrentDirectorySrc = "../Proof/src/";
inline const std::string ProofCurrentDirectory = "../Proof/";
#define BIT(x) (1 << x)
#define GetVariableName(X)#X
#define PF_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. does not work on pointers

#define DEFINE_ENUM_CLASS_FLAGS(Enum) \
    inline Enum& operator|=(Enum& lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        lhs = static_cast<Enum>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs)); \
        return lhs; \
    } \
    inline Enum& operator&=(Enum& lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        lhs = static_cast<Enum>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs)); \
        return lhs; \
    } \
    inline Enum& operator^=(Enum& lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        lhs = static_cast<Enum>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs)); \
        return lhs; \
    } \
    inline constexpr Enum operator|(Enum lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        return static_cast<Enum>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs)); \
    } \
    inline constexpr Enum operator&(Enum lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        return static_cast<Enum>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs)); \
    } \
    inline constexpr Enum operator^(Enum lhs, Enum rhs) { \
        using underlying_type = std::underlying_type_t<Enum>; \
        return static_cast<Enum>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs)); \
    }

namespace Proof
{
    
    using MemoryAddress = uintptr_t;
    using byte = uint8_t;
    template<typename T>
    using Special = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    inline constexpr Special<T> CreateSpecial(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
