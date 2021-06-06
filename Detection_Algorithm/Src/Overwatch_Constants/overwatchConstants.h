/***************************************************************************************************
 * Overwatch OWConst Interface
 *
 * This class provides enums for the efficient comparison of Overwatch Heroes and Weapon Actions,
 * as well as functions for converting enums to strings and vice versa.
 *
 * TO ADD A WEAPON ACTION:
 * - Increment the WEAPON_ACTION_COUNT
 * - Add the weapon action to the WeaponActions enum, keep No_Action as the last entry
 * - Add the weapon action to the WEAPON_ACTIONS string array at the corresponding index
 *
 **************************************************************************************************/

#ifndef MERCY_DETECTOR_OVERWATCHCONSTANTS_H
#define MERCY_DETECTOR_OVERWATCHCONSTANTS_H
using namespace std;
#include <string>


// Enums have no size in C++, using seperate variables to prevent hacky-ness
static const int HERO_COUNT = 33;
static const int WEAPON_ACTION_COUNT = 7; // TODO: Keep this updated!

class OWConst {

public:

    /***********************************************************************************************
     * Heroes Enum
     *
     * These are all of the heroes in Overwatch as of May 2021
    ***********************************************************************************************/
    enum Heroes {
        Ana,
        Ashe,
        Baptiste,
        Bastion,
        Brigitte,
        DVa,
        Doomfist,
        Echo,
        Genji,
        Hanzo,
        Junkrat,
        Lucio,
        Mccree,
        Mei,
        Mercy,
        Moira,
        Orisa,
        Pharah,
        Reaper,
        Reinhardt,
        Roadhog,
        Sigma,
        Soldier76,
        Sombra,
        Symmetra,
        Torbjorn,
        Tracer,
        Widowmaker,
        Winston,
        Wrecking_Ball,
        Zarya,
        Zenyatta,
        No_Hero
    };

    /***********************************************************************************************
     * Weapon Actions Enum
     *
     * These are the weapon actions we intend to identify
    ***********************************************************************************************/
    enum WeaponActions {
        Holding_Staff, Holding_Pistol, Firing, Melee, Healing, Damage_Boosting, No_Action
    };

    /***********************************************************************************************
     * Get Hero String
     *
     * Returns the string associated with the provided Hero enum.
    ***********************************************************************************************/
    static string getHeroString(Heroes hero);

    /***********************************************************************************************
     * Get Weapon Action String
     *
     * Returns the string associated with the provided Weapon Action enum.
    ***********************************************************************************************/
    static string getWeaponActionString(WeaponActions action);

    /***********************************************************************************************
     * Get Hero
     *
     * Returns the Hero enum associated with the provided string
    ***********************************************************************************************/
    static Heroes getHero(const string &heroString);

    /***********************************************************************************************
     * Get Action
     *
     * Returns the Weapon Action enum associated with the provided string
    ***********************************************************************************************/
    static WeaponActions getAction(const string &actionString);

private:

    // String array corresponding to hero enums
    static const constexpr char *HERO_NAMES[HERO_COUNT]
            {
                    "Ana", "Ashe", "Baptiste", "Bastion", "Brigitte", "D.Va", "Doomfist",
                    "Echo", "Genji",
                    "Hanzo", "Junkrat", "Lucio", "Mccree", "Mei", "Mercy", "Moira", "Orisa",
                    "Pharah",
                    "Reaper", "Reinhardt", "Roadhog", "Sigma", "Soldier: 76", "Sombra",
                    "Symmetra", "Torbjorn",
                    "Tracer", "Widowmaker", "Winston", "Wrecking Ball", "Zarya", "Zenyatta",
                    "No Hero"
            };

    // String array corresponding to weapon action enums
    static const constexpr char *WEAPON_ACTIONS[WEAPON_ACTION_COUNT]
            {
                    "Holding Staff", "Holding Pistol", "Firing", "Melee",
                    "Healing", "Damage Boosting", "No Action"
            };

};



#endif //MERCY_DETECTOR_OVERWATCHCONSTANTS_H
