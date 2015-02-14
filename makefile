RAYOBJS = main.o material.o object.o veclib.o plane.o light.o projection.o \
  list.o model.o sphere.o image.o raytrace.o malloc.o pplane.o illuminate.o \
  fplane.o tplane.o spotlight.o psphere.o disk.o cylander.o perlin.o \
  texsphere.o ppmread.o texplane.o texture.o triangle.o util.o cone.o torus.o \
  dispsphere.o

INCLUDE = ray.h material.h object.h veclib.h plane.h light.h projection.h \
  list.h model.h sphere.h image.h raytrace.h malloc.h pplane.h illuminate.h \
  fplane.h tplane.h spotlight.h psphere.h disk.h cylander.h perlin.h \
  texsphere.h ppmread.h texplane.h texture.h triangle.h util.h cone.h torus.h \
  dispsphere.h

# CFLAGS = -DAA_SAMPLES=12

ray: $(RAYOBJS)
#	gcc -Wall -o ray -g $(RAYOBJS) -lm
	gcc -Wall -Wextra -o ray -g $(RAYOBJS) -lm
#clang -Wall -o ray -g $(RAYOBJS) -lm
$(RAYOBJS):$(INCLUDE)

#DEBUG = -DDBG_AA
#DEBUG = -DDBG_PIX -DDBG_HIT -DDBG_WORLD -DDBG_AMB -DDBG_FND
#DEBUG = -DDBG_PIX #-DDBG_HIT -DDBG_FND
#DEBUG = -DDBG_TRANS
#DEBUG = -DDBG_REFRAC
DEBUG = -DDBG_FIND -DDBG_REFRAC
#DEBUG = -DDBG_TEXMAP
.c.o: $<
	- gcc -c -Wall $(CFLAGS) $(DEBUG) -g $< 
#	cat $*.err

clean:
	rm *.o
	#rm *.err
	rm debug
	#rm *.ppm

