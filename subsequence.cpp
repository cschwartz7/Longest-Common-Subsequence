#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct sequenceSet
{
	string name;
	string sequence;
};

struct groupSet
{
	string sequence1;
	string sequence2;
};

string lcs(sequenceSet, sequenceSet);
void compareSequence(vector<sequenceSet> *, ofstream&);

int main()
{
	ifstream inFile;
	ofstream outFile;
	vector<sequenceSet> sequence_vec;
	sequenceSet fullSequence;
	string buffer;
	
	inFile.open("tulp.txt");
	outFile.open("output.txt");
	
	while(getline(inFile, buffer))
	{		
		if(buffer[0] == '>')
		{
			fullSequence.name = buffer;
			getline(inFile, fullSequence.sequence);
			sequence_vec.push_back(fullSequence);
		}
	}
	
	cout << endl << endl;
	
	compareSequence(&sequence_vec, outFile);
	
	cout << endl << endl;
	
	inFile.close();
	outFile.close();
		
	return 0;
}

string lcs(sequenceSet sequence1, sequenceSet sequence2)
{
	int m = sequence1.sequence.size(), n = sequence2.sequence.size();
	int matchArray[m + 1][n + 1];
	string lcs = "";
	
	//Build LCS matrix to find where characters match
	for(int i = 0; i < m + 1; i++)
		for(int j = 0; j < n + 1; j++)
		{
			if(i == 0 || j == 0)
				matchArray[i][j] = 0;
			else if(sequence1.sequence[i - 1] == sequence2.sequence[j - 1])
				matchArray[i][j] = matchArray[i - 1][j - 1] + 1;
			else
				matchArray[i][j] = max(matchArray[i - 1][j], matchArray[i][j - 1]);
		}
	
	while(m > 0 && n > 0)
	{
		if(sequence1.sequence[m - 1] == sequence2.sequence[n - 1])
		{
			lcs = sequence1.sequence[m - 1] + lcs;
			m--;
			n--;
		}
		else if(matchArray[m - 1][n] > matchArray[m][n - 1])
			m--;
		else
			n--;
	}
	
	return lcs;
}

void compareSequence(vector<sequenceSet> *sequence_vec, ofstream& outFile)
{
	int compare[sequence_vec->size()][sequence_vec->size()];
	vector<groupSet> groups_vec;
	groupSet group;
	sequenceSet groupSequence;
	string subsequence;
	int highest, iIndex, jIndex;
	
	while(sequence_vec->size() > 1)
	{
		highest = 0;
		
		for(int i = 0; i < sequence_vec->size(); i++)
			for(int j = i + 1; j < sequence_vec->size(); j++)
				{
					subsequence = lcs((*sequence_vec)[i], (*sequence_vec)[j]);
					compare[i][j] = subsequence.size();
					
					//Print subsequence to outfile
					outFile << (*sequence_vec)[i].name << " & " << (*sequence_vec)[j].name << endl;
					outFile << subsequence << endl << endl;
					
					if(compare[i][j] > highest)
						highest = compare[i][j];
				}	
		
		//Find lcs with the highest length
		for(int i = 0; i < sequence_vec->size(); i++)
			for(int j = i + 1; j < sequence_vec->size(); j++)
				if(highest == compare[i][j])
				{
					iIndex = i;
					jIndex = j;
					break;
				}				

		//Add new group to group vector
		group.sequence1 = (*sequence_vec)[iIndex].name;
		group.sequence2 = (*sequence_vec)[jIndex].name;
		groups_vec.push_back(group);
		
		//Combine sequences and add to sequence vector
		groupSequence.name = (*sequence_vec)[iIndex].name + "|" + (*sequence_vec)[jIndex].name;
		groupSequence.sequence = lcs((*sequence_vec)[iIndex], (*sequence_vec)[jIndex]);
		sequence_vec->push_back(groupSequence);
		
		//Remove the two sequences used in the group from the sequence vector to avoid using them again
		sequence_vec->erase(sequence_vec->begin() + iIndex);
		sequence_vec->erase(sequence_vec->begin() + jIndex - 1);
	}
	
	cout << "GROUPS FORMED" << endl;
	for(int i = 0; i < groups_vec.size(); i++)
		cout << groups_vec[i].sequence1 << ", " << groups_vec[i].sequence2 << endl;
}