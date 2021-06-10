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
 **************************************************************************************************/

#ifndef MERCY_DETECTOR_META_FILE_H
#define MERCY_DETECTOR_META_FILE_H

#include "../CSV/csv_wrapper.h"
#include "../../Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

class MetaFile {

public:

    /***********************************************************************************************
     * Meta File Default Constructor
     *
     * @post: Does nothing - the object is not initialized
     **********************************************************************************************/
    MetaFile();

    /***********************************************************************************************
     * Meta File FrameCount Constructor
     *
     * Use this constructor to create a metadata object from scratch. The identified hero and
     * action for each frame will be set to No_Hero and No_Action respectively. Note that the size 
     * of the metadata object is immutable
     *
     * @param frameCount: The number of frames contained in the video associated with the created
     *        metafile
     * 
     * @post: A metafile is created from scratch, with OWConst::No_Hero and OWConst::No_Action 
     *        populated for each frame
     **********************************************************************************************/
    MetaFile(int frameCount);

    /***********************************************************************************************
     * Meta File Filename Constructor
     *
     * Use this constructor to open a metadata file that is saved as a .csv.
     *
     * @param filename: The file location of a .csv file which contains data to be loaded into a 
     *        MetaFile object
     * 
     * @pre:  A CSV file exists at the location specified by filename, containing data to be loaded
     *        into a meta file
     * @post: The metafile located at 'filename' will be loaded into this object
     **********************************************************************************************/
    MetaFile(const string& filename);

    /***********************************************************************************************
     * Set Hero
     *
     * Sets the identified hero of a given frame to the provided hero
     *
     * @param index: Frame number at which a specific hero should be set
     * @param hero: The hero which will be used to label the specified frame
     * 
     * @post: The given frame will be labelled with the OWConst value of the parameter hero
     * 
     * @return: Boolean value, with true indicating a successful labelling of the requested frame 
     *          with the specified weapon action. Returns false for out of range indices
     **********************************************************************************************/
    bool setHero(int index, OWConst::Heroes hero);

    /***********************************************************************************************
     * Set Weapon Action
     *
     * Sets the identified weapon action of a given frame to the provided action
     * 
     * @param index: Frame number at which a specific weapon action should be set
     * @param hero: The weapon action which will be used to label the specified frame
     * 
     * @post: The given frame will be labelled with the OWConst value of the parameter weapon action
     * 
     * @return: Boolean value, with true indicating a successful labelling of the requested frame 
     *          with the specified weapon action. Returns false for out of range indices
     **********************************************************************************************/
    bool setWeaponAction(int index, OWConst::WeaponActions action);

    /***********************************************************************************************
     * Get Hero
     *
     * Gets the Hero at the provided frame index
     * 
     * @param index: Frame number from which the currently labelled hero will be pulled
     * 
     * @post: The current hero at the specified frame will be returned.
     * 
     * @return: The currently labelled hero at the specified frame. Will return No_Hero for invalid 
     *          indices
     **********************************************************************************************/
    OWConst::Heroes getHero(int index);

    /***********************************************************************************************
     * Get Weapon Action
     *
     * Gets the Weapon Action at the provided frame index
     * 
     * @param index: Frame number from which the currently labelled weapon action will be pulled
     * 
     * @post: The current weapon action at the specified frame will be returned.
     * 
     * @return: The currently labelled hero at the specified frame. Will return No_Action for 
     *          invalid indices
     **********************************************************************************************/
    OWConst::WeaponActions getWeaponAction(int index);


    /***********************************************************************************************
     * Save
     *
     * Saves the identified heroes and weapon actions as a csv file at the provided path
     * 
     * @param filename: The file location at which a .csv file will be written
     * 
     * @post: If a file currently exists at the location specified by 'filename', it will be 
     *        overwritten. Otherwise, a new file will be created with the data contained in the 
     *        current MetaFile object.
     **********************************************************************************************/
    void save(const string& filename);

private:

    // A vector representing the hero identified at each frame in a video
    vector<OWConst::Heroes> identifiedHeroes;
    // A vector representing the weapon action identified at each frame in a video
    vector<OWConst::WeaponActions> identifiedActions;
};

#endif //MERCY_DETECTOR_META_FILE_H
