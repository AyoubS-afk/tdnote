#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "i_circular_buffer_repository.h"
#include "circular_buffer_infra_file_service.h"
#include "i_circular_buffer.h"

#define MAX_RECORD 1024
#define INDEX_SUFFIX ".ndx"
#define DATA_SUFFIX ".rec"
#define FILE_DB_REPO "../Persistence/FileDB/CircularBuffer/CIRCULAR_BUFFER"

struct RECORD_CB
{
    unsigned long data_length;
    int offset_head;
    int offset_current;
    char data[]; //new in c99 !!
};

struct circular_buffer
{
    char *tail;
    unsigned long length;
    char *head;
    char *current;
    bool isFull;
};

struct index
{
    long recordStart;
    size_t recordLength;
};

static FILE *index_stream;
static FILE *data_stream;

int ICircularBufferRepository_save(circular_buffer cb)
{
    if (!ICircularBufferRepository_open(FILE_DB_REPO))
        return 0;
    ICircularBufferRepository_append(cb);

    return 1;
}

void ICircularBufferRepository_close(void)
{
    fclose(data_stream);
    fclose(index_stream);
}

static int Calculate_Offset_Head(circular_buffer cb)
{
    return cb->head - cb->tail;
}

static int Calculate_Offset_Current(circular_buffer cb)
{
    return cb->current - cb->tail;
}

static FILE *auxiliary_open(char *prefix, char *suffix)
{
    int prefix_length = strlen(prefix);
    int suffix_length = strlen(suffix);
    char filename[prefix_length+suffix_length+1];
    strncpy(filename,prefix,prefix_length);
    strncpy(filename +prefix_length, suffix,suffix_length+1);
    FILE *flow = fopen(filename,"r+");
    if(flow==NULL)
        flow=fopen(filename,"r+");
    if(flow==NULL);
        perror(filename);
		
    return flow;
	
}

int ICircularBufferRepository_open(char *name)
{
    data_stream = auxiliary_open(name, DATA_SUFFIX);
    if (data_stream == NULL)
    {
        fclose(data_stream);
        return 0;
    }
    index_stream = auxiliary_open(name, INDEX_SUFFIX);
    if (index_stream == NULL)
    {
        fclose(index_stream);
        return 0;
    }
    return 1;
}

int ICircularBufferRepository_append(circular_buffer cb)
{
    struct index index;
    /* Mise des données du Circular Buffer dans RECORD_CB*/
    int i=0;
    struct RECORD_CB RECORD_CB;
    RECORD_CB.data_length=CircularBuffer_get_buffer_length(cb);
    RECORD_CB.offset_head=Calculate_Offset_Head(cb);
    RECORD_CB.offset_current=Calculate_Offset_Current(cb);
    CircularBuffer_set_current_to_head(cb);
    for(i=0;i<=RECORD_CB.offset_head;i++)
    {
        RECORD_CB.data[i]=CircularBuffer_get_char_before_current(cb);
    }
	
    char myRecord[3+RECORD_CB.offset_head];
    myRecord[0]=RECORD_CB.data_length;
    myRecord[1]=RECORD_CB.offset_head;
    myRecord[2]=RECORD_CB.offset_current;
    for(i=0;i<=RECORD_CB.offset_head;i++)
    {
        myRecord[3+i]=RECORD_CB.data[i];
    }
    size_t length = sizeof(myRecord);
    fseek(data_stream, 0L, SEEK_END);
    index.recordStart = ftell(data_stream);
    index.recordLength = length;
    fwrite(myRecord, sizeof(myRecord), 1, data_stream);
    fseek(index_stream, 0L, SEEK_END);
    fwrite(&index, sizeof index, 1, index_stream);
    return 1;
	
}
