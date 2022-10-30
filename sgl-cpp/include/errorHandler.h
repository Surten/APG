#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include "sgl.h"

class ErrorHandler{
    static sglEErrorCode error;

    public:

    static void setError(sglEErrorCode e){
        if (error == SGL_NO_ERROR) error = e;
    }

    static sglEErrorCode getAndReset()
    {
        sglEErrorCode ret = error;
        error = SGL_NO_ERROR;
        return ret;
    }

};



#endif