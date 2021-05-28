/***************************************************************************************************
 * Meta File Interface
 *
 * @author Matthew Munson
 * @date May 27th 2021
 *
 * Tests most of the functionality included in the MetaFile class.
 *
 **************************************************************************************************/

#include <iostream>
#include "../Tools/Meta_File/meta_file.h"

using namespace std;

int main()
{
    MetaFile metaFile(10);

    for(int i = 0; i < 10; i++)
    {
        metaFile.setHero(i, OWConst::Hanzo);
        metaFile.setWeaponAction(i,OWConst::Firing);
    }

    cout << "Hero at 9 is: " << OWConst::getHeroString(metaFile.getHero(9)) << endl;
    cout << "Hero at 10 is: " << OWConst::getHeroString(metaFile.getHero(10)) << endl;

    metaFile.save("test.meta");

    MetaFile openedFile("test.meta");

    cout << "Hero at 0 is: " << OWConst::getHeroString(openedFile.getHero(0)) << endl;
    cout << "Hero at -1 is: " << OWConst::getHeroString(openedFile.getHero(-1)) << endl;

    return 0;
}