#
# Dictionary encoding 
#   by Adam Stallard and Aranya Ajith#
#
#  COP5725 Advanced Databases
#  Spring 2016
#
#
##########################################################

define HEADER

   ---------------------------------------------------------
   |                                                       |
   |    RDF Storage Compression Comparision Against HDT    |
   |                                                       |
   |          by Adam Stallard and Aranya Ajith            |
   |                                                       |
   ---------------------------------------------------------

   . . . building components . . .

endef

define FOOTER

  . . . components ready!! enjoy :)

endef

export HEADER FOOTER

##########################################################


CC = g++ -std=c++11
CCFLAGS = -Wall -pedantic
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
EXE := main


##########################################################


.SUFFIXES :
.PHONY : fresh partial_clean clean cls hdr ftr


##########################################################

all : fresh

$(EXE) : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<


##########################################################


fresh : clean cls hdr $(EXE) partial_clean ftr

partial_clean :
	@rm $(OBJS) 2>/dev/null || true

clean :
	@rm $(OBJS) $(EXE) 2>/dev/null || true

cls :
	@clear || true

hdr :
	@$(info $(HEADER))

ftr :
	@$(info $(FOOTER))


##########################################################