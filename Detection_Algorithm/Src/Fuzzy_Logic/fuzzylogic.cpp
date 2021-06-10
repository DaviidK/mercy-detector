/***************************************************************************************************
 * FuzzyLogic Implementation
 * 
 * This module allows state to be tracked over a period of time, and for a detection algorithm's
 * certainty in a decision to be recorded. This reduces error over time.
 *
 **************************************************************************************************/

#include "fuzzylogic.h"

#include <utility>

/***************************************************************************************************
 * FuzzyLogic Constructor
 *
 * Initializes all queues using a vector of strings
 *
 * @param   keys as vector of strings
 * 
 * @pre     Keys have been declared with a non-zero size
 * @post    Initializes all queues using a vectotr of strings 
 * 
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
 * @pre     All queues are not null 
 * @post    Destroys the contents of the values vector, then all instance variables
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
 * @param key   : Name of queue
 * @param value : Value of detection confidence
 * 
 * @pre     Queues have been initlaied, Value is valid
 * @post    Adds a value to a specific queue
 * 
 * @return  Returns TRUE if a value has been added successfully,
 *          Returns FALSE if the index of the provided key does not exist
 * 
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
 * 
 * @pre     Queues have been initlaized 
 * @post    Find the maximum value that exists in the module
 * 
 * @return  Maximum value that exists in the module as String,
 *          "NONE" if queues 
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
 * @pre     Queues have been initlaized 
 * @post    Poll all queues for maximum value in the module, return the index of that key 
 * 
 * @return  Key of the maximum value that exists in the module as integer,
 *          -1 if maximum value does not exist
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
 * @param key   : key in the queue vector
 * 
 * @pre     the queue of the key exists
 * @post    iterate through the keys variable to find the index of specified key
 * 
 * @return  Index of specified key, 
 *          -1 if the specified key does not exist 
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