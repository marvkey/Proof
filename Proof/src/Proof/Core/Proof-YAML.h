#include <iostream>
#include <fstream>
#include "Core.h"
namespace Proof{
    class Proof_API Proof_YAML {
        static std::fstream FileToWrite;
        static std::string s_CurrentType;
        static std::string s_PrevCurrentType;
        static bool s_OneLine;
        static uint32_t s_NumberofContext;
        static uint32_t s_NumberofIndent;
    public:
        virtual ~Proof_YAML() =default;
        static void BeginContext(const std::string& FilePath);
        template<typename T>
        static void WriteVariables(const std::string& VariableName,const T& Variable) {
            if (s_OneLine == true) {
                return;//Assertion
            }
            FileToWrite << "\n";
            if (s_CurrentType == "Dictonary") {
                for (int i = 0; i < s_NumberofIndent + 2; i++) {
                    FileToWrite << " ";
                }
                goto a;
            }
            for (int i = 0; i < s_NumberofIndent; i++) {
                FileToWrite << " ";
            }
            a:
            if (s_CurrentType == "Dictonary" || s_CurrentType == "null") {
                FileToWrite << VariableName << ": " << Variable;
            }
        }

        static void WriteString(const std::string& Element) {
            FileToWrite << " \n";
            if (s_CurrentType == "Dictonary") {
                for (int i = 0; i < s_NumberofIndent + 2; i++) {
                    FileToWrite << " ";
                }
                goto a;
            }
            for (int i = 0; i < s_NumberofIndent; i++) {
                FileToWrite << " ";
            }
            a:
            if (s_CurrentType == "Dictonary" || s_CurrentType == "null") {
                FileToWrite << Element << ": ";
            }
        }
        template<typename T>
        static void WriteList(const T& Variable) {
            if (s_OneLine == true) {
                FileToWrite << Variable << ",";
                return;
            }
            else
                FileToWrite << "\n";

            for (int i = 0; i < s_NumberofIndent; i++) {
                FileToWrite << " ";
            }

            FileToWrite << "- " << Variable << ": ";
            return;
        }

        static void StartList(const std::string& ID);
        static void StartDictonary(const std::string& ID);
        static void EndList();
        static void EndDictonary();
        static void BeginSingleLine();
        static void EndSingleLine();
        static void EndContext();
    };
}