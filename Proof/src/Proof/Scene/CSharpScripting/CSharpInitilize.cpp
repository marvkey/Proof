#include "Proofprch.h"
#include "CSharpInitilize.h"

namespace Proof
{
	InitlizeCSharp::InitlizeCSharp() {
		
		// set the current directory of the mono to this
		mono_set_dirs("C:\\Program Files\\Mono\\lib","C:\\Program Files\\Mono\\lib");// vry temprorayy 
		m_Domain = mono_jit_init("ProofCSHARP");
		if(m_Domain){
			m_Assembly =mono_domain_assembly_open(m_Domain,"ProofGame.dll"); // links to startup of project file lets go
			if(m_Assembly){
				m_Image=mono_assembly_get_image(m_Assembly);
				if(m_Image){
					MonoClass* Script = mono_class_from_name(m_Image,"Movement","Script");
				}
			};
		}
		
	}

	InitlizeCSharp::~InitlizeCSharp() {
		if(m_Domain)
			mono_jit_cleanup(m_Domain);
	}

	void InitlizeCSharp::OnUpdate(float deltaTime) {}
}