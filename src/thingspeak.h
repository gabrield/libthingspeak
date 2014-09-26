#ifndef _THINGSPEAK_H_
#define _THINGSPEAK_H_

#include <stdlib.h>
#include <stdint.h>

#define TS_MAX_DATASTREAMS       16
#define TS_MAX_DATASTREAM_NAME   16
#define TS_VALUE_STRING_MAX_SIZE 16

typedef uint32_t ts_feed_id_t;


/** Datapoint value types */
typedef enum
{   /** 32-bit signed integer */
    TS_VALUE_TYPE_I32 = 0 ,
    /** 32-bit floating point number */
    TS_VALUE_TYPE_F32,
    /** any string-econded data */
    TS_VALUE_TYPE_STR
} ts_value_type_t;


/** Response value types */
typedef enum
{   
    TS_DATA_CSV = 0,
    TS_DATA_XML,
    TS_DATA_JSON
} ts_data_type_t;



typedef union {
    int32_t i32_value;
    float   f32_value;
    char    str_value[TS_VALUE_STRING_MAX_SIZE];
} ts_datapoint_value_t;



typedef struct
{
    ts_datapoint_value_t  value;
    ts_value_type_t       value_type;
    /*ts_timestamp_t        timestamp;*/
} ts_datapoint_t;

typedef struct
{
    char              datastream_id[TS_MAX_DATASTREAM_NAME];
    ts_datapoint_t    datapoint;
} ts_datastream_t;



typedef struct
{
    ts_feed_id_t      feed_id;
    ts_datastream_t   datastreams[TS_MAX_DATASTREAMS];
} ts_feed_t;



typedef struct 
{
    char api_key[16 + 1];              /** ThingSpeak API key */
    ts_feed_id_t feed_id;       /** ThingSpeak feed ID */
} ts_context_t;


typedef struct
{
   uint32_t http_retcode;
} ts_response_t;


ts_context_t *ts_create_context(char *, ts_feed_id_t);

void ts_delete_context(ts_context_t *);


ts_datapoint_t *ts_set_value_i32(ts_datapoint_t *, int32_t);

int32_t ts_get_value_i32(ts_datapoint_t *);


ts_datapoint_t *ts_set_value_f32(ts_datapoint_t *, float);

float ts_get_value_f32(ts_datapoint_t *);


int32_t ts_datastream_update(ts_context_t*, ts_feed_id_t, char *, ts_datapoint_t *);

char *ts_datastream_get(ts_context_t *, ts_feed_id_t,  ts_data_type_t type);

#endif