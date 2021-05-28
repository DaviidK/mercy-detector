/***************************************************************************************************
 * Meta File Interface
 *
 * @author Matthew Munson
 * @date May 27th 2021
 *
 * This class provides functions for creating and modifying a metadata file. This file is designed
 * to accompany a video and represent the "ground truth" of the Overwatch hero and weapon action
 * for every frame in the video.
 *
 * A metadata file can be created from scratch by specifying the number of frames in the video.
 * The default value of No_Hero and No_Action will be assigned for each frame.
 *
 * A metadata file can also be opened from an existing .csv file.
 *
 **************************************************************************************************/

#ifndef MERCY_DETECTOR_META_FILE_H
#define MERCY_DETECTOR_META_FILE_H

#include "../CSV/csv_wrapper.h"
#include "../../Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

class MetaFile {

public:

    /***********************************************************************************************
     * Meta File FrameCount Constructor
     *
     * Use this constructor to create a metadata object from scratch. The identified hero and
     * action for each frame will be set to No_Hero and No_Action respectively.
     *
     * Note that the size of the metadata object is immutable.
     *
     **********************************************************************************************/
    MetaFile(int frameCount);

    /***********************************************************************************************
     * Meta File Filename Constructor
     *
     * Use this constructor to open a metadata file that is saved as a .csv.
     *
     **********************************************************************************************/
    MetaFile(const string& filename);

    /***********************************************************************************************
     * Set Hero
     *
     * Sets the identified hero of a given frame to the provided hero.
     *
     * Returns false for out of range indices.
     *
     **********************************************************************************************/
    bool setHero(int index, OWConst::Heroes hero);

    /***********************************************************************************************
     * Set Weapon Action
     *
     * Sets the identified weapon action of a given frame to the provided action.
     *
     * Returns false for out of range indices.
     *
     **********************************************************************************************/
    bool setWeaponAction(int index, OWConst::WeaponActions action);

    /***********************************************************************************************
     * Get Hero
     *
     * Gets the Hero at the provided frame index. Returns No_Hero for invalid indices.
     *
     **********************************************************************************************/
    OWConst::Heroes getHero(int index);

    /***********************************************************************************************
     * Get Weapon Action
     *
     * Gets the Weapon Action at the provided frame index. Returns No_Action for invalid indices.
     *
     **********************************************************************************************/
    OWConst::WeaponActions getWeaponAction(int index);


    /***********************************************************************************************
     * Save
     *
     * Saves the identified heroes and weapon actions as a csv file at the provided path.
     **********************************************************************************************/
    void save(const string& filename);

private:

    // A vector representing the hero identified at each frame in a video
    vector<OWConst::Heroes> identifiedHeroes;
    // A vector representing the weapon action identified at each frame in a video
    vector<OWConst::WeaponActions> identifiedActions;
};

#endif //MERCY_DETECTOR_META_FILE_H
