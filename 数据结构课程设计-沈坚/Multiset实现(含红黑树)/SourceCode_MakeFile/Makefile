tj_multiset: tj_multiset_main.o tj_multiset.o tj_student.o
	g++ -o tj_multiset tj_multiset_main.o tj_multiset.o tj_student.o
tj_multiset_main.o: tj_multiset_main.cpp
	g++ -std=c++11 -c tj_multiset_main.cpp
tj_multiset.o: tj_multiset.cpp
	g++ -std=c++11 -c tj_multiset.cpp
tj_student.o: tj_student.cpp
	g++ -std=c++11 -c tj_student.cpp

clean:
	rm *.o
	rm tj_multiset
