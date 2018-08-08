#ifndef __I_CRC_h__
#define __I_CRC_h__


#include "library\Component\Header\I_DataStruct.h"
typedef enum
{
    _16bit=2,
    _32bit=4
} enum_crckind;


typedef struct
{
    enum_crckind bitcount;
    uint32_t polynom;
    uint8_t crcresult[4];

} CRCParameter;

typedef struct
{

// length:length of data including crc value
// data: appended by empty crc value zero

bool (*crc)(uint32_t index,uint8_t* length,
               uint8_t  *data);

uint32_t(*ParameterInitial)(enum_crckind bitcount, uint32_t polynom);

} CRCDevice;
ListCollection* CRC_listcomp;



extern void CreateCRCDevice(CRCDevice* device,ListCollection* listcomp);

#endif
