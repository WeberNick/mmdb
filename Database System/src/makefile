#REMEMBER TO ADD PATH TO BOOST IN ORDER TO USE THE GNUPLOT HEADER FILE
#ADD PATH TO BOOST IN /main/makefile and /infra/webe/makefile

DIRECTORIES = \
	infra/moer \
	infra/webe \
	memory \
	app \
	nsm \
	pax \
	relation \
	queries \
	main \
	\


all:
	for dir in $(DIRECTORIES); do \
		(cd $$dir; $(MAKE) all); \
	done

clean:
	for dir in $(DIRECTORIES); do \
		(cd $$dir; $(MAKE) clean); \
	done