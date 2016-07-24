stem = pnumber

all: $(stem).l.cpp $(stem).y.cpp

$(stem).y.cpp: $(stem).y
	bison -d -o $@ $<

# The `sed` step is to make a change that suppresses a warning.
# This is supposed to be corrected in more recent versions of flex.
$(stem).l.cpp: $(stem).l
	flex --posix -s -I -o $@ $<
	sed -i 's/yy_size_t yy_buf_size;/int yy_buf_size;/' $@

clean:
	-rm $(stem).l.cpp
	-rm $(stem).y.hpp
	-rm $(stem).y.cpp
	-rm $(stem).l.o
	-rm $(stem).l.d
	-rm $(stem).y.o
	-rm $(stem).y.d
	-rm $(stem)

# ===============================================================
# Only used to development / debugging
# ===============================================================
$(stem): $(stem).l.o $(stem).y.o
	g++ -g -o $@ $^

$(stem).yy.o: $(stem).l.cpp $(stem).y.cpp
	g++ -g -c $<

$(stem).tab.o: $(stem).y.cpp
	g++ -g -c $<

.PHONY: all clean
