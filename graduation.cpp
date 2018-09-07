  #include <iostream>
#include <cstring>
#define INT_MAX 9999999	//in ubuntu environment, INT_MAX is not defined
using namespace std;

int testCase; //num of test cases (C) (0~50)
int numOfMajors; //num of majors (N) (1~12)
int requiredMajors; //num of majors needed to take for graduation(K) (0~N)
int numOfSemester; //num of semesters left(M) (1~10)
int limitOfMajorPerSemester; //limitation of num of courses can be taken in one semester (L) (1~10)
int prerequisites[12]; //prerequisites for each majors. bitWise.
int openMajors[10]; //majors that opens for each smemsters. bitWise
int memoization[10][1 << 12]; //for memoization

//get input and initialize datum
void initialize()
{
	//reset datas
	memset(prerequisites, 0, sizeof(prerequisites));
	memset(openMajors, 0, sizeof(openMajors));
	memset(memoization, -1, sizeof(memoization));
	cin >> numOfMajors >> requiredMajors >> numOfSemester >> limitOfMajorPerSemester;
	for (int i = 0; i < numOfMajors; i++)
	{
		int numOfPre;
		cin >> numOfPre;
		for (int j = 0; j < numOfPre; j++)
		{
			int pre;
			cin >> pre;
			prerequisites[i] |= (1 << pre); //set prerequisites bitwisely.
		}
	}

	for (int i = 0; i < numOfSemester; i++)
	{
		int majorNumThisSemester;
		cin >> majorNumThisSemester;
		for (int j = 0; j < majorNumThisSemester; j++)
		{
			int major;
			cin >> major;
			openMajors[i] |= (1 << major);
		}
	}
}

//counts how many bits are on in bitSet
int bitCount(const int bitSet)
{
	if (bitSet == 0)
		return 0;
	return bitSet % 2 + bitCount(bitSet >> 1);
}

//remove courses in 'canTake' variable that cannot take because of prerequisites
void removeUnableToTake(int& canTake, const int taken)
{
	for (int i = 0; i < numOfMajors; i++)
	{
		if ((canTake & (1 << i)) && 
			((taken & prerequisites[i]) != prerequisites[i]))
		{
			canTake &= ~(1 << i);
		}
	}
}

//return smaller one
int min(int num1, int num2)
{
	return (num1 < num2) ? num1 : num2;
}

//solve the problem
int graduate(int semester, int taken)
{
	//base case : have taken enough majors
	if (bitCount(taken) >= requiredMajors)
		return 0;
	//base case : all semesters are end without taking enough courses
	if (semester >= numOfSemester)
		return INT_MAX;

	int& ret = memoization[semester][taken];

	//memoization check
	if (ret > 0)
		return ret;
	ret = INT_MAX;

	//canTake : majors that can be taken out of majors open in this semesters
	int canTake = (openMajors[semester] & (~taken));
	removeUnableToTake(canTake, taken);

	//test every cases that takes at least one course in this semester
	for (int subset = canTake; subset; subset = ((subset - 1) & canTake))
	{
		if (bitCount(subset) > limitOfMajorPerSemester)
			continue;
		ret = min(ret, graduate(semester + 1, taken | subset) + 1);
	}
	//case that not taking any major
	ret = min(ret, graduate(semester + 1, taken));
	return ret;
}

//process the result
void processResult(int res)
{
	if (res > 10)
		cout << "IMPOSSIBLE" << endl;
	else
		cout << res << endl;
}

int main()
{
	cin >> testCase;
	for (int i = 0; i < testCase; i++)
	{
		initialize();
		processResult(graduate(0, 0));
	}

}


//test case
/*

2
4 4 4 4
0
1 0
3 0 1 3
0
4 0 1 2 3
4 0 1 2 3
3 0 1 3
4 0 1 2 3
4 2 2 4
1 1
0
1 3
1 2
3 0 2 3
3 1 2 3


*/
