#include "STDInc.hpp"
#include <imgui_markdown.h>

namespace Components
{
    void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
    {
        std::string url(data_.link, data_.linkLength);
        if (!data_.isImage)
        {
            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOW);
        }
    }

    void Markdown::MarkdownInit(const std::string& markdown_)
    {
        ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, /*ImageCallback*/ NULL, NULL, {{ImGui_Fonts::H1, true}, {ImGui_Fonts::H2, true}, {ImGui_Fonts::H3, false}}, NULL};
        ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
    }
}