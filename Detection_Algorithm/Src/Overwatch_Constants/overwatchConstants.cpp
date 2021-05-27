/***************************************************************************************************
 * Overwatch OWConst Implementation
 *
 * This class provides extra functionality linking the heroes enum to the corresponding string
 * array.
 *
 **************************************************************************************************/

#include "overwatchConstants.h"


/***************************************************************************************************
 * Get Hero String
 *
 * Assumption: Hero enum is the same size and order as the Hero string array
 *
 **************************************************************************************************/
string OWConst::getHeroString(OWConst::Heroes hero) {
    if (hero < 0 || hero >= HERO_COUNT) {
        hero = No_Hero;
    }

    return HERO_NAMES[hero];
}

/***************************************************************************************************
 * Get Weapon Action String
 *
 * Assumption: Weapon Action enum is the same size and order as the Weapon Action string array
 *
 **************************************************************************************************/
string OWConst::getWeaponActionString(OWConst::WeaponActions action) {

    if (action < 0 || action >= WEAPON_ACTION_COUNT) {
        action = No_Action;
    }

    return WEAPON_ACTIONS[action];
}

/***************************************************************************************************
 * Get Hero
 *
 * O(N) search for the hero in the string array.
 *
 * TODO: Consider converting to a binary search if performance is impacted
 *
 **************************************************************************************************/
OWConst::Heroes OWConst::getHero(const string &heroString) {
    for (int i = 0; i < HERO_COUNT; i++) {
        if (heroString == HERO_NAMES[i])
            return Heroes(i);
    }

    return No_Hero;
}

/***************************************************************************************************
 * Get Action
 *
 * O(N) search for hero in the string array
 *
 * TODO: Consider converting to a binary search if performance is impacted
 *
 **************************************************************************************************/
OWConst::WeaponActions OWConst::getAction(const string &actionString) {

    for (int i = 0; i < WEAPON_ACTION_COUNT; i++) {
        if (actionString == WEAPON_ACTIONS[i])
            return WeaponActions(i);
    }

    return No_Action;
}
