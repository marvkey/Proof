#include "Proofprch.h"
#include "PhysicsDebugger.h"
#include "PhysicsEngine.h"
struct PhysXData
{
	physx::PxPvd* Debugger;
	physx::PxPvdTransport* Transport;
};

static PhysXData* s_Data = nullptr;
namespace Proof
{

	void PhysicsDebugger::Initialize()
	{
		s_Data = new PhysXData();

		s_Data->Debugger = physx::PxCreatePvd(*PhysicsEngine::GetFoundation());
		PF_CORE_ASSERT(s_Data->Debugger, "PxCreatePvd failed");
	}

	void PhysicsDebugger::Shutdown()
	{
		PhysicsDebugger::StopDebugging();
		s_Data->Debugger->release();
		// TODO adelete
		delete s_Data;
		s_Data = nullptr;
	}
	void PhysicsDebugger::StartDebugging(const std::string& filepath, bool networkDebugging)
	{
		StopDebugging();

		if (!networkDebugging)
		{
			s_Data->Transport = physx::PxDefaultPvdFileTransportCreate((filepath + ".pxd2").c_str());
			s_Data->Debugger->connect(*s_Data->Transport, physx::PxPvdInstrumentationFlag::eALL);
		}
		else
		{
			s_Data->Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 1000);
			s_Data->Debugger->connect(*s_Data->Transport, physx::PxPvdInstrumentationFlag::eALL);
		}
	}

	bool PhysicsDebugger::IsDebugging()
	{
		return s_Data->Debugger->isConnected();
	}

	void PhysicsDebugger::StopDebugging()
	{
		if (!s_Data->Debugger->isConnected())
			return;

		s_Data->Debugger->disconnect();
		s_Data->Debugger->release();
		s_Data->Transport->release();
	}

	physx::PxPvd* PhysicsDebugger::GetDebugger()
	{
		return s_Data->Debugger;
	}


}
