#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>
#include <utility>

enum class ModifierTypes
{
};

struct Modifier
{
    ModifierTypes modiferType;
    std::string sourceName;
    int modifierValue;
    std::string modifierDescription;
};

struct CharacterClass
{
    std::string className;
    unsigned int classLevel;
    unsigned int hitDieSize;
    unsigned int skillPointsPerLevel;
};

struct CharacterDescription
{
    std::string characterName;
    std::string alignment;
    std::string player;
    std::vector<CharacterClass> classLevels;
    std::string deity;
    std::string homeland;
    std::string race;
    char size;
    std::string gender;
    unsigned int age;
    std::string height;
    unsigned int weight;
    std::string hair;
    std::string eyes;
};

enum class AttributeTypes
{
    STR,
    DEX,
    CON,
    INT,
    WIZ,
    CHA
};

struct Attribute
{
    int abilityScore;
    int abilityModifier;
    int tempAdjustment;
    std::vector<Modifier> modifiers;
};

struct Attributes
{
    Attribute strength;
    Attribute dexterity;
    Attribute constitution;
    Attribute intelligence;
    Attribute wisdom;
    Attribute charisma;
};

struct Hitpoints
{
    int total;
    int damageReduction;
    int current;
    int nonlethalDamage;
    std::vector<Modifier> modifiers;
};

struct Speed
{
    int base;
    int withArmor;
    int fly;
    int swim;
    int climb;
    int burrow;
    std::vector<Modifier> modifiers;
};

struct Initiative
{
    int total;
    std::vector<Modifier> modifiers;
};

struct ArmorClass
{
    //Base starts at 10
    int total;
    int touch;
    int flatFooted;
    int armorBonus;
    int shieldBonus;
    int naturalArmor;
    std::vector<Modifier> modifiers;
};

enum class SavingThrowTypes
{
    Fortitude,
    Reflex,
    Will
};

struct SavingThrow
{
    int base;
    std::vector<Modifier> modifiers;
};

struct SavingThrows
{
    SavingThrow fortitude;
    SavingThrow reflex;
    SavingThrow will;
};

struct BaseAttackBonus
{
    int base;
    std::vector<Modifier> modifiers
};

struct SpellResistance
{
    int base;
    std::vector<Modifier> modifiers
};

struct CombatManeuverBonus
{
    int total;
    std::vector<Modifier> modifiers
};

struct CombatManeuverDefense
{
    //Base starts at 10
    int total;
};

struct Weapon
{
    std::string name;
    int attackBonus;
    unsigned int criticalMultiplier;
    char type;
    unsigned int range;
    unsigned int ammunition;
    unsigned int damageDieSize;
    int staticDamage;
    std::string description;
    std::vector<Modifier> modifiers
};

enum class CraftingSkillTypes
{
};

enum class KnowledgeSkillTypes
{
    Arcana,
    Dungeoneering,
    Engineering,
    Geography,
    History,
    Local,
    Nature,
    Nobility,
    Planes,
    Religion
};

enum class PerformSkillTypes
{
};

enum class ProfessionSkillTypes
{
};

struct Skill
{
    bool isClassSkill = false;
    AttributeTypes keyAttribute;
    int totalBonus;
    unsigned int ranks;
    std::vector<Modifier> modifiers
};

struct Skills
{
    Skill acrobatics;
    Skill appraise;
    Skill bluff;
    Skill climb;
    std::vector<std::pair<CraftingSkillTypes, Skill>> craft;
    Skill diplomacy;
    Skill disableDevice; //Trained only
    Skill disguise;
    Skill escapeArtist;
    Skill fly;
    Skill handleAnimal; //Trained only
    Skill heal;
    Skill intimidate;
    std::vector<std::pair<KnowledgeSkillTypes, Skill>> knowledge; //Trained only
    Skill linguistics; //Trained only
    Skill perception;
    std::vector<std::pair<PerformSkillTypes, Skill>> perform;
    std::vector<std::pair<ProfessionSkillTypes, Skill>> profession; //Trained only
    Skill ride;
    Skill senseMotive;
    Skill sleightOfHand; //Trained only
    Skill spellcraft; //Trained only
    Skill stealth;
    Skill survival;
    Skill swim;
    Skill useMagicDevice; //Trained only
};

struct Languages
{
    std::vector<std::string> languagesKnown;
};

enum class ACItemTypes
{
};

struct ACItems
{
    int bonus;
    ACItemType type;
    int checkPenalty;
    double spellFailure;
    unsigned int weight;
    std::vector<Modifier> modifiers;
};

struct InventoryItem
{
    std::string name;
    double weight;
    std::string description;
    unsigned int quantity;
};

//Character inventory, not necessarily equipped items
struct Gear
{
    std::vector<InventoryItem> items;
    double totalWeight;
};

struct Feat
{
    std::string name;
    std::string description;
    std::vector<Modifier> modifiers;
}

struct Feats
{
    std::vector<Feat> characterFeats;
};

struct SpecialAbility
{
    std::string name;
    std::string description;
    std::vector<Modifier> modifiers;
};

struct SpecialAbilities
{
    std::vector<SpecialAbility> abilities;
};

//bard, magus, wizard, witch, cleric, etc
enum class ClassSpellLists
{
};

struct ClassSpellLevel
{
    ClassSpellLists casterClass;
    unsigned int spellLevel
};

enum class SpellSchools
{
}

enum class SpellSubSchool
{
}

struct SpellSchool
{
    std::pair<SpellSchools, SpellSubSchool> schools;
};

enum class SpellCastingTime
{
};

enum class SpellComponents
{
};

struct Spell
{
    SpellSchool school;
    std::vector<ClassSpellLevel> levels;
    SpellCastingTime castingTime;
    std::vector<SpellComponents> components;
    bool hasScalingEffect;
    std::string effect;
    //Lambda for scaling effect function?
    //Lambda for scaling duration function?
    bool allowsSpellResistance;
    std::string spellLongDescription;
};

struct SpellLevel
{
    unsigned int spellsKnown;
    unsigned int saveDC;
    unsigned int level;
    unsigned int spellsPerDay;
    unsigned int bonusSpells;
};

struct CarryCapacity
{
    unsigned int lightLoad;
    unsigned int mediumLoad;
    unsigned int heavyLoad;
    double liftOverHead;
    double liftOffGround;
    double dragOrPush;
};

struct Money
{
    unsigned int copper;
    unsigned int silver;
    unsigned int gold;
    unsigned int platinum;
};

struct Experience
{
    unsigned int points;
    unsigned int nextLevel;
};

#endif
