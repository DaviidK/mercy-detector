/***************************************************************************************************
 * FuzzyLogic Interface
 *
 * This module allows state to be tracked over a period of time, and for a detection algorithm's
 * certainty in a decision to be recorded. This reduces error over time.
 *
 **************************************************************************************************/

#ifndef MERCY_DETECTOR_FUZZYLOGIC_H
#define MERCY_DETECTOR_FUZZYLOGIC_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

static const int QUEUE_SIZE = 10;

class FuzzyLogic {

public:

    /***********************************************************************************************
     * FuzzyLogic Constructor
     *
     * Initializes all queues using a vector of strings
     *
     * @param   keys as vector of strings
     *
     * @pre     Keys have been declared with a non-zero size
     * @post    Initializes all queues using a vectotr of strings
     *
     **********************************************************************************************/
    FuzzyLogic(vector<string> keys);

    /***********************************************************************************************
     * FuzzyLogic Destructor
     *
     * Destroys the contents of the values vector, then all instance variables
     *
     * //TODO: set pointers to null, test
     * @pre     All queues are not null
     * @post    Destroys the contents of the values vector, then all instance variables
     *
     **********************************************************************************************/
    ~FuzzyLogic();

    /***********************************************************************************************
     * Add Value
     *
     * Adds a value to the queue matching a provided name
     *
     * @param key   : Name of queue
     * @param value : Value of detection confidence
     *
     * @pre     Queues have been initlaied, Value is valid
     * @post    Adds a value to a specific queue
     *
     * @return  Returns TRUE if a value has been added successfully,
     *          Returns FALSE if the index of the provided key does not exist
     *
     **********************************************************************************************/
    bool addValue(const string& key, double value);

    /***********************************************************************************************
     * Get State
     *
     * Polls all queues for the maximum value contained in the module, returns the corresponding
     *
     * @pre     Queues have been initlaized
     * @post    Find the maximum value that exists in the module
     *
     * @return  Maximum value that exists in the module as String,
     *          "NONE" if queues
     *
     **********************************************************************************************/
    string getState();

    /***********************************************************************************************
     * Get State Index
     *
     * Polls all queues for the maximum value contained in the module, returns the corresponding index
     * of which key was selected.
     *
     * @pre     Queues have been initlaized
     * @post    Poll all queues for maximum value in the module, return the index of that key
     *
     * @return  Key of the maximum value that exists in the module as integer,
     *          -1 if maximum value does not exist
     **********************************************************************************************/
    int getStateIndex();

private:

    /***********************************************************************************************
     * Get Key Index
     *
     * Returns the index of the provided key
     *
     * @param key   : key in the queue vector
     *
     * @pre     the queue of the key exists
     * @post    iterate through the keys variable to find the index of specified key
     *
     * @return  Index of specified key,
     *          -1 if the specified key does not exist
     *
     **********************************************************************************************/
    int getKeyIndex(const string& value);

    /***********************************************************************************************
     * Instance Variables
     *
     * Values is a vector of lists. Vector was selected for fast random access, the vector is
     * immutable so insertion/removal is not of concern. Lists were selected because
     * insertion/removal are most important. There will be no random access of the list, each
     * access will be a full traverse.
     *
     * Keys is a vector of strings. Like the vector of lists, it was selected for fast random
     * access. It is also immutable so insertion and removal are not of concern.
    ***********************************************************************************************/
    vector<list<double>*>* values;
    vector<string>* keys;
};


#endif //MERCY_DETECTOR_FUZZYLOGIC_H
