// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

class Solution_726 {
	typedef unordered_map<string, int> element_map; //<化学元素、出现次数>

public:
	string countOfAtoms(string formula) {

		formula.append(")1");
		static stack<int> leftbkt_stack; //遇到')'就从栈中取'('的位置
		int len_formula = formula.length();

		// 一遍循环，将所有括号内的化学元素信息解析完毕
		for (int i = 0; i < len_formula;)
		{
			if (formula[i] == '(')
			{
				leftbkt_stack.push(i);
				i++;
				continue;
			}
			if (!leftbkt_stack.empty())
			{
				if (formula[i] == ')')
				{
					int pos_leftbkt = leftbkt_stack.top();
					leftbkt_stack.pop();
					i++; int j = i;
					int num = getNumber(formula, i, j);
					createNewGroup(formula, pos_leftbkt, i - 2, num);
					i = j;
					continue;
				}
			}
			i++; continue;
		}

		// 左括号位置填-1，将整个表达式解析一遍
		createNewGroup(formula, -1, len_formula - 2, 1);

		// 建list用来对元素符号排序
		list<string> elements;
		for (element_map::iterator itr = element_group[-1].begin(); itr != element_group[-1].end(); itr++)
		{
			elements.push_back(itr->first);
		}
		elements.sort();
		string ans_str = "";

		// 构建一个返回值
		int elem_n = elements.size();
		for (list<string>::iterator itr = elements.begin(); itr != elements.end(); ++itr)
		{
			ans_str.append(itr->data());
			int num = element_group[-1][itr->data()];
			if(num > 1)
				ans_str.append(to_string(num));
		}
		return ans_str;
	}

private:
	unordered_map<int, element_map> element_group; //一组括号内括号内化学元素信息，int是左括号的位置
	unordered_map<int, int> backet_pair; //<左括号位置，对应的右括号位置>

	inline bool isUpper(char ch) const {
		return ch >= 'A' && ch <= 'Z';
	}
	inline bool isLower(char ch) const {
		return ch >= 'a' && ch <= 'z';
	}
	inline bool isNumber(char ch) const {
		return ch >= '0' && ch <= '9';
	}
	inline int getNumber(string& formula, int& i, int& j) {
		// 确保(i == j)
		int num = 1;
		while (isNumber(formula[j]))
		{
			j++;
		}
		if (j > i)
		{
			string str_num = formula.substr(i, j - i);
			num = atoi(str_num.c_str());
		}
		return num;
	};

	// 解析一对括号中的化学元素，加入element_group中
	void createNewGroup(string& formula, int pos_leftbkt, int pos_end, int number)
	{
		element_map one_group;
		
		for (int pos = pos_leftbkt + 1; pos <= pos_end;)
		{
			if (formula[pos] == '(')
			{
				for (element_map::iterator i = element_group[pos].begin(); i != element_group[pos].end(); ++i)
				{
					if (one_group.find(i->first) != one_group.end())
					{
						one_group[i->first] += i->second;
					}
					else
					{
						one_group[i->first] = i->second;
					}
				}
				pos = backet_pair[pos] + 1;
			}
			else if (isUpper(formula[pos]))
			{
				string elm_name;
				int i = pos + 1;
				while (isLower(formula[i]))
					i++;
				elm_name = formula.substr(pos, i - pos);
				pos = i;

				int elm_num = getNumber(formula, pos, i);
				pos = i;

				if (one_group.find(elm_name) != one_group.end())
				{
					one_group[elm_name] += elm_num;
				}
				else
				{
					one_group[elm_name] = elm_num;
				}
			}
			else
				pos++;
		}

		if (number > 1)
		{
			for (element_map::iterator itr = one_group.begin(); itr != one_group.end(); ++itr)
			{
				itr->second *= number;
			}
		}

		element_group[pos_leftbkt] = one_group;
		backet_pair[pos_leftbkt] = pos_end + 1;
	}
};


