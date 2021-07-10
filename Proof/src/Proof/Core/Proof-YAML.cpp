#include <Proofprch.h>
#include "Proof-YAML.h"
namespace Proof
{
    std::fstream Proof_YAML::FileToWrite;
    std::string Proof_YAML::s_CurrentType = "null";
    std::string Proof_YAML::s_PrevCurrentType;
    bool Proof_YAML::s_OneLine = false;
    uint32_t Proof_YAML::s_NumberofContext;
    uint32_t Proof_YAML::s_NumberofIndent = 0;
    void Proof_YAML::BeginContext(const std::string& FilePath) {
        FileToWrite.open(FilePath,std::ios::out | std::ios::trunc |std::ios::in);
    }
    
    template<>
    void Proof_YAML::WriteVariables(const std::string& VariableName, const bool& Variable){
        if(s_OneLine== true){
                return;//Assertion
        }
        FileToWrite<<"\n";
          if(s_CurrentType == "Dictonary"){
            for(int i=0; i<s_NumberofIndent+2; i++){
                FileToWrite<<" ";
            }
            goto a;
        }
         for(int i=0; i<s_NumberofIndent; i++){
                FileToWrite<<" ";
        }
        a:
        if(s_CurrentType == "Dictonary"){
              if(Variable == false)
                FileToWrite<<VariableName<<": False";
            else
                FileToWrite<<VariableName<<": True";
        }
    }
    
    

    template<>
    void Proof_YAML::WriteList(const bool& Variable) {
        if(s_OneLine == false){
         FileToWrite<<"\n";
        }
        for(int i=0; i<s_NumberofIndent; i++){
            FileToWrite<<" ";
        }
        if(Variable == false)
               FileToWrite<<Variable<<"- False";
           else
                FileToWrite<<Variable<<"- True";
          return;
        //Assertion;
    }
    
    void Proof_YAML::StartList(const std::string& ID) {
        s_PrevCurrentType = s_CurrentType;
        s_CurrentType = "List";
        FileToWrite << "\n";
        if(s_PrevCurrentType == "Dictonary" || s_CurrentType =="Dictonary"){
        
            for (int i = 0; i < s_NumberofIndent + 3; i++) {
                FileToWrite << " ";
            }
        }else{
            for (int i = 0; i < s_NumberofIndent; i++) {
                FileToWrite << " ";
            }
        }
        FileToWrite << ID << ": ";
        s_NumberofIndent++;
    }

    void Proof_YAML::StartDictonary(const std::string& ID) {
        s_PrevCurrentType = s_CurrentType;
        s_CurrentType = "Dictonary";
        FileToWrite << "\n";
        if (s_PrevCurrentType == "List" || s_PrevCurrentType == "Dictonary") {
            for (int i = 0; i < s_NumberofIndent + 2; i++) {
                FileToWrite << " ";
            }
        }
        else {
            for (int i = 0; i < s_NumberofIndent+2; i++) {
                FileToWrite << " ";
            }
        }
        FileToWrite << ID << ": ";
        s_NumberofIndent++;
    }

    void Proof_YAML::EndList() {
        s_NumberofIndent--;
        if (s_NumberofIndent == 0) {
            s_CurrentType = "null";
        }
    }

    void Proof_YAML::EndDictonary() {

        s_NumberofIndent--;

        /*
        if (s_NumberofIndent == 0) {
            s_CurrentType = "null";
        }
        */
    }

    void Proof_YAML::BeginSingleLine() {
        s_OneLine = true;
        FileToWrite << "[";
    }

    void Proof_YAML::EndSingleLine() {
        FileToWrite << "]";
        if (s_OneLine == false)
            //Assertion
            return;
        s_OneLine = false;
    }

    void Proof_YAML::EndContext() {
        FileToWrite.close();
    }
}