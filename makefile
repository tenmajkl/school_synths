NAME=synths
AUTHOR=KripacMichal

SRC=src
OBJ=obj
ZIP=$(AUTHOR)-$(NAME).zip
OBJFILES=actions.o comparing.o fields.o helpers.o lists.o main.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Wextra -g


.PHONY: all dep run clean submit push

all: $(NAME)

$(OBJ):
	mkdir $(OBJ)

%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $(OBJ)/$@

dep:
	$(CC) -MM $(SRC)/*.c >dep.list

-include dep.list

$(NAME): $(OBJ) $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES:%.o=$(OBJ)/%.o) -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(ZIP)
	rm -rf $(OBJ)
	rm -f $(NAME) *.o

submit: clean
	zip $(ZIP) *

push: clean
	git add .
	git commit -m $(M)
	git push


