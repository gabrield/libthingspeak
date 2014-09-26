all:
	@make -C src all

debug:
	@make -C src DEBUG=-DTS_DEBUG=1 all

clean:
	@make -C src clean
