#include "hfunc.h"

namespace std
{
    int hfunc(string val)
    {
        int sum = 0;
        for (int i = 0; i < val.size(); i++) {
            sum += val[i];
        }
        const double A = 0.62;
        const int M = 2222;
        return int(M * (A * sum - int(A * sum)));
    }
}
