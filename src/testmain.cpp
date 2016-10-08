#include <characterdescription.h>
#include <abilityscores.h>
#include <hitpoints.h>

int main()
{
    //TODO: Need to account for favored class bonus choices (+1 skill, +1hp)
    RulesEngine::Character::CharacterDescription characterDescription;

    RulesEngine::Character::AbilityScores abilityScores(characterDescription);
    abilityScores.setBaseAbilityScore(RulesEngine::Character::AbilityScoreTypes::CON, 15); //+2 modifier

    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);
    hitPoints.setUsePFSStyleFixedHPCalculation(true);

    characterDescription.addClass("Fighter", 6, 10, 2, 1);

    abilityScores.setBaseAbilityScore(RulesEngine::Character::AbilityScoreTypes::CON, 7); //-2 modifier

    characterDescription.addTemporaryNegativeLevelDebuff("tempSource", 1, "tempDesc");
    characterDescription.addPermanentNegativeLevelDebuff("permSource", 2, "permDesc");
    characterDescription.addClass("Wizard", 2, 6, 4, .5);

    characterDescription.removeClass("Wizard");

    characterDescription.setClassLevel("Fighter", 1);
    characterDescription.setClassLevel("Fighter", 8);
    characterDescription.setClassLevel("Fighter", 8);
}
