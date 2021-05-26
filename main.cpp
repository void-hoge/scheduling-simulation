// elevator algorithm simulation
// by mugi noda
// 2021/5/26

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <random>
#include <bitset>

template<long long SIZE>
class table {
private:
	long long pos = SIZE/2;
	std::bitset<SIZE> tasks;
	std::array<int, SIZE> wait_count;
	std::mt19937 mt;
public:
	long long size() const {
		return SIZE;
	}
	table () {
		std::random_device seed;
		mt = std::mt19937(seed());
		tasks = 0;
		for (auto& a: wait_count) {
			a = -1;
		}
		gen_task(SIZE/2);
	}
	table (int seed) {
		mt = std::mt19937(seed);
		tasks = 0;
		for (auto& a: wait_count) {
			a = -1;
		}
		gen_task(SIZE/64);
	}
	void gen_task(long long count) {
		for (long long i = 0; i < count; i++) {
			tasks[mt()%SIZE] = true;
		}
	}
	double average_wait_count() const {
		long long sum = 0;
		long long count = 0;
		for (auto a: wait_count) {
			if (a != -1) {
				sum+=a;
				count++;
			}
		}
		std::cout << "sum: " << sum << '\n';
		return (double)sum/count;
	}
	double distribution_wait_count() const {
		double average = average_wait_count();
		double tmp = 0;
		int count = 0;
		int max = 0;
		for (auto a: wait_count) {
			if (a != -1) {
				tmp += ((double)a-average)*((double)a-average);
				count++;
			}
			if (max < a) {
				max = a;
			}
		}
		std::cout << "max: " << max << '\n';
		return tmp/count;
	}
	std::vector<int> make_hiistgram(const int hoge) const {
		int max = 0;
		for (auto a:wait_count) {
			if (max < a) {
				max = a;
			}
		}
		std::vector<int> res((max/hoge)+1, 0);
		// for (auto a:res) {
		// 	a = 0;
		// }
		for (auto a:wait_count) {
			res.at(a/hoge)++;
		}
		return res;
	}
	void elevator() {
		int current_step = 0;
		int tmp = 0;
		for (; pos >= 0; pos--) {
			if (tasks[pos]) {
				tasks[pos] = false;
				current_step += tmp;
				wait_count.at(pos) = current_step;
				// std::cout << current_step << '\n';
				// dump();
				tmp = 0;
			}
			tmp++;
		}
		tmp = 0;
		pos = 0;
		for (; pos < SIZE; pos++) {
			if (tasks[pos]) {
				tasks[pos] = false;
				current_step += tmp;
				wait_count.at(pos) = current_step;
				// std::cout << current_step << '\n';
				// dump();
				tmp = 0;
			}
			tmp++;
		}
	}
	void nearest() {
		int current_step;
		while (tasks != 0) {
			// 前にあるかを探索
			long long front = -1;
			for (long long i = pos; i >= 0; i--) {
				// std::cout << i << '\n';
				if (tasks[i]) {
					front = i;
					break;
				}
			}
			// 後ろにあるかを探索
			long long back = -1;
			for (long long i = pos; i < SIZE; i++) {
				if (tasks[i]) {
					back = i;
					break;
				}
			}
			// 小さい方を選び、動く
			long long tmp = pos;
			if (front == -1) {
				tmp = back;
			}else if (back == -1) {
				tmp = front;
			}else if (front == pos) {
				tmp = pos;
			}else if (pos-front > back-pos) {
				tmp = back;
			}else if (pos-front < back-pos){
				tmp = front;
			}else {
				tmp = front;
			}
			tasks[tmp] = false;
			current_step += std::abs(pos-tmp);
			wait_count.at(tmp) = current_step;
			pos = tmp;
			// std::cout << current_step << '\n';
			// dump();
		}
	}
	void dump() const {
		for (size_t i = 0; i < SIZE; i++) {
			std::cout << tasks[i];
		}
		std::cout << '\n';
		for (size_t i = 0; i < SIZE; i++) {
			if (i == pos) {
				std::cout << '^';
			}else {
				std::cout << ' ';
			}
		}
		std::cout << '\n';
	}
};

int main() {
	int hoge = 10000;
	table<1<<15> elv(0);
	elv.elevator();
	std::cout << elv.average_wait_count() << '\n';
	std::cout << elv.distribution_wait_count() << '\n';
	auto elv_histgram = elv.make_hiistgram(hoge);
	for (int i = 0; i < elv_histgram.size(); i++) {
		std::cout << i*hoge << " " << elv_histgram.at(i) << '\n';
	}
	std::cout << '\n';
	table<1<<15> near(0);
	near.nearest();
	std::cout << near.average_wait_count() << '\n';
	std::cout << near.distribution_wait_count() << '\n';
	auto near_histgram = near.make_hiistgram(hoge);
	for (int i = 0; i < near_histgram.size(); i++) {
		std::cout << i*hoge << " " << near_histgram.at(i) << '\n';
	}
	return 0;
}
