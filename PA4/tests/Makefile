# Testing
SCRIPTCW	= ./cwruntests
SCRIPTCCW	= ./ccwruntests
OUT     = ./out/*

.PHONY: testcw testccw clean clobber
# test the program
test:   ccwtest cwtest
ccwtest:
	echo Running Public Tests
	$(SCRIPTCCW)

cwtest:
	echo Running Public Tests
	$(SCRIPTCW)

# remove output files
clobber: clean

clean:
	rm -f $(OUT)
