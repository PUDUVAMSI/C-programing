#ifndef TAGS_H
#define TAGS_H

/* User defined types */
typedef unsigned int uint;
/* Status will be used in fn. return type */
typedef enum
{
    mp_success,
    mp_failure
} Status;

typedef enum
{
    mp_view,
    mp_edit,
    mp_unsupported
} OperationType;

#endif
