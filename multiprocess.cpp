#include <iostream>
#include <vector>
#include <random>
#include <array>

template<long long SIZE, typename rand_class>
class task_table {
private:
	const int seed;
	std::mt19937 mt;
	rand_class rnd;
public:
	std::array<int, SIZE> requiered_time;
	std::array<int, SIZE> finished_time;

	task_table (int seed, rand_class rnd):seed(seed), rnd(rnd){
	}
	void gen_task() {
		mt = std::mt19937(seed);
		for (auto &a: finished_time) {
			a = 0;
		}
		for (long long i = 0; i < SIZE; i++) {
			requiered_time.at(i) = std::abs(rnd(mt));
		}
	}
	double get_average_finish_time() {
		double sum = 0;
		for (auto a: finished_time) {
			sum += a;
		}
		return (double)sum/finished_time.size();
	}
	double get_variance() {
		double average = get_average_finish_time();
		double sum = 0;
		for (auto a: finished_time) {
			sum += (a - average)*(a - average);
		}
		return (double)sum/finished_time.size();
	}
	void add_all(const int time) {
		// 割り込みの時間用
		for (long long i = 0; i < SIZE; i++) {
			if (requiered_time.at(i) != 0) {
				// 終わっていないタスクに対してtimeを足す
				finished_time.at(i) += time;
			}
		}
	}
	int run_little(const int idx, const int time) {
		try {
			if (requiered_time.at(idx) == 0) {
				return 0;
			} else if (requiered_time.at(idx) >= time) {
				add_all(time);
				requiered_time.at(idx) -= time;
				return time;
			}else {
				// time以下で終わるとき
				add_all(requiered_time.at(idx));
				int tmp = requiered_time.at(idx);
				requiered_time.at(idx) = 0;
				return tmp;
			}
		} catch(std::exception e) {
			std::cerr << "out of range" << '\n';
		}
		return 0;
	}
	bool is_finished() {
		for (auto a:requiered_time) {
			if (a != 0) {
				return false;
			}
		}
		return true;
	}
};

template<long long SIZE, typename rand_class>
long long round_robin(task_table<SIZE, rand_class>& task, const int interrupt, const int quantum) {
	long long alltime = 0;
	for (long long i = 0; !task.is_finished(); i++) {
		int tmp = task.run_little((i%SIZE), quantum);
		alltime+=tmp;
		if (tmp) {
			task.add_all(interrupt);
			alltime+=interrupt;
		}
	}
	return alltime;
}

template<long long SIZE, typename rand_class>
long long fcfs(task_table<SIZE, rand_class>& task, const int interrupt) {
	const int VERY_BIG_NUM = 2147483647;
	long long alltime = 0;
	for (long long i = 0; i < SIZE; i++) {
		alltime += task.run_little(i, VERY_BIG_NUM);
		task.add_all(interrupt);
		alltime+=interrupt;
	}
	return alltime;
}

int main() {
	task_table<1000, std::uniform_int_distribution<>> task(0, std::uniform_int_distribution<>(0, 10000));
	// task_table<100, std::normal_distribution<>> task(0, std::normal_distribution<>(500.0, 20.0));
	task.gen_task();
	std::cout << "FCFS" << '\n';
	std::cout << "total execute time: " << fcfs(task, 1) << '\n';
	std::cout << "average execute time: " << task.get_average_finish_time() << '\n';
	std::cout << "variance of execute time: " << task.get_variance() << '\n';
	std::cout << '\n';
	task.gen_task();
	std::cout << "Round Robin" << '\n';
	std::cout << "total execute time: " << round_robin(task, 1, 1000) << '\n';
	std::cout << "average execute time: " << task.get_average_finish_time() << '\n';
	std::cout << "variance of execute time: " << task.get_variance() << '\n';
	return 0;
}
