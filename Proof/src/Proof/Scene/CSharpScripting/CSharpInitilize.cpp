#include "Proofprch.h"
#include "CSharpInitilize.h"

namespace Proof
{
	InitlizeCSharp::InitlizeCSharp() {
		/*
		// set the current directory of the mono to this
		mono_set_dirs(".",". ");
		m_Domain = mono_jit_init("ProofCSHARP");
		if(m_Domain){

			m_Assembly =mono_domain_assembly_open(m_Domain,"ProofGame");
		}
		*/
	}

	InitlizeCSharp::~InitlizeCSharp() {
		if(m_Domain)
			mono_jit_cleanup(m_Domain);
	}

	void InitlizeCSharp::OnUpdate(float deltaTime) {}
}