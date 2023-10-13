#include "Proofprch.h"
#include "ScriptGCManager.h"

#include <mono/metadata/object.h>
#include <mono/metadata/mono-gc.h>
namespace Proof
{
    using ReferenceMap = std::unordered_map<ScriptGCHandle, MonoObject*>;

    struct ScriptGCState
    {
        ReferenceMap StrongReferences;
        ReferenceMap WeakReferences;
    };

    ScriptGCState* s_ScriptGCStateData = nullptr;
    void ScriptGCManager::Init()
    {
        PF_CORE_ASSERT(!s_ScriptGCStateData, "Trying to initialize ScriptGC Manager multiple times!");

        s_ScriptGCStateData = new ScriptGCState();
    }
    void ScriptGCManager::Shutdown()
    {
        if (s_ScriptGCStateData->StrongReferences.size() > 0)
        {
            PF_ENGINE_ERROR("ScriptEngine {}", "Memory leak detected!");
            PF_ENGINE_ERROR("ScriptEngine {}", "Not all GCHandles have been cleaned up!");

            for (auto [handle, monoObject] : s_ScriptGCStateData->StrongReferences)
                mono_gchandle_free_v2(handle);

            s_ScriptGCStateData->StrongReferences.clear();
        }

        if (s_ScriptGCStateData->WeakReferences.size() > 0)
        {
            PF_ENGINE_ERROR("ScriptEngine {}", "Memory leak detected!");
            PF_ENGINE_ERROR("ScriptEngine {} ", "Not all GCHandles have been cleaned up!");

            for (auto [handle, monoObject] : s_ScriptGCStateData->WeakReferences)
                mono_gchandle_free_v2(handle);

            s_ScriptGCStateData->WeakReferences.clear();
        }

        // Collect any leftover garbage
        mono_gc_collect(mono_gc_max_generation());
        while (mono_gc_pending_finalizers());

        delete s_ScriptGCStateData;
        s_ScriptGCStateData = nullptr;
    }
    ScriptGCHandle ScriptGCManager::CreateObjectReference(MonoObject* managedObject, bool weakReference, bool pinned, bool track)
    {
        ScriptGCHandle handle = weakReference ? mono_gchandle_new_weakref_v2(managedObject, pinned) : mono_gchandle_new_v2(managedObject, pinned);
        PF_CORE_ASSERT(handle, "Failed to retrieve valid GC Handle!");

        if (track)
        {
            if (weakReference)
                s_ScriptGCStateData->WeakReferences[handle] = managedObject;
            else
                s_ScriptGCStateData->StrongReferences[handle] = managedObject;
        }

        return handle;

    }
    bool ScriptGCManager::IsHandleValid(ScriptGCHandle handle)
    {
        if (handle == nullptr)
            return false;

        MonoObject* obj = mono_gchandle_get_target_v2(handle);

        if (obj == nullptr)
            return false;

        if (mono_object_get_vtable(obj) == nullptr)
            return false;

        return true;
    }
    MonoObject* ScriptGCManager::GetReferencedObject(ScriptGCHandle handle)
    {
        MonoObject* obj = mono_gchandle_get_target_v2(handle);
        if (obj == nullptr || mono_object_get_vtable(obj) == nullptr)
            return nullptr;
        return obj;
    }
    void ScriptGCManager::ReleaseObjectReference(ScriptGCHandle handle)
    {
        if (mono_gchandle_get_target_v2(handle) != nullptr)
        {
            mono_gchandle_free_v2(handle);
        }
        else
        {
            PF_ENGINE_ERROR("Tried to release an object reference using an invalid handle!");
            return;
        }

        if (s_ScriptGCStateData->StrongReferences.find(handle) != s_ScriptGCStateData->StrongReferences.end())
            s_ScriptGCStateData->StrongReferences.erase(handle);

        if (s_ScriptGCStateData->WeakReferences.find(handle) != s_ScriptGCStateData->WeakReferences.end())
            s_ScriptGCStateData->WeakReferences.erase(handle);
    }

   
   
    void ScriptGCManager::CollectGarbage(bool blockUntilFinalized)
    {
        PF_PROFILE_FUNC();
        PF_ENGINE_INFO("Collecting garbage...");
        mono_gc_collect(mono_gc_max_generation());
        if (blockUntilFinalized)
        {
            while (mono_gc_pending_finalizers());
            PF_ENGINE_INFO("GC Finished...");
        }
    }
}