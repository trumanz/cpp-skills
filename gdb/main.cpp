#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
int tmp;
void dprint(const int& a, const vector<string>& vs)
{
    tmp += a;
    tmp += vs.size();
}

int main(int argc, char** argv)
{
    int a = 0;
    vector<string> vs;
    for(int i = 0; i < 5; i++) {
        printf("i=%d\n", i);
        a += i;
        vs.push_back(std::to_string(i));
        dprint(a, vs);
    }
}
