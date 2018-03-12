objects:=wifiLocate.o\
		  capture.o\
		  systemfunction.o\
		  common/util.o\
		  common/count_signal.o\
		  common/logwriter.o\
		  dissect/dissecter.o\
		  dissect/ieee80211/ieee80211_dissect.o\
		  dissect/radiotap/radiotap_dissect.o\

all:${objects}
	g++ -o wifiLocate ${objects} -lpcap -lpthread
	
clean:
	rm ${objects} wifiLocate