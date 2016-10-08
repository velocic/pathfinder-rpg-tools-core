#include <characterdescription.h>
#include <abilityscores.h>
#include <hitpoints.h>

int main()
{
    RulesEngine::Character::CharacterDescription characterDescription;

    RulesEngine::Character::AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(RulesEngine::Character::AbilityScoreTypes::CON, 15); //+2 modifier
    auto debug1 = abilityScores.getTotalAbilityModifier(RulesEngine::Character::AbilityScoreTypes::CON);

    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);

    characterDescription.addClass("Fighter", 6, 10, 2, 1);

    abilityScores.setBaseAbilityScore(RulesEngine::Character::AbilityScoreTypes::CON, 7); //-2 modifier
    auto debug2 = abilityScores.getTotalAbilityModifier(RulesEngine::Character::AbilityScoreTypes::CON);

    // characterDescription.addTemporaryNegativeLevelDebuff("tempSource", 1, "tempDesc");
    // characterDescription.addPermanentNegativeLevelDebuff("permSource", 2, "permDesc");
    characterDescription.addClass("Wizard", 2, 6, 4, .5);
    // characterDescription.addClass("Rogue", 3, 8, 8, .75);

    characterDescription.removeClass("Wizard");

    // characterDescription.setClassLevel("Fighter", 1);
    // characterDescription.setClassLevel("Fighter", 8);
    // characterDescription.setClassLevel("Fighter", 8);
    //const std::string& className,
    //unsigned int classLevel,
    //unsigned int hitDieSize,
    //unsigned int skillPointsPerLevel,
    //float baseAttackBonusProgression);
}
