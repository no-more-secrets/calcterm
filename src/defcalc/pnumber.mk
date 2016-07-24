stem = pnumber

all: $(stem).yy.cpp $(stem).tab.cpp

$(stem).tab.cpp: $(stem).y
	bison -d -o $@ $<

# The `sed` step is to make a change that suppresses a warning.
# This is supposed to be corrected in more recent versions of flex.
$(stem).yy.cpp: $(stem).l
	flex --posix -s -I -o $@ $<
	sed -i 's/yy_size_t yy_buf_size;/int yy_buf_size;/' $@

clean:
	-rm $(stem).yy.cpp
	-rm $(stem).tab.hpp
	-rm $(stem).tab.cpp
	-rm $(stem).yy.o
	-rm $(stem).yy.d
	-rm $(stem).tab.o
	-rm $(stem).tab.d
	-rm $(stem)

# ===============================================================
# Only used to development / debugging
# ===============================================================
$(stem): $(stem).yy.o $(stem).tab.o
	g++ -g -o $@ $^

$(stem).yy.o: $(stem).yy.cpp $(stem).tab.cpp
	g++ -g -c $<

$(stem).tab.o: $(stem).tab.cpp
	g++ -g -c $<

.PHONY: all clean
