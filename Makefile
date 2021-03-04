SUBDIRS = arbitro cliente jogos

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean: 
	rm out/*
	rm ./jogos/g_*.o
	rm ./PIPE*