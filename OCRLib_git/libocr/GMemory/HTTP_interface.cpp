#include "GMemory.h"
#include "GVector.h"
#include "php2stl.h"


//HTTP interface to database. Main function of communication between database and www.dharmabook.ru 


void GMemory::buildFileDatabase(string &path){

    string pathLib=inputData.data["dictPath"]+"/LIBRARY_CATALOG_FOLDER.bin";
    string pathCatalog=inputData.data["dictPath"]+"/LIBRARY_CATALOG_FOLDER.xml";
    GVector *vectorLib=GVector::create(pathLib);
    vectorLib->resizeData(100000,140);   //100k records in 140 mb
    uint recordNum=vectorLib->size();
    if(recordNum==0)vectorLib->importXML(pathCatalog);
    cout_<<"vectorLib->size()="<<vectorLib->size()<<" path="<<path; exit(0);
    

}