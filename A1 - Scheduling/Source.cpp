#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Job {
public:
	Job(int weight, int duration) : weight(weight), duration(duration) {};
	
	static bool sortJobs(const Job & job1, const Job & job2) {
		double job1Imp = job1.importanceIndex();
		double job2Imp = job2.importanceIndex();
		
		if (job1Imp > job2Imp)
			return true;
		if (job2Imp > job1Imp)
			return false;
		//==
		return job1.weight > job2.weight;
	}

	int weight;
	int duration;

	double importanceIndex() const {
		//return weight - duration;
		return (double)weight / duration;
	}
};

ostream & operator<<(ostream & out, Job & j) {
	out << "Job w: " << j.weight << " l: " << j.duration << " val: " << j.importanceIndex();
	return out;
}



void main() {
	freopen("input/trueInput.txt", "r", stdin);
	int jobsCount;
	cin >> jobsCount;

	vector<Job> jobs;
	int weight, duration;
	for (int i = 0; i < jobsCount; i++) {
		cin >> weight >> duration;
		jobs.push_back(Job(weight, duration));
	}

	sort(jobs.begin(), jobs.end(), Job::sortJobs);

	int finishingTime = 0;
	long long sumOfTimes = 0;
	for (Job j : jobs) {
		//cout << j << endl;
		finishingTime += j.duration;
		sumOfTimes += finishingTime*j.weight;
	}
	cout << sumOfTimes << endl;
}