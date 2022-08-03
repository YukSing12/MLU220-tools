#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
void test() {
    int a = 0;
    for(int i = 0; i < 10000000; i++) {
        a = i + 1;
    }
}

int main() {
    int nthrds = 4;
    omp_set_num_threads(nthrds);
    timeval tStart,cTime;
    long long tPassed = 0;
    gettimeofday(&tStart, 0);
    for (int i = 0; i < 100; i++) {
        test();
    }
    gettimeofday(&cTime, 0);
    cTime.tv_sec -= tStart.tv_sec;
    cTime.tv_usec -= tStart.tv_usec;
    tPassed = 1000000LL * cTime.tv_sec + cTime.tv_usec;
    tPassed /= 1000;
    printf("Time = %lld\n", tPassed);
    gettimeofday(&tStart, 0);
    #pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        test();
    }
    gettimeofday(&cTime, 0);
    cTime.tv_sec -= tStart.tv_sec;
    cTime.tv_usec -= tStart.tv_usec;
    tPassed = 1000000LL * cTime.tv_sec + cTime.tv_usec;
    tPassed /= 1000;
    printf("Time = %lld\n", tPassed);
    return 1;
}