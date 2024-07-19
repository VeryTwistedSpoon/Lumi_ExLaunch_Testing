#pragma once

#include "externals/il2cpp-api.h"

#include "externals/Pml/PokePara/CoreParam.h"
#include "externals/Pml/PokePara/InitialSpec.h"

namespace Pml::PokePara {
    struct PokemonParam : ILClass<PokemonParam, 0x04c59c10> {
        struct Fields : public CoreParam::Fields {
            //TODO
        };

        inline void ctor(int32_t monsno, uint16_t level, uint64_t id) {
            external<void>(0x02054fe0, this, monsno, level, id);
        }

        inline void ctor(Pml::PokePara::InitialSpec::Object* spec) {
            external<void>(0x02055140, this, spec);
        }
    };
}

namespace System::Collections::Generic {
    struct List$$PokemonParam : List<List$$PokemonParam, Pml::PokePara::PokemonParam> {
        static inline StaticILMethod<0x04c8a080, Pml::PokePara::PokemonParam> Method$$Add {};
    };
}
