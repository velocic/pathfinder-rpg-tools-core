#include <characterdescription.h>
#include <abilityscores.h>
#include <hitpoints.h>

int main()
{
    RulesEngine::Character::CharacterDescription characterDescription;
    RulesEngine::Character::AbilityScores abilityScores(characterDescription);
    RulesEngine::Character::HitPoints hitPoints(characterDescription, abilityScores);
    characterDescription.addClass("Fighter", 6, 10, 2, 1);
    characterDescription.addClass("Wizard", 2, 6, 4, .5);
    characterDescription.addClass("Rogue", 3, 8, 8, .75);
    characterDescription.removeClass("Wizard");
    characterDescription.setClassLevel("Fighter", 1);
    characterDescription.setClassLevel("Fighter", 8);
    characterDescription.setClassLevel("Fighter", 8);
    //const std::string& className,
    //unsigned int classLevel,
    //unsigned int hitDieSize,
    //unsigned int skillPointsPerLevel,
    //float baseAttackBonusProgression);
}
