#include <vector>
template<typename T>
class Iterator {
protected:
    int iteration = 0;
    std::vector<T> list;
public:
    T operator()() {
        if (iteration >= list.size) return T();
        return list[iteration++];
    }
};