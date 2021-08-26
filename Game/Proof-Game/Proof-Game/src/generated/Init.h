#pragma once
#ifdef  PROOFGAMELIB
#define ProofGAMEAPI __declspec(dllexport)
#else
#define ProofGAMEAPI __declspec(dllimport)
#endif
#include <Proof/Input/Input.h>

ProofGAMEAPI void INIT();
