GTEST_DIR=../../googletest-master/googletest

test: test.c++ utils.cpp
	g++ -isystem ${GTEST_DIR}/include -pthread test.c++ utils.cpp libgtest.a -o unittest
	./unittest

csv:
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=0 | grep COM28 | bash > ../vlcSyncFiles/auto_delay0ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=50 | grep COM28 | bash > ../vlcSyncFiles/auto_delay50ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=100 | grep COM28 | bash > ../vlcSyncFiles/auto_delay100ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=150 | grep COM28 | bash > ../vlcSyncFiles/auto_delay150ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=150 | grep COM28 | bash > ../vlcSyncFiles/AAA_BEST_auto_delay150ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=200 | grep COM28 | bash > ../vlcSyncFiles/auto_delay200ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=250 | grep COM28 | bash > ../vlcSyncFiles/auto_delay250ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=300 | grep COM28 | bash > ../vlcSyncFiles/auto_delay300ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=350 | grep COM28 | bash > ../vlcSyncFiles/auto_delay350ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=400 | grep COM28 | bash > ../vlcSyncFiles/auto_delay400ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=450 | grep COM28 | bash > ../vlcSyncFiles/auto_delay450ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=500 | grep COM28 | bash > ../vlcSyncFiles/auto_delay500ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=550 | grep COM28 | bash > ../vlcSyncFiles/auto_delay550ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=600 | grep COM28 | bash > ../vlcSyncFiles/auto_delay600ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=650 | grep COM28 | bash > ../vlcSyncFiles/auto_delay650ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=700 | grep COM28 | bash > ../vlcSyncFiles/auto_delay700ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=750 | grep COM28 | bash > ../vlcSyncFiles/auto_delay750ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=800 | grep COM28 | bash > ../vlcSyncFiles/auto_delay800ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=850 | grep COM28 | bash > ../vlcSyncFiles/auto_delay850ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=900 | grep COM28 | bash > ../vlcSyncFiles/auto_delay900ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=950 | grep COM28 | bash > ../vlcSyncFiles/auto_delay950ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=1000 | grep COM28 | bash > ../vlcSyncFiles/auto_delay1000ms.csv
	g++ -nostdinc -x c -E main.ino -DGENERATE_CSV -DDELAY_RADIO=1050 | grep COM28 | bash > ../vlcSyncFiles/auto_delay1050ms.csv













