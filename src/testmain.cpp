#include <characterdescription.h>
#include <abilityscores.h>
#include <hitpoints.h>

int main()
{
    RulesEngine::Character::CharacterDescription characterDescription;
    characterDescription.addClass("Fighter", 6, 10, 2, 1);
    characterDescription.addClass("Wizard", 2, 6, 4, .5);
    characterDescription.addClass("Rogue", 3, 8, 8, .75);
    RulesEngine::Character::AbilityScores abilityScores(characterDescription);
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);
    hitPoints.generateHitPoints();
    auto blah1 = hitPoints.getMaxHitPoints();
    characterDescription.removeClass("Wizard");
    hitPoints.generateHitPoints();
    auto blah2 = hitPoints.getMaxHitPoints();
    characterDescription.setClassLevel("Fighter", 1);
    auto blah3 = hitPoints.getMaxHitPoints();
    //const std::string& className,
    //unsigned int classLevel,
    //unsigned int hitDieSize,
    //unsigned int skillPointsPerLevel,
    //float baseAttackBonusProgression);
}
