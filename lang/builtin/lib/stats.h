#ifndef STATS_H
#define STATS_H

#include "build.h"

extern const ModuleFuncInfo STATS_MODULE[];
extern const int STATS_MODULE_COUNT;

typedef enum
{
    STATS_MEAN,
    STATS_MEDIAN,
    STATS_MODE,
    STATS_VARIANCE,
    STATS_STDEV,
    STATS_CORRELATION,
    STATS_MIN,
    STATS_MAX,
    STATS_RANGE,
    STATS_SUM,
    STATS_COUNT,
    STATS_PERCENTILE,
    STATS_ZSCORE,
} StatsType;

void initializeStatsModule(Runtime *mod_rt);
Data *dispatchStatsBuiltin(StatsType type, List *args, Dict *kwargs, Runtime *rt);

#endif