#include <thingspeak.h>
#include <ts_http.h>
#include <ts_time.h>


ts_context_t *ts_create_context(char *api_key, ts_feed_id_t feed_id)
{
    ts_context_t *ctx = NULL;

    if(api_key != NULL)
    {
        ctx = (ts_context_t*)malloc(sizeof(ts_context_t));
        bzero(ctx->api_key, 16);
        strcpy(ctx->api_key, api_key);
        ctx->api_key[16] = '\0';
        ctx->feed_id = feed_id;
    }
    return ctx;
}


void ts_delete_context(ts_context_t *ctx)
{
    if(ctx)
        free(ctx);
}



ts_datapoint_t *ts_set_value_i32(ts_datapoint_t *datapoint, int32_t val)
{
    datapoint->value_type = TS_VALUE_TYPE_I32;
    datapoint->value.i32_value = val;
    return datapoint;
}


int32_t ts_get_value_i32(ts_datapoint_t *datapoint)
{
    return datapoint->value.i32_value;
}


ts_datapoint_t *ts_set_value_f32(ts_datapoint_t *datapoint, float val)
{
    datapoint->value_type = TS_VALUE_TYPE_F32;
    datapoint->value.f32_value = val;
    return datapoint;
}

float ts_get_value_f32(ts_datapoint_t *datapoint)
{
    return datapoint->value.f32_value;
}


int32_t ts_datastream_update(ts_context_t* ctx, ts_feed_id_t feed_id, char * datastream_id,
                                                                      ts_datapoint_t *datapoint)
{
    char  num[200]    = {0};
    ssize_t n         = 0;
    ts_context_t *tsx = NULL;
    ts_tm_t *timeinfo;

    if(datapoint->timestamp <= 0)
        timeinfo = ts_settime(datapoint, TS_TIME_LOCAL);
        
    switch(datapoint->value_type)
    {
        case TS_VALUE_TYPE_I32:
            sprintf(num, "%s=%d", datastream_id, datapoint->value.i32_value);
            break;
        case TS_VALUE_TYPE_F32:
            sprintf(num, "%s=%f", datastream_id, datapoint->value.f32_value);
            break;
        case TS_VALUE_TYPE_STR:
            sprintf(num, "%s=%s", datastream_id, datapoint->value.str_value);
            break;
    }
    
    sprintf(num, "%s&created_at=%d-%d-%d %d:%d:%d", num, (1900/*unix time start*/+timeinfo->tm_year),
                                                                                (timeinfo->tm_mon+1),
                                                                                   timeinfo->tm_mday,
                                                                                   timeinfo->tm_hour,
                                                                                    timeinfo->tm_min, 
                                                                                   timeinfo->tm_sec);
#if TS_DEBUG                                                                                    
    printf("%s\n", num);
#endif


    if(feed_id == 0)
        n = ts_http_post(ctx, HOST_API, "/update", num);
    else
    {
        tsx = ts_create_context(ctx->api_key, feed_id);
        if(tsx)
        {
            n = ts_http_post(ctx, HOST_API, "/update", num);
            ts_delete_context(tsx);
        }
        else
            return -1;
    }
    return (n > 0) ? 0 : 1;
}



char *ts_datastream_get(ts_context_t *ctx, ts_feed_id_t feed_id,
                                           ts_data_type_t type,
                                           char *datastream_id,
                                           char *result)
{
    char *ans       = NULL;
    uint32_t id     = 0;
    char  page[128] = {0};
    char  d_id[2]   = {0}; 

    id = (feed_id == 0) ? ctx->feed_id : feed_id;
    sscanf(datastream_id, "field%s", d_id);

    switch(type)
    {
        case TS_DATA_CSV:
            sprintf(page, "/channels/%d/feeds/%s.csv", id, d_id);
            break;
        case TS_DATA_XML:
            sprintf(page, "/channels/%d/feeds/%s.xml", id, d_id);
            break;
        case TS_DATA_JSON:
            sprintf(page, "/channels/%d/feeds/%s.json", id, d_id);
            break;
    }

#if TS_DEBUG
    printf("%s\n", page);
#endif

    ans = ts_http_get(HOST_API, page, result);

    return ans;
}


char *ts_feed_get_all(ts_context_t* ctx, ts_feed_id_t feed_id,
                                         ts_data_type_t type,
                                         char *result)
{
    char *ans       = NULL;
    uint32_t id     = 0;
    char  page[128] = {0};


    id = (feed_id == 0) ? ctx->feed_id : feed_id;

    switch(type)
    {
        case TS_DATA_CSV:
            sprintf(page, "/channels/%d/feeds.csv", id);
            break;
        case TS_DATA_XML:
            sprintf(page, "/channels/%d/feeds.xml", id);
            break;
        case TS_DATA_JSON:
            sprintf(page, "/channels/%d/feeds.json", id);
            break;
    }

#if TS_DEBUG
    printf("%s\n", page);
#endif    

    ans = ts_http_get(HOST_API, page, result);


    return ans;
}
