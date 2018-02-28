SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
SHFLAGS :=  -fPIC
DEPENDS = $(SRC:.cpp=.d)

CC  := icpc -O3 -qopenmp -qoverride-limits -g -Wall -std=c++14 -Wextra \
	  -pedantic -xHOST -no-inline-max-size -no-inline-max-total-size \
		-DDEBUG
LINK = $(CC)
DEPFLAGS := -M
OUT := md

INC = 
LIB = -lmatheval

.PHONY : clean

$(OUT) : $(OBJ)
	$(LINK) $(OBJ) $(LIB) -o $@

%.o : %.cpp
	@/bin/rm -f $(OUT)
	$(CC) $(SHFLAGS) $(INC) -c $< -o $@

%.d : %.cpp
	@set -e; /bin/rm -f $@; \
         $(CC) $(DEPFLAGS) $(INC) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

clean :
	@/bin/rm -f $(OUT)
	@/bin/rm -f $(OBJ)
	@/bin/rm -f $(DEPENDS)

ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPENDS)
endif
