#pragma once
#include "externals/il2cpp-api.h"
#include "externals/UnityEngine/MonoBehaviour.h"

namespace Dpr::UI {
    struct UINavigator : ILClass<UINavigator> {
        struct Fields : UnityEngine::MonoBehaviour::Fields {
            UINavigator::Object* _left;
            UINavigator::Object* _right;
            UINavigator::Object* _top;
            UINavigator::Object* _bottom;
            bool _isStopLeft;
            bool _isStopRight;
            bool _isStopTop;
            bool _isStopBottom;
            UINavigator::Array* _navigates;
            System::Boolean_array* _isStops;
            Il2CppObject* userParam;
            bool isInitialized;
        };

        inline bool isStop(int32_t type) {
            return external<bool>(0x01a14700, this, type);
        }
    };
}