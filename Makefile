CC = gcc
CFLAGS = -Wall -Wextra

SRCDIR = src
OBJDIR = build
COREDIR = $(SRCDIR)/core
LOCALDIR = $(SRCDIR)/local
NETDIR = $(SRCDIR)/net
PARSEDIR = $(SRCDIR)/parse
STARTPAGEDIR = $(SRCDIR)/common

CLIENT_SRC = $(SRCDIR)/client.c
MAIN_SRC = $(SRCDIR)/server.c
LOC_CLIENT_SRC = $(LOCALDIR)/local_client.c
IFS_SRC = $(COREDIR)/ifs.c
NET_SRC = $(NETDIR)/net.c
PARSE_SRC = $(PARSEDIR)/parse.c
STARTPAGE_SRC = $(STARTPAGEDIR)/start_page.c

CLIENT_OBJ = $(OBJDIR)/client.o
LOC_CLIENT_OBJ = $(OBJDIR)/local_client.o
IFS_OBJ = $(OBJDIR)/ifs.o
MAIN_OBJ = $(OBJDIR)/server.o
NET_OBJ = $(OBJDIR)/net.o
PARSE_OBJ = $(OBJDIR)/parse.o
STARTPAGE_OBJ = $(OBJDIR)/start_page.o

TARGET = ifsServer

CLI=ifsClient

.PHONY: all clean

all: $(TARGET) $(CLI)

$(TARGET): $(LOC_CLIENT_OBJ) $(IFS_OBJ) $(MAIN_OBJ) $(NET_OBJ) $(PARSE_OBJ) $(STARTPAGE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(CLI): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(LOC_CLIENT_OBJ): $(LOC_CLIENT_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(IFS_OBJ): $(IFS_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NET_OBJ): $(NET_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSE_OBJ): $(PARSE_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(STARTPAGE_OBJ): $(STARTPAGE_SRC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o 