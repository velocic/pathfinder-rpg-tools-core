#ifndef PREPROCESSORDEFINES_H
#define PREPROCESSORDEFINES_H

#if defined (_WIN32)
    //PathfinderRulesEngine_EXPORTS auto-generated by cmake
    //for shared libraries it builds. Pattern is LibName_EXPORTS
    #if defined(PathfinderRulesEngine_EXPORTS)
        #define PATHFINDER_RULES_ENGINE_EXPORT __declspec(dllexport)
    #else
        #define PATHFINDER_RULES_ENGINE_EXPORT __declspec(dllimport)
    #endif
#else
    #define PATHFINDER_RULES_ENGINE_EXPORT
#endif

#endif