class Solution_1711 {
public:
	int countPairs(vector<int>& deliciousness) {
		
		unordered_map<int, unsigned int> num_map;
		int len = deliciousness.size();
		int max_dlcs = 0;
		for (int i = 0; i < len; i++)
		{
			if (max_dlcs < deliciousness[i])
				max_dlcs = deliciousness[i];
			if (num_map.find(deliciousness[i]) == num_map.end())
				num_map[deliciousness[i]] = 1;
			else
				num_map[deliciousness[i]]++;
		}

		// 美味程度之和有 n_sums 种可能，n_sums <= 22
		int sums[22];
		int n_sums = 22;
		for (int i = 0; i < 22; i++)
		{
			sums[i] = 1 << i;
			if (max_dlcs * 2 < sums[i])
			{
				n_sums = i;
				break;
			}
		}
		
		unsigned long long ans = 0;
		unordered_set<int> used_set;
		unordered_map<int, unsigned int>::iterator i, j;
		for (i = num_map.begin(); i != num_map.end(); ++i)
		{
			for (int k = 0; k < n_sums; k++)
			{
				int obj = sums[k] - i->first;
				if (obj > max_dlcs)
					break;
				j = num_map.find(obj);
				if (j != num_map.end() && used_set.find(j->first) == used_set.end())
				{
					if (i->first == j->first)
					{
						if (i->second > 1)
						{
							ans += (unsigned long long)round((unsigned long long)(i->second) * (unsigned long long)(i->second - 1) * 0.5);
							cout << i->first << "+" << j->first << "=" << sums[k] << "(" << round((i->second)* (i->second - 1) * 0.5) << ")" << endl;
						}
					}
					else
					{
						ans += i->second * j->second;
						cout << i->first << "+" << j->first << "=" << sums[k] << "(" << i->second * j->second << ")" << endl;
					}
				}
			}
			used_set.insert(i->first);
		}

		return (int)(ans % 1000'000'007);
	}
};


class Solution_930 {
public:
	int numSubarraysWithSum(vector<int>& nums, int goal) {
		int sum = 0;
		unordered_map<int, int> cnt; //记录扫描过的 sum[i] 的出现次数
		int ret = 0;
		for (auto& num : nums) {
			cnt[sum]++; // sum 的数目加1（对应i），sum表示第i个元素之前元素（不包括i）的和
			sum += num; // 第i个元素及之前元素（包括i）的和
			ret += cnt[sum - goal]; // 满足 sum(含i) - sum(不含i) = goal 的 sum' 的数目
		}
		return ret;
	}
};


class Solution_4 {
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		int i = 0, j = 0, cnt = 0, mid_1 = 0, mid_2 = 0x7fffffff;
		int len_1 = nums1.size();
		int len_2 = nums2.size();
		int mid_cnt = (len_1 + len_2) / 2;
		bool find_two = false;
		if ((len_1 + len_2) % 2 == 0)
			find_two = true;
		while (i < len_1 && j < len_2)
		{
			cnt++;
			if (nums1[i] < nums2[j])
			{
				if (find_two && cnt == mid_cnt)
					mid_1 = nums1[i];
				if (cnt > mid_cnt)
					mid_2 = nums1[i];
				i++;
			}
			else
			{
				if (find_two && cnt == mid_cnt)
					mid_1 = nums2[j];
				if (cnt > mid_cnt)
					mid_2 = nums2[j];
				j++;
			}
			if (mid_2 != 0x7fffffff)
				break;
		}
		if (mid_2 == 0x7fffffff && i < len_1)
		{
			while (i < len_1)
			{
				cnt++;
				if (find_two && cnt == mid_cnt)
					mid_1 = nums1[i];
				if (cnt > mid_cnt)
					mid_2 = nums1[i];
				i++;
				if (mid_2 != 0x7fffffff)
					break;
			}
		}
		else if (mid_2 == 0x7fffffff && j < len_2)
		{
			while (j < len_2)
			{
				cnt++;
				if (find_two && cnt == mid_cnt)
					mid_1 = nums2[j];
				if (cnt > mid_cnt)
					mid_2 = nums2[j];
				j++;
				if (mid_2 != 0x7fffffff)
					break;
			}
		}
		if (find_two)
		{
			return (double)(mid_1 + mid_2) / 2;
		}
		else
			return mid_2;
	}
};


class Solution_7 {
public:
	int reverse(int x) {
		string s = to_string(x);
		std::reverse(s.begin(), s.end());
		long long num = atoll(s.c_str());
		if (num > 0x7fffffff) return 0;
		if (x < 0) num = -num;
		return num;
	}
};


