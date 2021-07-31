#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
namespace Proof{

	class Proof_API Event {
	protected:
	public:
		virtual std::string ToString()const { return "NULL"; };
		friend std::ostream& operator<<(std::ostream& os,const Event& e);
	};
	


}

