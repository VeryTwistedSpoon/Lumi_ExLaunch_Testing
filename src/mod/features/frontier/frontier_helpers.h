#pragma once

#include "frontierIndex.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/Pml/PmlUse.h"

namespace Frontier {
    int randAbility() {
        int randNo = 1 + rand() % 2;
        return randNo;
    }

    Pml::PokePara::PokemonParam::Object* GeneratePokemon(frontierIndex pokeInfo) {
        auto initialSpec = Pml::PokePara::InitialSpec::newInstance();
        initialSpec->fields.monsno = pokeInfo.monsNo;
        initialSpec->fields.formno = pokeInfo.formNo;
        initialSpec->fields.level = 50;
        initialSpec->fields.sex = 0;

        auto pokeParam = Pml::PokePara::PokemonParam::newInstance(initialSpec);
        auto pmlUse = Pml::PmlUse::get_Instance();
        int32_t lang = pmlUse->get_LangId();

        pokeParam->fields.m_accessor->SetLangID(lang);
        pokeParam->fields.m_accessor->SetOwnedOthersFlag(false);
        pokeParam->fields.m_accessor->SetSeikaku(pokeInfo.seikaku);

        switch (randAbility()) {
            case (1): {
                pokeParam->fields.m_accessor->SetTokusei1Flag(true);
            }
            case (2): {
                pokeParam->fields.m_accessor->SetTokusei2Flag(true);
            }
        }

        auto core = pokeParam->cast<Pml::PokePara::CoreParam>();

        for (int i = 0; i < 4; i++) {
            core->SetWaza(i, pokeInfo.moves[i]);
        }

        for (int i = 0; i < 6; i++) {
            core->ChangeEffortPower(i, pokeInfo.effort[i]);
            core->ChangeTalentPower(i, 0);
        }

        core->SetItem(pokeInfo.item);

        return pokeParam;
    }

}
