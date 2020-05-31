// Copyright 2020 Robert Carneiro, Derek Meer, Matthew Tabak, Eric Lujan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "OHE.h"
#include "Backend/Database/GTable.h"
#include "Backend/Database/GType.h"

using namespace glades;

glades::OHE::OHE()
{
	OHEStrings.clear();
}

glades::OHE::~OHE()
{
	OHEStrings.clear();
}

void glades::OHE::addString(const char* newCharString)
{
	std::string newString(newCharString);
	addString(newString);
}

void glades::OHE::addString(const std::string& newString)
{
	// GType nodeContents(newString);
	if (!contains(newString))
		OHEStrings.push_back(newString);
}

unsigned int glades::OHE::size() const
{
	return OHEStrings.size();
}

std::vector<std::string> glades::OHE::getStrings() const
{
	return OHEStrings;
}

bool glades::OHE::contains(const std::string& newString) const
{
	// check if the string is already in the vector
	std::vector<std::string>::const_iterator itr = OHEStrings.begin();
	for (; itr != OHEStrings.end(); ++itr)
	{
		if ((*itr) == newString)
			return true;
	}

	return false;
}

void glades::OHE::print() const
{
	printf("[OHE] Output:\n");
	printf("[");
	for (unsigned int i = 0; i < size(); ++i)
	{
		printf("[");
		for (unsigned int j = 0; j < size(); ++j)
		{
			// print the value
			if (i == j)
				printf("%s", OHEStrings[i].c_str());
			else
				printf("0");

			// next in a list (but not last)
			if (j < size() - 1)
				printf(",");
		}

		printf("]");
		if (i < size() - 1)
			printf("\n");
	}
	printf("]\n\n");
}

int glades::OHE::indexAt(const char* needle) const
{
	std::string needleString(needle);

	int counter = 0;
	std::vector<std::string>::const_iterator itr = OHEStrings.begin();
	for (; itr != OHEStrings.end(); ++itr)
	{
		if ((*itr) == needleString)
			return counter;
		++counter;
	}

	return -1;
}

int glades::OHE::indexAt(const std::string& needle) const
{
	std::vector<int> retVal(size(), 0);

	int counter = 0;
	std::vector<std::string>::const_iterator itr = OHEStrings.begin();
	for (; itr != OHEStrings.end(); ++itr)
	{
		if ((*itr) == needle)
			return counter;
		++counter;
	}

	return -1;
}

std::vector<float> glades::OHE::operator[](const char* needle) const
{
	std::string needleString(needle);
	std::vector<float> retVal(size(), 0.01);

	int counter = 0;
	std::vector<std::string>::const_iterator itr = OHEStrings.begin();
	for (; itr != OHEStrings.end(); ++itr)
	{
		if ((*itr) == needleString)
		{
			retVal[counter] = 0.99;
			break;
		}

		++counter;
	}

	return retVal;
}

std::vector<float> glades::OHE::operator[](const std::string& needle) const
{
	std::vector<float> retVal(size(), 0.01);

	int counter = 0;
	std::vector<std::string>::const_iterator itr = OHEStrings.begin();
	for (; itr != OHEStrings.end(); ++itr)
	{
		if ((*itr) == needle)
		{
			retVal[counter] = 0.99;
			break;
		}

		++counter;
	}

	return retVal;
}

// ONLY SUPPORTS FIRST HOT FOUND
// EXPAND TO SUPPORT MULTIDIMENSIONALITY
std::string glades::OHE::operator[](const std::vector<int>& needle) const
{
	// check if the string is already in the vector
	int counter = 0;
	std::vector<int>::const_iterator itr = needle.begin();
	for (; itr != needle.end(); ++itr)
	{
		if ((*itr) == 1)
			return OHEStrings[counter];
		++counter;
	}

	return "";
}

std::string glades::OHE::operator[](const std::vector<float>& needle) const
{
	// check if the string is already in the vector
	float max = 0.0f;
	int counter = 0, index = -1;
	std::vector<float>::const_iterator itr = needle.begin();
	for (; itr != needle.end(); ++itr)
	{
		if ((*itr) >= max)
		{
			max = (*itr);
			index = counter;
		}
		++counter;
	}

	if (index >= 0)
		return OHEStrings[index];

	return "";
}

void glades::OHE::mapFeatureSpace(const shmea::GTable& gTable, int featureCol)
{
	for (unsigned int r = 0; r < gTable.numberOfRows(); ++r)
	{
		shmea::GType cCell = gTable.getCell(r, featureCol);
		std::string word = cCell.getString();
		addString(word);
	}
}

void glades::OHE::printFeatures() const
{
	printf("[OHE] ");

	for (unsigned int i = 0; i < size(); ++i)
	{
		std::string word = OHEStrings[i];
		printf("%s ", word.c_str());
	}

	printf("\n");
}
