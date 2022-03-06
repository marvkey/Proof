#include "Proofprch.h"
#include "VulkanShader.h"
#include <fstream>
namespace Proof
{
    std::vector<char> VulkanShader::ReadFile(const std::string& filePath) {
        // ate = when open go to end imediately, helps us get size more convieneinet
        // binary  =
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (file.is_open() == false)
            PF_ASSERT(false, "failed to open file, %s", filePath);
        // cause of at we are at last position so tellg will give us size
        size_t fileSize = size_t(file.tellg()); 
        std::vector<char>  buffer(fileSize);
        file.seekg(0); // seek go to start of file so we can read the data
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
}
