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
void serverMonitor(){
    string vmStat;
    int vmStatInt, vmStatOldInt=0;
    vector<string>result;
    int count=0;
    //int needReboot=0;
    string cmd="osascript '/MainYagpoOCR/reboot/reboot/CRON.script'";
    time_t rawtime;
    string timeStr;
    int timeCount=1600; // 1440  24 hours
    string rebootTime="20:05";
    ofstream c_out; c_out.open("/_Image2OCR/log/_logout2016.txt", fstream::in | fstream::out | fstream::app);
    
    vmStat=run("vm_stat");
    vmStat=substr((int)vmStat.find("Swapins:"),(int)vmStat.length(),vmStat);
    result=explode("\n", vmStat);
    vmStat=result[0];
    vmStat=substr((int)vmStat.find(" "),(int)vmStat.length(),vmStat);
    vmStat=str_replace(".", "", vmStat);
    vmStat=str_replace(" ", "", vmStat);
    vmStatOldInt=atoi(vmStat.c_str());
    time ( &rawtime );
    timeStr=ctime (&rawtime);
    cout<<timeStr<<endl;
    c_out<<"@@@ New start "<<timeStr<<endl;

    while(1){
        vmStat=run("vm_stat");
        vmStat=substr((int)vmStat.find("Swapins:"),(int)vmStat.length(),vmStat);
        result=explode("\n", vmStat);
        vmStat=result[0];
        vmStat=substr((int)vmStat.find(" "),(int)vmStat.length(),vmStat);
        vmStat=str_replace(".", "", vmStat);
        vmStat=str_replace(" ", "", vmStat);
        vmStatInt=atoi(vmStat.c_str());
        
        time ( &rawtime );
        timeStr=ctime (&rawtime);
        cout<<timeStr<<endl;
        result=explode(" ", timeStr);
        if(result.size()>2){
            timeStr=result[result.size()-2];
            result=explode(":", timeStr);
            timeStr=result[0]+":"+result[1];
            
        }
        cout<<timeStr<<" server VM monitor: "<<vmStat<<" count "<<count<<" reboot time "<<rebootTime<<endl;
        //c_out<<timeStr<<" server VM monitor: "<<vmStat<<" count "<<count<<" reboot time "<<rebootTime<<endl;
        if(count>timeCount||timeStr==rebootTime){
                cout<<count<<" server reboot: "<<" timeStr="<<timeStr<<" rebootTime="<<rebootTime<<" vmStat="<<vmStat<<endl;
                c_out<<count<<" server reboot: "<<" timeStr="<<timeStr<<" rebootTime="<<rebootTime<<" vmStat="<<vmStat<<endl;
                system(cmd.c_str());
                sleep(60);
                system("/MainYagpoOCR/reboot/reboot/reboot");
        }
        
        if(vmStatInt>vmStatOldInt+50){
           cout<<count<<" server monitor report swap memory using: "<<vmStatInt<<endl;
           c_out<<count<<" server monitor report swap memory using: "<<vmStatInt<<endl;
           vmStat=run("killall OCRLib");
           cout<<"output vmStatInt="<<vmStatInt<<" vmStatOldInt="<<vmStatOldInt<<endl;
           c_out<<"output vmStatInt="<<vmStatInt<<" vmStatOldInt="<<vmStatOldInt<<endl;
           vmStatOldInt=vmStatInt;
           if(vmStatOldInt!=0&&(vmStatInt>vmStatOldInt+100||vmStatInt>25000)){
               cout<<count<<" server reboot by memory leak: "<<vmStatInt<<endl;
               c_out<<count<<" server reboot by memory leak: "<<vmStatInt<<endl;
               system(cmd.c_str());
           }
        }
        count++;
        sleep(57);
    }

    exit(0);
    
}