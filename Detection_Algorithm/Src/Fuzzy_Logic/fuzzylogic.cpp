/***************************************************************************************************
 * FuzzyLogic Implementation
 *
 **************************************************************************************************/

#include "fuzzylogic.h"

#include <utility>

/***************************************************************************************************
 * FuzzyLogic Constructor
 *
 * Initializes all queues using a vector of strings
 *
 * @param keys
 **************************************************************************************************/
FuzzyLogic::FuzzyLogic(vector<string> keys)
{
    this->values = new vector<list<double>*>();
    this->keys = new vector<string>(std::move(keys));

    for(int i = 0; i < this->keys->size(); i++)
    {
        auto* valueQueue = new list<double>();

        this->values->push_back(valueQueue);
    }
}

/***************************************************************************************************
 * FuzzyLogic Destructor
 *
 * Destroys the contents of the values vector, then all instance variables
 *
 * //TODO: set pointers to null, test
 *
 **************************************************************************************************/
FuzzyLogic::~FuzzyLogic()
{
    for(unsigned i = 0; i < this->keys->size(); i++)
    {
        delete this->values->at(i);
    }

    delete this->values;

    delete this->keys;
}

/***************************************************************************************************
 * Add Value
 *
 * Adds a value to the queue matching a provided name
 *
 * @param key
 * @param value
 **************************************************************************************************/
bool FuzzyLogic::addValue(const string& key, double value)
{
    int keyIndex = getKeyIndex(key);

    // Check if the key doesn't exist
    if(keyIndex == -1)
    {
        return false;
    }

    auto uKeyIndex = (unsigned) keyIndex;

    // Check how many values are in the queue at the index
    int currentQueueSize = this->values->at(uKeyIndex)->size();

    // If there's more than QUEUE_INDEX values, get rid of one
    if(currentQueueSize >= QUEUE_SIZE)
    {
        this->values->at(uKeyIndex)->pop_back();
    }

    // Add our value to the queue
    this->values->at(uKeyIndex)->push_front(value);

    return true;
}

/***************************************************************************************************
 * Get State
 *
 * Polls all queues for the maximum value contained in the module, returns the corresponding
 * key
 *
 **************************************************************************************************/
string FuzzyLogic::getState()
{
    int index = getStateIndex();

    if(index == -1)
    {
        return "NONE";
    }
    else
    {
        auto uIndex = (unsigned) index;

        return this->keys->at(uIndex);
    }
}

/***************************************************************************************************
 * Get State Index
 *
 * Polls all queues for the maximum value contained in the module, returns the corresponding index
 * of which key was selected.
 *
 **************************************************************************************************/
int FuzzyLogic::getStateIndex()
{
    double max = -1.0;
    unsigned maxIndex = 0;

    double sum;

    for(unsigned i = 0; i < this->keys->size(); i++)
    {
        sum = -1.0;

        list<double>* nextList = this->values->at(i);

        for(double value : *nextList)
        {
            sum += value;
        }

        if(sum > max)
        {
            max = sum;
            maxIndex = i;
        }
    }

    if(max == -1.0)
    {
        return -1;
    }
    else
    {
        return (int) maxIndex;
    }
}

/***************************************************************************************************
 * Get Key Index
 *
 * Returns the index of the provided key
 *
 **************************************************************************************************/
int FuzzyLogic::getKeyIndex(const string& key)
{
    // Find the index of the provided key in this->keys
    for(unsigned i = 0; i < this->keys->size(); i++)
    {
        if(key == this->keys->at(i))
        {
            return (int) i;
        }
    }

    return -1;
}