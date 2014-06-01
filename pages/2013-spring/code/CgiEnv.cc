#include "CgiEnv.h"
#include <cstdio>

CgiEnv::CgiEnv() {
}

void CgiEnv::reset() {
    mEnvVars.resize(0);
    mEnvVals.resize(0);
}

/***********************************************************************
*                                                                      *
* pre: inUnparsed is the input string from the form in its raw format  *
* post: each var/value pair has been stored                            *
*                                                                      *
***********************************************************************/
void CgiEnv::populate(string inUnparsed) {
    string nextVar;
    string nextVal;

    while (inUnparsed != "") {
        this->extractInputString(inUnparsed, nextVar, nextVal);
        this->set(nextVar, nextVal);
    }
}

/**********************************************
*                                             *
* a different way to store var/value pairs    *
* pre: inVar and inValue are correct          *
* post: they are added to the parallel array  *
*                                             *
**********************************************/
void CgiEnv::set(const string & inVar, const string & inValue) {

    mEnvVars.push_back(inVar);
    mEnvVals.push_back(inValue);
}

/*******************************************
*                                          *
* pre: var is one of the variables stored  *
* post: corresponding value is returned    *
* exception: when variable not found       *
*                                          *
*******************************************/
string CgiEnv::get(const string & inVar) const {
    for (unsigned int i = 0; i < mEnvVars.size(); i++) {
        if (mEnvVars[i] == inVar)
            return mEnvVals[i];
    }
    return "";
}

/***************************************************************************
*                                                                          *
* extractInputString                                                       *
* an HTML input to CGI program is in the form of                           *
* title1=content1&title2=content2, ...                                     *
* The function extract the content one at a time and return it             *
* through the heading of the function                                      *
* If the input contains multiple words, they are separated by '+'. This    *
* function will remove these '+'s.                                         *
* Pre: 'buf' contains CGI input from stdin                                 *
* Post: one 'name' and 'value' pair extracted, 'buf' advaned to next pair  *
*                                                                          *
***************************************************************************/
void CgiEnv::extractInputString(
    string & ioBuf,
    string & outName,
    string & outValue)
{
    unsigned int loc = ioBuf.find("=");
    unsigned int end = ioBuf.find("&");
    string tempValue;
                                        // ready to move on

    if (loc != string::npos && end != string::npos) {
        outName = ioBuf.substr(0, loc);
        tempValue = ioBuf.substr(loc + 1, end - loc - 1);
        ioBuf = ioBuf.substr(end + 1, ioBuf.length() - end - 1);

                                        // reaches the end
    } else if (loc != string::npos) {
        outName = ioBuf.substr(0, loc);
        tempValue = ioBuf.substr(loc + 1, ioBuf.length() - loc - 1);
        ioBuf = "";
    }
    outValue = "";
    for (unsigned int i = 0; i < tempValue.length(); ) {

                                        // replace a plus with a space
        if (tempValue[i] == '+') {
            outValue += " ";
            i++;
                                        // Convert characters represented in
                                        // hex.

        } else if (tempValue[i] == '%') {
            int v;
            string tempBuf = tempValue.substr(i + 1, 2);
            (void) sscanf(tempBuf.c_str(), "%x", &v);
            char ch = v;
            outValue += ch;
            i += 3;
                                        // Otherwise, just copy the character
                                        // over.
        } else {
            outValue += tempValue[i];
            i++;
        }
    }
}

/************************************************************************
*                                                                       *
* Method: extractInputInt                                               *
* Pre:    'ioBuf' contains CGI input from stdin                         *
* Post:   one 'name' and 'value' pair extracted, 'buf' advaned to next  *
*         pair.  'value' is converted to an integer.                    *
*                                                                       *
************************************************************************/
void CgiEnv::extractInputInt(
    string & ioBuf,
    string & outName,
    int    & outValue)
{
    string theString;
    extractInputString(ioBuf, outName, theString);
    outValue = ::atoi(theString.c_str());
}

/****************************************************
*                                                   *
* a function to put the plusses back into a string  *
*                                                   *
****************************************************/
string CgiEnv::formatize(const string & inString) const {
    string tmp("");

    for (unsigned int i = 0; i < inString.length(); i++) {
        if (inString[i] == ' ')
            tmp += '+';
        else
            tmp += inString[i];
    }

    return tmp;
}

/************************************************
*						*
* getSize					*
* return the count of pairs stored in mEnvVars	*
* Pre:  none					*
* Post: the count of the pairs is returned	*
*						*
************************************************/
int CgiEnv::getSize() const
{
    return mEnvVars.size();
}



/*******************************************************************
*								   *
* getPair							   *
* return the pair of name/value at a given index		   *
* Pre:  the given index is valid				   *
* Post: the name/value pair at the specified location is returned  *
*       if the index is valid, a 'false' is returned if invalid	   *
*								   *
*******************************************************************/
bool CgiEnv::getPair(int inLoc, string & outName, string & outValue)
{
    if (mEnvVars.size() == 0)
	return false;
    if (inLoc >= 0 && inLoc < mEnvVars.size())
    {
	outName  = mEnvVars[inLoc];
	outValue = mEnvVals[inLoc];
	return true;
    }

    return false;
}
