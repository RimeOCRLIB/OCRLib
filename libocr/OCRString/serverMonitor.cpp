//
//  serverMonitor.mm
//
//  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will redestrubution of Buddha Dharma bring benefit and peace in the world*.
//****************************************************************************
//

#include <iostream>
#include "php2stl.h"
#include <time.h>
#include <unistd.h>

using namespace std;

//монитор состояния сервера
void serverMonitor()
{
	vector<string> result;
	int count = 0;
	//int needReboot=0;
	string cmd = "osascript '/OSBL/reboot/reboot/CRON.script'";
	time_t rawtime;
	string timeStr;
	int timeCount = 1440; // 3*24 hours
    //int timeCount = 1; // 3*24 hours
	//string rebootTime = "15:00";
	ofstream c_out;
	c_out.open("/_Image2OCR/_logout2021.txt", fstream::in | fstream::out | fstream::app);

    time(&rawtime);
	timeStr = ctime(&rawtime);
	cout << timeStr << endl;
	c_out << "@@@ New start " << timeStr << endl;
    int step=0;

	while (1){
		time(&rawtime);
		timeStr = ctime(&rawtime);
		cout << timeStr << endl;
		explode(" ", timeStr,result);
		if (result.size() > 2)
		{
			timeStr = result[result.size() - 2];
			explode(":", timeStr,result);
			timeStr = result[0] + ":" + result[1];
		}
		cout << timeStr << " server VM monitor: count " << count <<" step:"<<step<< endl;
		//c_out<<timeStr<<" server VM monitor: "<<vmStat<<" count "<<count<<" reboot time "<<rebootTime<<endl;
		if (count > timeCount){
			cout << count << " server reboot: " << " timeStr=" << timeStr << endl;
			c_out << count << " server reboot: " << " timeStr=" << timeStr << endl;
			system(cmd.c_str());
		}
        if(step==10){
            run("echo dzogchen | /usr/bin/sudo -S killall imdir");
            step=0;
        }
        step++;
		count++;
		sleep(180);
	}

	//exit(0);
}
