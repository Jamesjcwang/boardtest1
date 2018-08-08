#ifndef __I_DataStruct_h__
#define __I_DataStruct_h__

#include <stdbool.h>
#include <stdint.h>

typedef struct _ListStruct_uint8
{
   uint8_t index;
   uint32_t value;
   struct _ListStruct_uint8* next;

}ListStruct_uint8;

typedef struct
{
    uint32_t(*Insert)(ListStruct_uint8* head, ListStruct_uint8* data );
    bool(*GetFirst)(ListStruct_uint8* head,ListStruct_uint8* datagetted);
    bool(*RemoveAt)(ListStruct_uint8* head,uint8_t index);
    uint32_t(*GetAt)(ListStruct_uint8* head,uint8_t index);
    bool(*Clean)(ListStruct_uint8* head);
} ListCollection;


extern void CreateListMoudle(ListCollection* device);


#endif
