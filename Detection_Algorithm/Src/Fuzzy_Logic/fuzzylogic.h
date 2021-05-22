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
     * Initializes the FuzzyLogic Module using a vector of keys. These are immutable so ensure
     * that all keys are included.
    ***********************************************************************************************/
    FuzzyLogic(vector<string> keys);

    /***********************************************************************************************
     * FuzzyLogic Destructor
     *
     * Destroys the FuzzyLogic Module and cleans up memory
    ***********************************************************************************************/
    ~FuzzyLogic();

    /***********************************************************************************************
     * Add Value
     *
     * Adds a value to a FuzzyLogic Queue matching the given key
    ***********************************************************************************************/
    bool addValue(const string& key, double value);

    /***********************************************************************************************
     * Get State
     *
     * Polls the FuzzyLogic Module, returning which key has the highest score based on the sum
     * of each queue.
    ***********************************************************************************************/
    string getState();

    /***********************************************************************************************
     * Get State Index
     *
     * Polls the FuzzyLogic Module, returning the index of the key that has the highest score based
     * on the sum of each queue.
    ***********************************************************************************************/
    int getStateIndex();

private:

    /***********************************************************************************************
     * Get Key Index
     *
     * Helper method, returns the index of a provided key
    ***********************************************************************************************/
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
