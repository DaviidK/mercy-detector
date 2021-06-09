/***************************************************************************************************
 * Meta File Implementation
 *
 * @author Matthew Munson
 * @date May 27th 2021
 *
 * This class provides file open/save functionality for metadata files and accessors for the
 * identified heroes and identified actions lists.
 *
 * A metadata file is formatted as:
 *
 * FRAME_NUMBER, HERO_NAME, WEAPON_ACTION
 *
 * With an arbitrary number of rows.
 *
 **************************************************************************************************/

#include "meta_file.h"

/***************************************************************************************************
 * Meta File Integer Constructor
 *
 * Creates a meta file from scratch and initializes it as empty. The identifiedHeroes and
 * Actions should always be the same length, and are made for each frame of the video.
 *
 * @param frameCount: The number of frames contained in the video associated with the created
 *        metafile
 * 
 * @post: A metafile is created from scratch, with OWConst::No_Hero and OWConst::No_Action populated
 *        for each frame
 **************************************************************************************************/
MetaFile::MetaFile(int frameCount)
{
    if(frameCount < 1)
    {
        cerr << "Error: Metafile frame count must be greater than 1" << endl;
        exit(-1);
    }

    for(int i = 0; i < frameCount; i++)
    {
        identifiedHeroes.push_back(OWConst::No_Hero);
        identifiedActions.push_back(OWConst::No_Action);
    }
}

/***************************************************************************************************
 * Meta File String Constructor
 *
 * Creates a meta file from an existing CSV file. Exits if the file does not exist or has no
 * data. Ignores lines of data that are improperly formatted, allowing for comments in a
 * CSV file if desired
 *
 * @param filename: The file location of a .csv file which contains data to be loaded into a MetaFile
 *        object
 * 
 * @pre:  A CSV file exists at the location specified by filename, containing data to be loaded into
 *        a meta file
 * @post: The metafile located at 'filename' will be loaded into this object
 **************************************************************************************************/
MetaFile::MetaFile(const string& filename)
{
    vector<vector<string>> data;

    csv_wrapper::readFromCSV(filename, data);

    if(data.empty())
    {
        cerr << "Error: File does not exist" << endl;
        exit(-1);
    }

    for(int i = 0; i < data.size(); i++)
    {
        if(data[i].size() < 3)
        {
            cerr << "Warning: Improperly formatted data on line " << i + 1 << "." << endl;
            cerr << "Skipping this line" << endl;

            continue;
        }

        string hero = data[i][1];
        string action = data[i][2];

        identifiedHeroes.push_back(OWConst::getHero(hero));
        identifiedActions.push_back(OWConst::getAction(action));
    }
}

/***************************************************************************************************
 * MetaFile NoArg Constructor
 *
 * @post: Does nothing - the object is not initialized
 **************************************************************************************************/
MetaFile::MetaFile() = default;

/***************************************************************************************************
 * Set Hero
 *
 * Checks that the provided index is inside the valid range, then sets the hero at this index
 * 
 * @param index: Frame number at which a specific hero should be set
 * @param hero: The hero which will be used to label the specified frame
 * 
 * @post: The given frame will be labelled with the OWConst value of the parameter hero
 * 
 * @return: Boolean value, with true indicating a successful labelling of the requested frame with 
 *          the specified weapon action. Returns false for out of range indices
 **************************************************************************************************/
bool MetaFile::setHero(int index, OWConst::Heroes hero)
{
    if(index < 0 || index > identifiedHeroes.size())
    {
        return false;
    }

    identifiedHeroes[index] = hero;

    return true;
}

/***************************************************************************************************
 * Set Weapon Action
 *
 * Checks that the provided index is inside the valid range, then sets the action at this index
 * 
 * @param index: Frame number at which a specific weapon action should be set
 * @param hero: The weapon action which will be used to label the specified frame
 * 
 * @post: The given frame will be labelled with the OWConst value of the parameter weapon action
 * 
 * @return: Boolean value, with true indicating a successful labelling of the requested frame 
 *          with the specified weapon action. Returns false for out of range indices
 **************************************************************************************************/
bool MetaFile::setWeaponAction(int index, OWConst::WeaponActions action)
{
    if(index < 0 || index > identifiedActions.size())
    {
        return false;
    }

    identifiedActions[index] = action;

    return true;
}

/***************************************************************************************************
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
 **************************************************************************************************/
OWConst::Heroes MetaFile::getHero(int index)
{
    if(index < 0 || index >= identifiedHeroes.size())
    {
        return OWConst::No_Hero;
    }

    return identifiedHeroes[index];
}

/***************************************************************************************************
 * Get Weapon Action
 *
 * Gets the Weapon Action at the provided frame index
 * 
 * @param index: Frame number from which the currently labelled weapon action will be pulled
 * 
 * @post: The current weapon action at the specified frame will be returned.
 * 
 * @return: The currently labelled hero at the specified frame. Will return No_Action for invalid 
 *          indices
 **************************************************************************************************/
OWConst::WeaponActions MetaFile::getWeaponAction(int index)
{
    if(index < 0 || index >= identifiedHeroes.size())
    {
        return OWConst::No_Action;
    }

    return identifiedActions[index];
}

/***************************************************************************************************
 * Save
 *
 * Saves the identified heroes and weapon actions to a csv file at the provided path and filename.
 *
 * @param filename: The file location at which a .csv file will be written
 * 
 * @post: If a file currently exists at the location specified by 'filename', it will be overwritten.
 *        Otherwise, a new file will be created with the data contained in the current MetaFile
 *        object
 **************************************************************************************************/
void MetaFile::save(const string& filename)
{
    vector<vector<string>> data;

    vector<string> row;

    for(int i = 0; i < identifiedHeroes.size(); i++)
    {
        row.clear();

        row.push_back(to_string(i + 1));
        row.push_back(OWConst::getHeroString(identifiedHeroes[i]));
        row.push_back(OWConst::getWeaponActionString(identifiedActions[i]));

        data.push_back(row);
    }

    csv_wrapper::saveToCSV(filename, data);
}
