CFLAGS+=-Wall
CFLAGS+=-Werror
CFLAGS+=-O2
CFLAGS+=-std=c99
CFLAGS+=-pedantic
all:binaryentropy

plot:all
	./binaryentropy $(IMAGE) > entropy_of_$(shell basename $(IMAGE)).gnuplot
	gnuplot --persist -e "plot \"entropy_of_$(shell basename $(IMAGE)).gnuplot\" with lines linestyle 1"
