// [[Rcpp::plugins(cpp20)]]
#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <numeric>

#define min(x, y)((x>y?y:x))
using vec = std::vector<unsigned int>;
// Overload << for std::vector<T>
// template <typename T>
// std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
//   os << "[";
//   for (size_t i = 0; i < v.size(); ++i) {
//     os << v[i];
//     if (i + 1 != vec.size())
//       os << ", ";
//   }
//   os << "]\n";
//   return os;
// }

int get_min_index(vec & nums){

  auto min_it = std::find_if(nums.begin(), nums.end(),
                             [&](auto x) {return x != 0;});
  if(min_it != nums.end())
    return std::distance(nums.begin(), min_it);
  return -1;
}

int get_max_index(vec & nums){

  auto max_it = std::find_if(nums.rbegin(), nums.rend(),
                             [&](auto x) {return x != 0;});
  if(max_it != nums.rend())
    return std::distance(nums.begin(), max_it.base());
  return -1;
}

class PQnumber {
private:
  int sign;
  unsigned int p, q;
  vec nums;
public:
  std::string repr = "[pq]";
  PQnumber(int sign, unsigned int p, unsigned int q, vec nums){
    if(sign != 1 && sign != -1)
      throw std::runtime_error("sign must be either -1L or 1L");
    if(p + q + 1 != nums.size())
      throw std::runtime_error("p + q + 1 must equal length(nums)");
    if(std::any_of(nums.begin(), nums.end(), [](auto x){return  x > 9;}))
      throw std::runtime_error("nums must contain integers [0-9]");
    if(std::all_of(nums.begin(), nums.end(), [](int x){return x == 0;}))
      this->sign = 1;
    else this->sign = sign;
    this->p = p;
    this->q = q;
    this->nums = nums;
    this->format();
    this->repr += "\n";
  }
  void format(unsigned int digits = 10, bool scientific = false){
    int mn = get_min_index(nums), mx = get_max_index(nums);
    unsigned int min_index, max_index;
    auto nums = this->nums;

    if(mn == -1|| mx == -1) {
      this->repr = "0\n";
      return;
    }
    else{
      min_index = mn;
      max_index = mx;
    }

    unsigned int p = this-> p;
    unsigned int new_p = p > min_index? p - min_index : 0;
    unsigned int q = max_index > p? max_index - p - 1 : 0;
    auto start = nums.rbegin() + (nums.size() - p - q - 1);
    auto end = start + new_p + q + 1;
    vec v(start, end);
    this->repr += this->sign < 0 ? "-" : " ";


    unsigned int index = get_min_index(v);
    int new_q = (int)q;


    if((p > digits && index > 5)|| q > digits){

      if(q == 0){
        new_q = 1 - index -1;
        v.erase(v.begin(), v.begin() + index);
      }
      if(std::all_of(v.begin()+1, v.end(), [&](auto i){
        return i == 0;}))
        v.resize(1);

      else v.resize(min((int)min(q + digits,  10),
                              get_max_index(v)) + 1);


      this->repr += std::to_string(v[0]);

      if(v.size() > 1){

        this->repr += ".";
        this->repr += std::accumulate(v.begin() + 1, v.end(),
                                   std::string{}, [](const std::string &a, int b){
                                     return a + std::to_string(b);
                                   });
      }
      this->repr+="e";
      this->repr+= new_q <0? "" : "+";
      this->repr+=std::to_string(new_q);

    }
    else{
      v.resize(min((int)min(q + digits,  10),
                   get_max_index(v)) + 1);
      std::string dec = v.size() > q + 1? "." : "";

      this->repr += std::accumulate(v.begin(), v.begin() + q + 1,
                                 std::string{}, [](const std::string &a, int b){
                                   return a + std::to_string(b);
                                 });
      this -> repr += dec;
      this->repr += std::accumulate(v.begin() + q+1, v.end(),
                                 std::string{}, [](const std::string &a, int b){
                                   return a + std::to_string(b);
                                 });
    }

  }

};


// [[Rcpp::export]]
std::string my_format(int sign, unsigned int p, unsigned int q,
                      std::vector<unsigned int> nums) {
  PQnumber A(sign, p, q, nums);
  return A.repr;
}

