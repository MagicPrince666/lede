#CROSS_COMPILE := 
#CXX := $(CROSS_COMPILE)g++
#CC := $(CROSS_COMPILE)gcc
TARGET	= video_proxy

livedir := live/
livemedia := $(livedir)liveMedia
usageenviroment := $(livedir)UsageEnvironment
basicusage := $(livedir)BasicUsageEnvironment
groupsock := $(livedir)groupsock
testProgs := $(livedir)testProgs

LIBDIR = -L$(usageenviroment) -L$(basicusage) -L$(livemedia) -L$(groupsock)

DIR		= ./src ./transfer
INC		= -I./include -I./transfer -I$(usageenviroment)/include -I$(basicusage)/include -I$(livemedia)/include -I$(groupsock)/include
CFLAGS	= -std=c++11 -Wall

OBJPATH	= .

LDFLAGS := $(LIBDIR) -lliveMedia -lBasicUsageEnvironment -lgroupsock -lUsageEnvironment -lpthread -ldl -lpcap

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))
CFILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.c))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))
COBJS	= $(patsubst %.c,%.o,$(CFILES))

all:
	cd $(livedir) ; ./genMakefiles mips
	cd $(livemedia) ; $(MAKE) 
	cd $(groupsock) ; $(MAKE) 
	cd $(usageenviroment) ; $(MAKE) 
	cd $(basicusage) ; $(MAKE)
	cd . ; $(MAKE) $(OBJS) $(COBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(COBJS):%.o:%.c
	$(CC) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET):$(OBJPATH)
	$(CXX) -o $@ $(OBJPATH)/*.o $(LDFLAGS)

clean:
	-rm $(OBJPATH)/*.o
	-rm $(TARGET)