SERVER_DIR=server
KLIENT_DIR=klient
COMMON_DIR=common

OUTPUT_SERVER=server_prog
OUTPUT_KLIENT=klient_prog

MAKE_ARGS=--no-print-directory

CC=g++
CC_ARGS=-pthread
CC_LIBS=-L./common -lcommon

all: $(OUTPUT_SERVER) $(OUTPUT_KLIENT)

compile: cmn srv cl

cmn:
	cd $(COMMON_DIR) && $(MAKE) $(MAKE_ARGS)

srv: cmn
	cd $(SERVER_DIR) && $(MAKE) $(MAKE_ARGS)
	
cl: cmn
	cd $(KLIENT_DIR) && $(MAKE) $(MAKE_ARGS)

$(OUTPUT_SERVER): compile
	$(CC) $(CC_ARGS) $(SERVER_DIR)/*.o $(CC_LIBS) -o$(OUTPUT_SERVER)
	
$(OUTPUT_KLIENT): compile
	$(CC) $(CC_ARGS) $(KLIENT_DIR)/*.o $(CC_LIBS) -o$(OUTPUT_KLIENT)

clean:
	cd $(SERVER_DIR) && $(MAKE) clean $(MAKE_ARGS)
	cd $(KLIENT_DIR) && $(MAKE) clean $(MAKE_ARGS)
	cd $(COMMON_DIR) && $(MAKE) clean $(MAKE_ARGS)
	rm -f $(OUTPUT_SERVER) $(OUTPUT_KLIENT)
