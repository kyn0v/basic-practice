#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class greaterCustom {
   public:
    greaterCustom(T x) { threshold = x; }
    bool operator()(T x) const { return x > threshold; }

   private:
    T threshold;
};

int main() {
    int n, threshold, cnt;
    cin>>n>>threshold;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    // greaterCustom<int> functor(threshold);
    cnt = count_if(nums.begin(), nums.end(), greaterCustom<int> (threshold));
    cout << cnt << endl;
    return 0;
}