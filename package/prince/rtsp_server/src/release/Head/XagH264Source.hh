/*
 * TIAM335X_H264_H_.h
 *
 *  Created on: 2016年11月1日
 *      Author: Stone
 */
#ifndef TIAM335X_H264_H_
#define TIAM335X_H264_H_

#include "FramedSource.hh"
#include <iostream>
#include <sys/time.h>

#define FRAME_PER_SEC 30  
class XagH264Source: public FramedSource
{

public:


static XagH264Source*  createNew(UsageEnvironment& env) 
{
      return new XagH264Source(env);
}

	XagH264Source(UsageEnvironment& env);
	virtual ~XagH264Source();

  static void updateTime(struct timeval& p);

  void doUpdateStart();
  static void updateDataNotify(void* d){((XagH264Source*)d)->doUpdateDataNotify();};
  void doUpdateDataNotify();

protected: // redefined virtual functions
  virtual void doGetNextFrame();
//   static void getNextFrame(void * ptr);
  virtual void doStopGettingFrames();

  virtual unsigned int maxFrameSize() const; 
 void GetFrameData();  
 
 void *m_pToken; 



  static struct timeval sPresentationTime;
  static struct timeval sdiff;

  static bool sbTimeUpdate;

	EventTriggerId m_eventTriggerId;

  bool bVideoFirst;

  bool m_can_get_nextframe;
  bool m_is_queue_empty;
  bool m_started;

};

#endif /* TIAM335X_H264_H_ */
