#ifndef __XAGRTSPCLIENT_H_
#define __XAGRTSPCLIENT_H_

#include <pthread.h>
#include "ringbuffer.h"
//#define UVC_CAMERA

class XagRtsp
{
public:
    static pthread_t rtsp_threadId;
    static bool rtsp_ison;
    static bool video_repo;
    static RingBuffer aoa_ring;
    static cycle_buffer* aoa_buffer;
    static uint64_t live_cnt;
    static void * rtsp_thead (void *arg);

private:

};

#endif