#include "exlaunch.hpp"

#include "externals/BattleCharacterEntity.h"
#include "externals/ColorVariation.h"
#include "externals/Dpr/Battle/Logic/TRAINER_DATA.h"
#include "externals/Dpr/Battle/View/TrainerSimpleParam.h"
#include "externals/FieldCharacterEntity.h"
#include "externals/UnityEngine/Component.h"
#include "externals/UnityEngine/GameObject.h"
#include "externals/UnityEngine/MaterialPropertyBlock.h"
#include "save/save.h"
#include "romdata/data/ColorSet.h"
#include "romdata/romdata.h"

#include "logger/logger.h"

RomData::ColorSet GetCustomColorSet()
{
    RomData::ColorSet set = {
        .fSkinFace =     getCustomSaveData()->colorVariations.fSkinFace[0],
        .fSkinMouth =    getCustomSaveData()->colorVariations.fSkinMouth[0],
        .fEyes =         getCustomSaveData()->colorVariations.fEyes[0],
        .fEyebrows =     getCustomSaveData()->colorVariations.fEyebrows[0],
        .fSkinBody =     getCustomSaveData()->colorVariations.fSkinBody[0],
        .fHair =         getCustomSaveData()->colorVariations.fHair[0],

        .bSkinFace =     getCustomSaveData()->colorVariations.bSkinFace[0],
        .bHairExtra =    getCustomSaveData()->colorVariations.bHairExtra[0],
        .bEyeLeft =      getCustomSaveData()->colorVariations.bEyeLeft[0],
        .bEyeRight =     getCustomSaveData()->colorVariations.bEyeRight[0],
        .bSkinBody =     getCustomSaveData()->colorVariations.bSkinBody[0],
        .bHair =         getCustomSaveData()->colorVariations.bHair[0],
    };

    return set;
}

ColorVariation::Property::Array* GetEditedProperty00(ColorVariation::Object* variation, int32_t index)
{
    system_load_typeinfo(0x2c09);
    system_load_typeinfo(0x9c60);

    auto component = variation->cast<UnityEngine::Component>();
    auto gameObject = component->get_gameObject()->instance();

    ColorVariation::Property::Array* properties = variation->fields.Property00;

    for (uint64_t i=0; i<properties->max_length; i++)
    {
        ColorVariation::Property::MaskColor::Array* colors = properties->m_Items[i].fields.colors;

        RomData::ColorSet set = {};
        if (index == -1)
            set = GetCustomColorSet();
        else
            set = GetColorSet(index);

        if (gameObject->GetComponent(UnityEngine::Component::Method$$BattleCharacterEntity$$GetComponent) != nullptr)
        {
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_SKIN_FACE)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_SKIN_FACE].fields.color.fields = set.bSkinFace.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_HAIR_EXTRA)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_HAIR_EXTRA].fields.color.fields = set.bHairExtra.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_EYE_LEFT)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_EYE_LEFT].fields.color.fields = set.bEyeLeft.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_EYE_RIGHT)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_EYE_RIGHT].fields.color.fields = set.bEyeRight.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_SKIN_BODY)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_SKIN_BODY].fields.color.fields = set.bSkinBody.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::B_HAIR)
                colors->m_Items[(int32_t)RomData::ColorSetID::B_HAIR].fields.color.fields = set.bHair.fields;
        }

        if (gameObject->GetComponent(UnityEngine::Component::Method$$FieldCharacterEntity$$GetComponent) != nullptr)
        {
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_FACE)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_FACE].fields.color.fields = set.fSkinFace.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_MOUTH)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_MOUTH].fields.color.fields = set.fSkinMouth.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_EYES)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_EYES].fields.color.fields = set.fEyes.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_EYEBROWS)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_EYEBROWS].fields.color.fields = set.fEyebrows.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_SKIN_BODY)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_SKIN_BODY].fields.color.fields = set.fSkinBody.fields;
            if (colors->max_length > (int32_t)RomData::ColorSetID::F_HAIR)
                colors->m_Items[(int32_t)RomData::ColorSetID::F_HAIR].fields.color.fields = set.fHair.fields;
        }
    }

    return properties;
}

HOOK_DEFINE_REPLACE(ColorVariation_LateUpdate) {
    static void Callback(ColorVariation::Object* __this) {
        system_load_typeinfo(0x2c09);
        ColorVariation::Property::Array* properties = GetEditedProperty00(__this, __this->fields.ColorIndex);

        if (__this->fields.propertyBlock != nullptr)
        {
            for (uint64_t i=0; i<properties->max_length; i++)
            {
                properties->m_Items[i].Update(__this->fields.propertyBlock);
            }
        }
    }
};

HOOK_DEFINE_REPLACE(GetColorID) {
    static int32_t Callback() {
        return getCustomSaveData()->colorVariations.playerColorID[0];
    }
};

HOOK_DEFINE_REPLACE(SetColorID) {
    static void Callback(int32_t value) {
        getCustomSaveData()->colorVariations.playerColorID[0] = value;
    }
};

HOOK_DEFINE_INLINE(SetColorID_Inline) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto colorId = (int32_t)ctx->W[20];
        getCustomSaveData()->colorVariations.playerColorID[0] = colorId;
    }
};

HOOK_DEFINE_INLINE(SetColorID_TrainerParam_StoreCore) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerData = (Dpr::Battle::Logic::TRAINER_DATA::Object*)ctx->X[1];
        trainerData->fields.colorID = getCustomSaveData()->colorVariations.playerColorID[0];
        trainerData->fields.trainerID = 0;

        ctx->X[1] = (uint64_t)trainerData;
    }
};

HOOK_DEFINE_INLINE(CardModelViewController_LoadModels) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        auto trainerParam = (Dpr::Battle::View::TrainerSimpleParam::Object*)ctx->X[1];
        auto isContest = (bool)ctx->W[2];
        auto battleCharacterEntity = (BattleCharacterEntity*)ctx->X[20];

        int32_t colorID = getCustomSaveData()->colorVariations.playerColorID[0];
        trainerParam->fields.colorID = colorID;

        battleCharacterEntity->Initialize(trainerParam, isContest);
    }
};

void exl_color_variations_main() {
    ColorVariation_LateUpdate::InstallAtOffset(0x018ecd90);

    GetColorID::InstallAtOffset(0x0203d3f0);
    GetColorID::InstallAtOffset(0x02cef820);

    SetColorID::InstallAtOffset(0x02cef870);
    SetColorID_Inline::InstallAtOffset(0x02cf3c7c);

    SetColorID_TrainerParam_StoreCore::InstallAtOffset(0x020387c4);

    CardModelViewController_LoadModels::InstallAtOffset(0x01a315a4);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x020388ac);
    p.WriteInst(Nop());
}