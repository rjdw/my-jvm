CC = clang-with-asan
CFLAGS = -Wall -Wextra -fno-sanitize=integer
TESTS_1 = OnePlusTwo
TESTS_2 = $(TESTS_1) PrintOnePlusTwo
TESTS_3 = $(TESTS_2) Constants Part3
TESTS_4 = $(TESTS_3) Part4
TESTS_5 = $(TESTS_4) Locals MoreLocals
TESTS_6 = $(TESTS_5) PrintLargeNumbers
TESTS_7 = $(TESTS_6) Collatz PythagoreanTriplet
TESTS_8 = $(TESTS_7) Arithmetic CoinSums DigitPermutations FunctionCall \
	Goldbach IntegerTypes Jumps PalindromeProduct Primes Recursion

test: test8
test1: $(addprefix tests/,$(TESTS_1:=-result.txt))
test2: $(addprefix tests/,$(TESTS_2:=-result.txt))
test3: $(addprefix tests/,$(TESTS_3:=-result.txt))
test4: $(addprefix tests/,$(TESTS_4:=-result.txt))
test5: $(addprefix tests/,$(TESTS_5:=-result.txt))
test6: $(addprefix tests/,$(TESTS_6:=-result.txt))
test7: $(addprefix tests/,$(TESTS_7:=-result.txt))
test8: $(addprefix tests/,$(TESTS_8:=-result.txt))

jvm: jvm.o read_class.o
	$(CC) $(CFLAGS) $^ -o $@

tests/%.class: tests/%.java
	javac $^

tests/%-expected.txt: tests/%.class
	java -cp tests $(*F) > $@

tests/%-actual.txt: tests/%.class jvm
	./jvm $< > $@

tests/%-result.txt: tests/%-expected.txt tests/%-actual.txt
	diff -u $^ | tee $@; \
	name='test $(@F:-result.txt=)'; \
	if [ -s $@ ]; then echo FAILED $$name. Aborting.; false; \
	else echo PASSED $$name.; fi

clean:
	rm -f *.o jvm tests/*.txt `find tests -name '*.java' | sed 's/java/class/'`

.PRECIOUS: %.o tests/%.class tests/%-expected.txt tests/%-actual.txt tests/%-result.txt
