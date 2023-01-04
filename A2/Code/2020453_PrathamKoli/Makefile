TS_TC: 
	rm TS.txt
	gcc Thread_Server.c -o TS -lpthread 
	gcc Thread_Client.c -o TC -lpthread

output2: 
	gcc Thread_Server.c -o TS -lpthread 
	gcc Thread_Client.c -o TC -lpthread
fork: 
	rm FS.txt
	gcc Fork_Server.c -o FS
	gcc Thread_Client.c -o TC -lpthread
SS_TC: 
	rm SS.txt
	gcc Sequential_Server.c -o SS
	gcc Thread_Client.c -o TC -lpthread
SS_SC:
	rm SS.txt
	gcc Sequential_Server.c -o SS
	gcc Sequential_Client.c -o SC
EP_TC:
	rm EP.txt
	gcc Epoll.c -o EP
	gcc Epoll_ThreadClient.c -o EPTC
	
