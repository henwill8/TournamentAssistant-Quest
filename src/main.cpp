#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
using namespace il2cpp_utils;

void log(std::string str) {
    Logger::get().info(str);
}//

class CustomButton {
    public:
        Il2CppObject* gameObject = nullptr;
        Il2CppObject* TMP = nullptr;
        Il2CppObject* button = nullptr;
        Il2CppObject* buttonTransform = nullptr;
        Il2CppObject* parent = nullptr;
        Il2CppObject* parentTransform = nullptr;
        Il2CppObject* TMPLocalizer = nullptr;
        Il2CppObject* rectTransform = nullptr;
        Vector3 sizeDelta = {0, 0, 0};
        Vector3 scale = {1.0f, 1.0f, 1.0f};
        Vector3 rotation = {0, 0, 0};
        float fontSize = 10.0f;
        bool toggle = true;//Use this when creating toggles
        std::string text = "Custom Button UI";
        function_ptr_t<void> onPress;
 
        bool isCreated = false;
 
        void setParentAndTransform(Il2CppObject* Obj, int parentedAmount) {
            parent = Obj;
 
            if(parentedAmount < 1) {
                log("Parented amount has to be greater than 0, setting to 1...");
                parentedAmount = 1;
            }
            Il2CppObject* transform = *RunMethod(Obj, "get_transform");
            std::vector<Il2CppObject*> parents;
            Il2CppObject* firstParent = *RunMethod(transform, "GetParent");
            parents.push_back(firstParent);
            for(int i = 1; i < parentedAmount; i++) {
                Il2CppObject* otherParent = *RunMethod(parents[i-1], "GetParent");
                parents.push_back(otherParent);
            }
            parentTransform = parents[parents.size()-1];
        }
 
        void setParentTransform(Il2CppObject* Obj, int parentedAmount) {
            if(parentedAmount < 1) {
                log("Parented amount has to be greater than 0, setting to 1...");
                parentedAmount = 1;
            }
            Il2CppObject* transform = *RunMethod(Obj, "get_transform");
            std::vector<Il2CppObject*> parents;
            Il2CppObject* firstParent = *RunMethod(transform, "GetParent");
            parents.push_back(firstParent);
            for(int i = 1; i < parentedAmount; i++) {
                Il2CppObject* otherParent = *RunMethod(parents[i-1], "GetParent");
                parents.push_back(otherParent);
            }
            parentTransform = parents[parents.size()-1];
        }
 
        void create() {
            if(!isCreated && parent != nullptr && parentTransform != nullptr) {
                log("Custom Button: Creating an instance of the parent");
                Il2CppObject* button = CRASH_UNLESS(*il2cpp_utils::RunMethod("UnityEngine", "Object", "Instantiate", parent));
                log("Custom Button: Getting the transform");
                Il2CppObject* buttonTransform = CRASH_UNLESS(*il2cpp_utils::RunMethod(button, "get_transform"));
                log("Custom Button: Setting the parent transform");
                CRASH_UNLESS(il2cpp_utils::RunMethod(buttonTransform, "SetParent", parentTransform));
                log("Custom Button: Setting the local scale");
                CRASH_UNLESS(il2cpp_utils::RunMethod(buttonTransform, "set_localScale", scale));
                log("Custom Button: Setting the local position");
                CRASH_UNLESS(il2cpp_utils::RunMethod(buttonTransform, "set_localPosition", sizeDelta));
                log("Custom Button: Setting the euler angles");
                CRASH_UNLESS(il2cpp_utils::RunMethod(buttonTransform, "set_eulerAngles", rotation));
                log("Custom Button: Getting the game object");
                gameObject = CRASH_UNLESS(*il2cpp_utils::RunMethod(button, "get_gameObject"));
                log("Custom Button: Getting the TMProUGUI");
                TMP = CRASH_UNLESS(*il2cpp_utils::RunMethod(gameObject, "GetComponentInChildren", il2cpp_utils::GetSystemType("TMPro", "TextMeshProUGUI")));
                log("Custom Button: Getting the TMP Localizer");
                TMPLocalizer = *RunMethod<Il2CppObject*>(gameObject, "GetComponentInChildren", GetSystemType("Polyglot", "LocalizedTextMeshProUGUI"));
                log("Custom Button: Getting the rect transform");
                rectTransform = CRASH_UNLESS(*RunMethod(TMP, "get_rectTransform"));
                log("Custom Button: Setting the text");
                CRASH_UNLESS(il2cpp_utils::RunMethod(TMP, "set_text", il2cpp_utils::createcsstr(text)));
                log("Custom Button: Setting the font size");
                CRASH_UNLESS(il2cpp_utils::RunMethod(TMP, "set_fontSize", fontSize));
                log("Custom Button: Getting the onClick property");
                Il2CppObject* onClick = CRASH_UNLESS(*il2cpp_utils::GetPropertyValue(button, "onClick"));
                log("Custom Button: Creating the action");
                auto actionToRun = il2cpp_utils::MakeAction(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine.Events", "UnityAction")), (Il2CppObject*)nullptr, onPress);
                log("Custom Button: Setting the action to onClick");
                CRASH_UNLESS(il2cpp_utils::RunMethod(onClick, "AddListener", actionToRun));
                isCreated = true;
            } else {
                log("Button was already created or parent/parentTransform was null");
            }
        }
 
        bool setText(std::string newText) {
            if(gameObject != nullptr) {
                RET_0_UNLESS(RunMethod(TMP, "SetText", createcsstr(newText)));
                return true;
            }
            log("Game object is null, not setting text");
            return false;
        }
 
        bool setActive(bool isActive) {
            if(gameObject != nullptr) {
                RET_0_UNLESS(RunMethod(gameObject, "SetActive", isActive));
                return true;
            }
            log("Game object is null, not setting active");
            return false;
        }
 
        void setPos(Vector3 pos) {//Doesnt work yet, work on it more later
            il2cpp_utils::RunMethod(buttonTransform, "set_localPosition", pos);
        }
 
        void destroy() {
            if(gameObject != nullptr) {
                RunMethod("UnityEngine", "Object", "Destroy", gameObject);
                gameObject = nullptr;
                TMP = nullptr;
                parentTransform = nullptr;
                parent = nullptr;
                TMPLocalizer = nullptr;
                isCreated = false;
            } else {
                log("Button was already destroyed");
            }
        }
};

extern "C" void setup(ModInfo& info) {
    info.id = "Replay";
    info.version = "0.1.2";
    modInfo = info;
    // Create logger
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    Logger::get().info("Completed setup!");
    // We can even check information specific to the modloader!
    Logger::get().info("Modloader name: %s", Modloader::getInfo().name.c_str());
}

extern "C" void load() {
    Logger::get().info("Installing hooks...");



    Logger::get().info("Installed all hooks!");
    il2cpp_functions::Init();
}
