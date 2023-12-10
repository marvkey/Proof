#pragma once
namespace Proof::UI
{
	enum class UIMemoryAssetTypes
	{
		None = 0,
		All,
		Default,
	};

    struct UIMessageBoxData
    {
        std::string Title = "";
        std::string Body = "";
        uint32_t Flags = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
        std::function<void()> UserRenderFunction;

        bool ShouldOpen = true;
        bool IsOpen = false;
    };

    enum UIMessageBoxBit
    {
        OkButton = BIT(0),
        CancelButton = BIT(1),
        UserFunc = BIT(2)
    };
}