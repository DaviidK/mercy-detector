/***************************************************************************************************
 * Overwatch OWConst Implementation
 *
 * This class provides extra functionality linking the heroes enum to the corresponding string
 * array.
 * 
 * Assumptions:
 * - Hero enum is the same size and order as the Hero string array
 * - Weapon Action enum is the same size and order as the Weapon Action string array
 **************************************************************************************************/

#include "overwatchConstants.h"


/***************************************************************************************************
 * Get Hero String
 *
 * Returns the string associated with the provided Hero enum.
 * 
 * @param hero: The Heroes enum for which a string representation will be returned
 * 
 * @post: The string representation of the passed parameter Heroes enum will be returned
 * 
 * @return: The string value of the parameter Heroes enum
***************************************************************************************************/
string OWConst::getHeroString(OWConst::Heroes hero) {
    if (hero < 0 || hero >= HERO_COUNT) {
        hero = No_Hero;
    }

    return HERO_NAMES[hero];
}

/***************************************************************************************************
 * Get Weapon Action String
 *
 * Returns the string associated with the provided Weapon Action enum.
 * 
 * @param action: The WeaponActions enum for which a string representation will be returned
 * 
 * @post: The string representation of the passed parameter WeaponActions enum will be returned
 * 
 * @return: The string value of the parameter WeaponActions enum
***************************************************************************************************/
string OWConst::getWeaponActionString(OWConst::WeaponActions action) {

    if (action < 0 || action >= WEAPON_ACTION_COUNT) {
        action = No_Action;
    }

    return WEAPON_ACTIONS[action];
}

/***************************************************************************************************
 * Get Hero
 *
 * Performs an O(N) search for the requested hero in the corresponding string array.
 * 
 * @param heroString: A string which corresponds to a valid Heroes enum
 * 
 * @pre:  The passed parameter string is a valid entity in the HERO_NAMES array
 * @post: The Heroes enum value corresponding to the passed string is returned
 * 
 * @return: The Heroes enum which is represented by the passed parameter string
***************************************************************************************************/
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
 * Performs an O(N) search for the requested weapon action in the corresponding string array.
 * 
 * @param actionString: A string which corresponds to a valid WeaponActions enum
 * 
 * @pre:  The passed parameter string is a valid entity in the WEAPON_ACTIONS array
 * @post: The WeaponActions enum value corresponding to the passed string is returned
 * 
 * @return: The WeaponActions enum which is represented by the passed parameter string
***************************************************************************************************/
OWConst::WeaponActions OWConst::getAction(const string &actionString) {

    for (int i = 0; i < WEAPON_ACTION_COUNT; i++) {
        if (actionString == WEAPON_ACTIONS[i])
            return WeaponActions(i);
    }

    return No_Action;
}
