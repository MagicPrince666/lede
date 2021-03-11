/*
 * V4L2.cpp
 *
 *  Created on: 2016年11月1日
 *      Author: Stone
 */

#include "XagH264Source.hh"
#include "FetchData.hh"
//#include "cbuf.h"

# define __DBGFUNS
# ifdef __DBGFUNS
# define DBGFUNS(fmt,args...) printf(fmt,  ##args)
# else
# define DBGFUNS(fmt,args...)
# endif
#if 0
#endif

FILE* rec_file = NULL;

bool emptyBufferFlag = true;

// extern cbuf_t g_cbuf;
XagH264Source::XagH264Source(UsageEnvironment& env):
  FramedSource(env),m_pToken(0)
{
	m_can_get_nextframe = true;
	m_is_queue_empty =false;
	bVideoFirst = true;
	m_started = false;
	fMaxSize = maxFrameSize();
//   cameraInit();
#if 0
  char rec_filename[] = "264_1.264";
  if(rec_file == NULL)
  	rec_file = fopen(rec_filename, "wb");
	printf("XagH264Source:: XagH264Source() 1\n");
#endif
	//RingBuffer rb(order, YieldWaitConsumerStrategy());
	gettimeofday(&sPresentationTime, NULL);

	//启动获取视频数据线程
	FetchData::startCap();
	emptyBufferFlag = true;
	FetchData::setSource(this);
	m_eventTriggerId = envir().taskScheduler().createEventTrigger(XagH264Source::updateDataNotify);
}

XagH264Source::~XagH264Source()
{
	printf("H264Source::~H264Source() tid:%d  \n",pthread_self());

	if(rec_file != NULL)
		fclose(rec_file);
	printf("H264Source::~H264Source() 1-3\n");
		
	rec_file = NULL;
	printf("H264Source::~H264Source() 1-4\n");
	FetchData::stopCap();
	printf("H264Source::~H264Source() 2\n");
	
	envir().taskScheduler().deleteEventTrigger(m_eventTriggerId);
	// envir().taskScheduler().unscheduleDelayedTask(m_pToken);  

}

int timeval_substract(struct timeval* result, struct timeval*t2, struct timeval* t1)
{
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);

	result->tv_sec = diff/1000000;
	result->tv_usec = diff % 1000000;

	return diff<0;
}


void  timeval_add(struct timeval* result, struct timeval*t2, struct timeval* t1)
{
	long int total = (t2->tv_usec + 1000000 * t2->tv_sec) + (t1->tv_usec + 1000000 * t1->tv_sec);

	result->tv_sec = total/1000000;
	result->tv_usec = total % 1000000;
}

struct timeval XagH264Source::sPresentationTime;
struct timeval XagH264Source::sdiff;
bool XagH264Source::sbTimeUpdate =false;
void XagH264Source::updateTime(struct timeval& p)
{
	struct timeval now;
	gettimeofday(&now, NULL);
    sPresentationTime.tv_sec = p.tv_sec;
	sPresentationTime.tv_usec = p.tv_usec;

	int i = timeval_substract(&sdiff, &now, &sPresentationTime);
	printf("DIFF:%d \n",sdiff);

    sbTimeUpdate = true;
}

void XagH264Source::doUpdateStart()
{
	envir().taskScheduler().triggerEvent(m_eventTriggerId, this);
}


void XagH264Source::doUpdateDataNotify()
{
	// nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting,this);  
	afterGetting(this);
}

// bool bVideoFirst = true;
// char _buf[204800];
void XagH264Source::GetFrameData()
{
	
	// printf("H264Source::GetFrameData 1 m_can_get_nextframe:%d tid:%d\n",m_can_get_nextframe,pthread_self());
	unsigned len = FetchData::getData(fTo,fMaxSize, fFrameSize, fNumTruncatedBytes);
	// printf("H264Source::GetFrameData 2\n");
	
	gettimeofday(&fPresentationTime, NULL);
	afterGetting(this);

	if(!m_can_get_nextframe)
	{
		envir().taskScheduler().unscheduleDelayedTask(nextTask());
		// DBGFUNS("H264Source m_is_queue_empty=true tid:%d\n",pthread_self());
		
		m_is_queue_empty=true;
	}
	
			
} 
 struct timeval m_start, m_end;

void XagH264Source::doGetNextFrame()
{
	if(!m_started)
	{
		m_started =true;
	}
	
	GetFrameData();
	
}


void XagH264Source::doStopGettingFrames()
{
	DBGFUNS("H264Source STOP FRAME 1  tid:%d\n",pthread_self());
	//启动获取视频数据线程
	// FetchData::stopCap();
	// emptyBufferFlag = true;
	m_can_get_nextframe = false;

	while(!m_is_queue_empty && m_started)
	{
		// DBGFUNS("H264Source STOP FRAME 2  tid:%d\n",pthread_self());
		usleep(10000);
	}

	DBGFUNS("H264Source STOP FRAME 2\n");
}


//网络包尺寸，注意尺寸不能太小，否则会崩溃
unsigned int XagH264Source::maxFrameSize() const
{
	printf("H264Source::maxFrameSize 150000\n");
	return 150000;
	//return 100*1024;
}
