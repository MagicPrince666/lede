#!/bin/sh

CheckProcess()
{
  if [ "$1" = "" ];
  then
    return 1
  fi
 
  PROCESS_NUM=`ps | grep "$1" | grep -v "grep" | wc -l` 
  if [ $PROCESS_NUM -eq 1 ];
  then
    return 0
  else
    return 1
  fi
}

while [ 1 ] ; do
 CheckProcess "rtsp_server"
 CheckQQ_RET=$?
 if [ $CheckQQ_RET -eq 1 ];
 then
  killall -9 rtsp_server
  rtsp_server /dev/video0 30 2000 554 &
 fi

 sleep 1

done
