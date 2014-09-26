#include <thingspeak.h>
#include <ts_http.h>


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


int32_t ts_datastream_update(ts_context_t* ctx, ts_feed_id_t feed_id, char * datastream_id, ts_datapoint_t *datapoint)
{
	char  num[128]    = {0};
	ssize_t n         = 0;
	ts_context_t* tsx = NULL;


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


	if(feed_id == 0)
	{
		n = ts_http_post(ctx, HOST_API, "/update", num);
	}
	else
	{
		tsx = ts_create_context(ctx->api_key, feed_id);
		if(tsx)
		{
			n = ts_http_post(ctx, HOST_API, "/update", num);
			ts_delete_context(tsx);
			return (n > 0) ? 0 : 1;
		}
		else
			return -1;
	}
	return 0;
}



char *ts_datastream_get(ts_context_t* ctx, ts_feed_id_t feed_id, ts_data_type_t type)
{
	char *ans          = NULL;
	uint32_t id        = 0;
	char  page[128] = {0};
//	ts_context_t* tsx  = NULL;
// http://api.thingspeak.com/channels/16258/feeds.json


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

	ans = ts_http_get(HOST_API, page);


	return ans;
}
