#include "exlaunch.hpp"
#include "save/save.h"
#include "logger/logger.h"
#include "externals/SmartPoint/Components/PlayerPrefsProvider_ViewerSettings_.h"
#include "externals/Dpr/UI/BoxListPanel.h"

/* Hooks */
HOOK_DEFINE_REPLACE(GetOpenTrayMax) {
    static int32_t Callback() {
        return BoxCount;
    }
};

HOOK_DEFINE_REPLACE(UpdateTrayMax) {
    static int32_t Callback(Pml::PokePara::PokemonParam::Object* tmp_pp) {
        return BoxCount;
    }
};

HOOK_DEFINE_REPLACE(GetTrayMax) {
    static int32_t Callback() {
        return BoxCount;
    }
};

HOOK_DEFINE_REPLACE(SetTrayMax) {
    static void Callback(int32_t max) {
        system_load_typeinfo(0x26d3);
        PlayerWork::getClass()->initIfNeeded();
        auto boxData = PlayerWork::GetBoxData();
        boxData->fields.trayMax = (uint8_t) max;
    }
};

HOOK_DEFINE_TRAMPOLINE(BoxListPanel$$ctor) {
    static void Callback(Dpr::UI::BoxListPanel::Object* __this) {
        Orig(__this);
        __this->fields._boxItemScales->m_Items[4] = 0.5f;
    }
};

/* Assembly Patches */
using namespace exl::armv8::inst;
using namespace exl::armv8::reg;

void Dpr_Box_BoxPokemonWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01d30ba4, CmpImmediate(W24, BoxCount - 1)}, // $$MovePokemon
            {0x01d30bac, CmpImmediate(W22, BoxCount - 1)}, // $$MovePokemon
            {0x01d30da0, CmpImmediate(W21, BoxCount - 1)}, // $$UpdatePokemon
            {0x01d309ec, CmpImmediate(W19, BoxCount - 1)}, // $$MovePokemon
            {0x01d309f4, CmpImmediate(W20, BoxCount)}, // $$SwapTray
            {0x01d31460, CmpImmediate(W20, BoxCount)}, // $$GetSpaceCountAll
            {0x01d31468, Movz(W8, BoxCount * 30)}, // $$GetSpaceCountAll
            {0x01d2fd90, CmpImmediate(W21, BoxCount - 1)}, // $$GetPokemon
            {0x01d308f0, CmpImmediate(W22, BoxCount - 1)}, // $$SwapPokemon
            {0x01d30900, CmpImmediate(W21, BoxCount)}, // $$SwapPokemon
            {0x01d31754, CmpImmediate(W22, BoxCount)}, // $$GetSpacePos
            {0x01d3170c, CmpImmediate(W22, BoxCount - 1)}, // $$GetSpacePos
            {0x01d31058, CmpImmediate(W21, BoxCount)}, // $$GetPokemonCountAll
            {0x01d3106c, CmpImmediate(W21, BoxCount)}, // $$GetPokemonCountAll
            {0x01d30da0, CmpImmediate(W21, BoxCount - 1)}, // $$ClearPokemon

    };
    p.WriteInst(inst);
}

void Dpr_Box_BoxWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01d32370, CmpImmediate(W19, BoxCount)}, // $$GetTrayName
            {0x01d33f20, CmpImmediate(W19, BoxCount)}, // $$GetWallPaper
            {0x01d33e78, CmpImmediate(W20, BoxCount)}, // $$SetWallPaper
            {0x01d32438, CmpImmediate(W0, BoxCount - 1)}, // $$ChangePokemon
            {0x01d32448, CmpImmediate(W2, BoxCount)}, // $$ChangePokemon
            {0x01d322b0, CmpImmediate(W1, BoxCount)}, // $$SetTrayName
            {0x01d320d0, CmpImmediate(W0, BoxCount - 1)}, // $$ChangeTeam
            {0x01d320dc, CmpImmediate(W1, BoxCount - 1)}, // $$ChangeTeam
            {0x01d33064, CmpImmediate(W0, BoxCount - 1)}, // $$DeleteTeam
            {0x01d32c60, CmpImmediate(W0, BoxCount - 1)}, // $$GetPokeTeamPos
            {0x01d32b00, CmpImmediate(W0, BoxCount - 1)}, // $$IsPokeTeam
            {0x01d32988, CmpImmediate(W0, BoxCount - 1)}, // $$IsTeam
            {0x01d32a10, CmpImmediate(W0, BoxCount - 1)}, // $$IsTeamPos
            {0x01d33b20, CmpImmediate(W2, BoxCount - 1)}, // $$SetTeamPokePos

    };
    p.WriteInst(inst);
}

void Dpr_Box_SaveBoxData_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            { 0x01d34238, Movz(X1, BoxCount)}, // $$Clear
            { 0x01d3438c, Movz(X1, BoxCount)}, // $$Clear

    };
    p.WriteInst(inst);
}

void Dpr_UI_BoxListPanel_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x01aaff64, CmpImmediate(W21, BoxCount)}, // $$Initialize

    };
    p.WriteInst(inst);
}

void PlayerWork_ASM(exl::patch::CodePatcher p) {
    auto inst = nn::vector<exl::patch::Instruction> {
            {0x02ceb170, Movz(X1, BoxCount)}, // $$Initialization

    };
    p.WriteInst(inst);
}

void exl_save_box_expansion_main() {
    exl::patch::CodePatcher p(0);

    /* Write Assembly Patches */
    Dpr_Box_BoxPokemonWork_ASM(p);
    Dpr_Box_BoxWork_ASM(p);
    Dpr_Box_SaveBoxData_ASM(p);
    Dpr_UI_BoxListPanel_ASM(p);
    PlayerWork_ASM(p);

    /* Install Hooks */
    UpdateTrayMax::InstallAtOffset(0x01d317f0);
    GetOpenTrayMax::InstallAtOffset(0x01d30190);
    GetTrayMax::InstallAtOffset(0x01d30610);
    SetTrayMax::InstallAtOffset(0x01d31a60);
    BoxListPanel$$ctor::InstallAtOffset(0x01ab0630);
}