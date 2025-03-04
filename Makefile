# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Executable Names that'll popup after make(ing) the file.
TARGET_RR = RR_Final
TARGET_SJF = SJF_Final

# Targets to build the executtion files with.
all: $(TARGET_RR) $(TARGET_SJF)

# If the c file has been changed or the target doesn't exist then the command
#  gcc -Wall -Wextra -o RR_Final RR_initial.c is executed.
$(TARGET_RR): RR_inital.c
	$(CC) $(CFLAGS) -o $(TARGET_RR) RR_inital.c

# If the c file has been changed or the target doesn't exist then the command
# gcc -Wall -Wextra -o SJF_Final SJF_intial.c is executed
$(TARGET_SJF): SJF_inital.c
	$(CC) $(CFLAGS) -o $(TARGET_SJF) SJF_inital.c

# Cleaning the executables out by removing them
clean:
	rm -f $(TARGET_RR) $(TARGET_SJF)