#include "rrsched.h"

float RRSched::getQuantumTime() const
{
    return quantumTime;
}

void RRSched::setQuantumTime(float value)
{
    quantumTime = value;
}

bool RRSched::cmp(const SysProcess &p1, const SysProcess &p2)
{
    if(p1.getArrivalTime()<p2.getArrivalTime())
    {
        return true;
    }
    else if(p1.getArrivalTime()==p2.getArrivalTime())
    {
        QString p1name=p1.getName();
        QString p2name=p2.getName();
         p1name=p1name.remove(0,1);
         p2name=p2name.remove(0,1);
         return p1name.toInt()<p2name.toInt();
    }
    return false;
}

RRSched::RRSched()
{

}

void RRSched::schedule()
{
    qSort(processes.begin(),processes.end(),cmp);
    QQueue<SysProcess> processesQueue;
    for(int i=0;i<processes.size();i++)
    {
        processesQueue.enqueue(processes[i]);
    }
    QQueue<SysProcess> runningProcessesQueue;
    runningProcessesQueue.enqueue(processes[0]);
    Interval interval = Interval();
    intervals.clear();
    float finish = processesQueue.dequeue().getArrivalTime();
    SysProcess p;
    while(!runningProcessesQueue.empty())
    {
        p = runningProcessesQueue.dequeue();
        interval.setFrom(finish);
        if(p.getBurstTime() > quantumTime)
        {
            finish+=quantumTime;
            p.setBurstTime(p.getBurstTime()-quantumTime);
            while(!processesQueue.empty() && processesQueue.head().getArrivalTime() <= finish)
            {

                    runningProcessesQueue.enqueue(processesQueue.dequeue());
            }
            runningProcessesQueue.enqueue(p);
        }
        else
        {
            finish+=p.getBurstTime();
            while(!processesQueue.empty() && processesQueue.head().getArrivalTime() <= finish)
            {

                    runningProcessesQueue.enqueue(processesQueue.dequeue());
            }
        }


        interval.setTo(finish);
        interval.setProcess(p);

        intervals.push_back(interval);




        if(runningProcessesQueue.empty())
        {
            if(!processesQueue.empty())
            {
                finish = processesQueue.head().getArrivalTime();
                runningProcessesQueue.enqueue(processesQueue.dequeue());
            }
        }
    }
}

float RRSched::waitingTime()
{
    float t = 0;
    for(int i = 0 ; i < processes.size() ; ++i)
    {
        float finish;
        float start = processes[i].getArrivalTime();
        for(int k = 0 ; k < intervals.size() ; ++k)
        {
            if(processes[i].getName() == intervals[k].getProcess().getName())
                finish = intervals[k].getTo();
        }
        t += finish - start - processes[i].getBurstTime();
    }
    return t/ processes.size();
}

RRSched::RRSched(QVector<SysProcess> processes, float qtime)
{
    this->setProcesses(processes);
    this->setQuantumTime(qtime);
}
