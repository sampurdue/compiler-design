all: group compiler

group:
        echo "Sambit Mishra : mishra22 , Gurleen Kaur : kaur46 "

compiler: helloworld

helloworld: helloworld.c
        $(CC) -o $@ $<

clean:
        rm helloworld

