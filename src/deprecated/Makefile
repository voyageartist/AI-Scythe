
CC           :=  g++
FLAGS        :=  -std=c++14 -Iinclude -fPIC
CFLAGS       :=  -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wconversion -pthread 
CFLAGS       +=  -O3 -ftree-loop-optimize -ftree-vectorize -funroll-loops -ftree-vectorizer-verbose=1
CPPFLAGS     :=  -g -fopenmp -fopenmp-simd
LDFLAGS      :=  -L. -shared
DEBUGFLAGS   :=  -O0 -D _DEBUG
RELEASEFLAGS :=  -O3 -D NDEBUG -combine -fwhole-program
NAME         :=  scythe

ifeq ($(OS),Windows_NT)
	TARGET = $(NAME).dll
else
	TARGET = $(NAME).so
endif

OBJS := deep_scythe.o scythe.o cart.o forest.o
OBJS += classification_gb.o regression_gb.o
OBJS += classification_metrics.o regression_metrics.o
OBJS += classification_rf.o regression_rf.o
OBJS += classification_complete_rf.o regression_complete_rf.o
OBJS += bagging.o heuristics.o sets.o layer.o concatenation_layer.o
OBJS += deep_forest.o scanner1D.o scanner2D.o scanner3D.o
OBJS += grayscale.o continuous.o proba.o

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJS)
	@rm -rf $(OBJS) # clean

cart.o : tree/cart.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

forest.o : forest/forest.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

heuristics.o : tree/heuristics.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

continuous.o : densities/continuous.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

grayscale.o : densities/grayscale.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

proba.o : densities/proba.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

sets.o : misc/sets.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

bagging.o : misc/bagging.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

regression_metrics.o: metrics/regression_metrics.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

regression_gb.o: forest/regression_gb.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

regression_rf.o: forest/regression_rf.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

regression_complete_rf.o: forest/regression_complete_rf.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

classification_metrics.o: metrics/classification_metrics.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

classification_gb.o: forest/classification_gb.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

classification_rf.o: forest/classification_rf.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

classification_complete_rf.o: forest/classification_complete_rf.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

layer.o: deep_learning/layers/layer.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

concatenation_layer.o: deep_learning/layers/concatenation_layer.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

scanner1D.o: deep_learning/layers/scanner1D.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

scanner2D.o: deep_learning/layers/scanner2D.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

scanner3D.o: deep_learning/layers/scanner3D.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

deep_forest.o: deep_learning/deep_forest.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<

%.o: %.cpp
	$(CC) $(FLAGS) $(CFLAGS) $(CPPFLAGS) -c $<