class Solution_mianshi_17_10 {
public:
	int majorityElement(vector<int>& nums) {

		// 首先用“摩尔投票法”求众数
		int candidate, count = 0;
		for (int& num : nums) {
			if (count == 0) { //如果候选数的计数为0，
				candidate = num; //那么设置当前数为候选数
			}
			if (num == candidate) {
				count++; //候选数的计数增加
			}
			else {
				count--; //候选数的计数减小
			}
		}

		// 然后检验众数是否符合条件即可
		count = 0;
		int length = nums.size();
		for (int& num : nums) {
			if (num == candidate) {
				count++;
			}
		}
		return count * 2 > length ? candidate : -1;
	}
};


class Solution_275 {
public:
	int hIndex(vector<int>& citations) {
		
	}
};


class Solution_218 {
	vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
		// 声明一个优先队列（顶端高度最高）
		auto cmp = [](const pair<int, int>& a, const pair<int, int>& b) -> bool { return a.second < b.second; };
		priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> que(cmp);

		// boundaries收集所有建筑的left和right，按从小到大排序
		vector<int> boundaries;
		for (auto& building : buildings) {
			boundaries.emplace_back(building[0]);
			boundaries.emplace_back(building[1]);
		}
		sort(boundaries.begin(), boundaries.end());

		// 准备遍历boundaries
		vector<vector<int>> ret;
		int n = buildings.size(), idx = 0;
		for (auto& boundary : boundaries) {
			// 枚举buildings，把左端点在boundary左边的building插入优先队列
			while (idx < n && buildings[idx][0] <= boundary) {
				que.emplace(buildings[idx][1], buildings[idx][2]);
				idx++;
			}
			// 把右端点在boundary左边的building从优先队列的顶端移出
			while (!que.empty() && que.top().first <= boundary) {
				que.pop();
			}

			int maxn = que.empty() ? 0 : que.top().second;
			if (ret.size() == 0 || maxn != ret.back()[1]) {
				ret.push_back({ boundary, maxn });
			}
		}
		return ret;
	}
};


class Solution_1818 {
public:
	static constexpr int mod = 1'000'000'007;

	int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
		vector<int> rec(nums1);
		sort(rec.begin(), rec.end()); //从小到大排列
		int sum = 0, maxn = 0;
		int n = nums1.size();
		for (int i = 0; i < n; i++) {
			int diff = abs(nums1[i] - nums2[i]);
			sum = (sum + diff) % mod;
			int j = lower_bound(rec.begin(), rec.end(), nums2[i]) - rec.begin();
			if (j < n) {
				maxn = max(maxn, diff - (rec[j] - nums2[i])); // rec[j] >= nums2[i]
			}
			if (j > 0) {
				maxn = max(maxn, diff - (nums2[i] - rec[j - 1])); // rec[j - 1] < nums2[i]
			}
		}
		return (sum - maxn + mod) % mod;
	}
};

class Solution_1846 {
// 	遍历数组元素，统计值为 1 到 数组长度 的元素个数（“值大于数组长度”的元素视为“值等于数组长度”）
// 	遍历：i 从 1 到 数组长度，查值为 i 的元素个数 cnt[i]
// 		= 0，说明缺失元素 i，我们需要在后续找一个大于 i 的元素，将其变更为 i
// 		> 1，可以将多余的 cnt[i]−1 个元素减小，补充到之前缺失的元素上
//	遍历结束后，记“经过补充仍然缺失的元素总个数”为 miss (miss>=0)。最后数组中的最大值为 n-miss
	int maximumElementAfterDecrementingAndRearranging(vector<int> &arr) {
		int n = arr.size();
		vector<int> cnt(n + 1);
		for (int v : arr) {
			if (v > n)
				++cnt[n];
			else
				++cnt[v];
		}
		int miss = 0;
		for (int i = 1; i <= n; ++i) {
			if (cnt[i] == 0)
				++miss;
			else if (cnt[i] > 0) {
				miss = miss - cnt[i] + 1;
				if (miss < 0)
					miss = 0;
			}
		}
		return n - miss;
	}
};

int main()
{
	vector<int> v = { 0,1 };
	Solution_275 s;
	int a = s.hIndex(v);
	cout << a;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
