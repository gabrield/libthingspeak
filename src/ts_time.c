#include <ts_time.h>

ts_tm_t *ts_settime(ts_datapoint_t *datapoint, ts_time_type_t type)
{
    ts_tm_t *timeinfo = NULL;
    time((time_t*)&datapoint->timestamp);
    
    switch(type)
    {
        case TS_TIME_LOCAL:
            timeinfo = (ts_tm_t*)localtime((time_t*)&datapoint->timestamp);
            break;
        case TS_TIME_GMT:
            timeinfo = (ts_tm_t*)gmtime((time_t*)&datapoint->timestamp);
            break;
    }
    
    return timeinfo;
}


void ts_mktime(ts_datapoint_t *datapoint, int year, int month, int mday, int hour,
                                                                         int min,
                                                                         int sec)
{
    ts_tm_t *timeinfo =  NULL;
    
    time((time_t*)&datapoint->timestamp);
    timeinfo = localtime((time_t*)&datapoint->timestamp);
    
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = mday,
    timeinfo->tm_hour = hour;
    timeinfo->tm_min  = min; 
    timeinfo->tm_sec  = sec;
    
    datapoint->timestamp = mktime (timeinfo);
}
