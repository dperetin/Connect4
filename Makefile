UNAME := $(shell uname)
test : main.c 
	@if [ "$(UNAME)" =  "Darwin" ]; then\
		gcc main.c -o CONNECT_4 -framework GLUT -framework OpenGL;\
	elif [ "$(UNAME)" =  "Linux" ]; then\
		gcc main.c -lGL -lglut -lm -o CONNECT_4;\
	else \
		echo "Get yourself a real os.";\
	fi
