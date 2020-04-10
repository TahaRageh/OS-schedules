#ifndef SJFSCHED_H
#define SJFSCHED_H
#include "scheduler.h"
#include <QtAlgorithms>

class SJFSched : public Scheduler
{

private :
    bool isPreemtive ;
    static bool cmp(const SysProcess &p1 , const SysProcess &p2);
    // static bool cmp_first(const SysProcess &p1,const SysProcess &p2) ;
public:

    static float finish   ;  // attribute to use as a pointer of the finish of each process

    SJFSched();
    SJFSched(bool isPreemtive , QVector<SysProcess> processes );

    void set_isPreemptive(bool value);
    bool get_isPreemptive();

    void schedule() ;

   float waitingTime();


};

#endif // SJFSCHED_H


