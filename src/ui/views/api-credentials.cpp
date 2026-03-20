#include "ui.h"
#include "imgui.h"
#include "fonts.h"

void ui::views::api_credentials(
    dba::DBAState& dba_state,
    fonts::Fonts& fonts,
    vm::api_credentials::ApiCredentialsViewModel& api_credentials_vm
)
{
    static const char* endpoint_labels[] = {
        "OpenAI",
        "OpenRouter",
        "Groq",
        "Ollama (local)",
        "Custom",
    };
    static const char* endpoint_urls[] = {
        "https://api.openai.com/v1/chat/completions",
        "https://openrouter.ai/api/v1/chat/completions",
        "https://api.groq.com/openai/v1/chat/completions",
        "http://localhost:11434/v1/chat/completions",
    };
    static const int preset_count = 4;
    static const int custom_idx = 4;

    ImGui::Text("API Endpoint");
    ui::components::select(
        "##endpoint",
        &api_credentials_vm.endpoint_selection,
        endpoint_labels,
        IM_ARRAYSIZE(endpoint_labels)
    );

    if (api_credentials_vm.endpoint_selection == custom_idx) {
        ui::components::input("Custom Endpoint URL", &api_credentials_vm.custom_endpoint);
        ImGui::TextDisabled("Full chat completions URL, e.g. https://your-host.com/v1/chat/completions");
    }

    ui::components::input_error(
        api_credentials_vm.api_endpoint_error,
        fonts
    );

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18, 11));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    ImGui::Text("API Key");

    if (ImGui::InputText(
        "##api_key",
        (char*)api_credentials_vm.api_key.c_str(),
        api_credentials_vm.api_key.capacity() + 1,
        ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CallbackResize,
        [](ImGuiInputTextCallbackData* data) -> int {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                std::string* str = (std::string*)data->UserData;
                str->resize(data->BufTextLen);
                data->Buf = (char*)str->c_str();
            }
            return 0;
        },
        (void*)&api_credentials_vm.api_key
    ))
    {
        api_credentials_vm.saved = false;
    }

    ImGui::PopStyleVar(2);

    ui::components::input_error(
        api_credentials_vm.api_key_error,
        fonts
    );

    if (ui::components::button("Save"))
    {
        if (api_credentials_vm.endpoint_selection < preset_count) {
            api_credentials_vm.api_endpoint = endpoint_urls[api_credentials_vm.endpoint_selection];
        } else {
            api_credentials_vm.api_endpoint = api_credentials_vm.custom_endpoint;
        }

        vm::api_credentials::validate(api_credentials_vm);

        if (
            !api_credentials_vm.api_endpoint_error.has_error
            && !api_credentials_vm.api_key_error.has_error
        )
        {
            dba::upsert_settings(
                dba_state,
                api_credentials_vm.api_endpoint,
                api_credentials_vm.api_key
            );
            api_credentials_vm.saved = true;
        }
    }

    if (api_credentials_vm.saved)
    {
        ImGui::Text("Saved!");
    }
}
