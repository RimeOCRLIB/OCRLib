///C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#include "php2stl.h"
#include "angel.h"

///основной перекресток управления программой через интерфейс
///состояние интерфейса записанное в inputData в процессе обработки HTTP запроса браузера или из командной строки интерпретируеся,
///вызывая создание соответсвующих объектов и выполнение методов
///inputData также содержит указатели на глобальные обьекты.
///processInput возвращает строку результирующих данных которая передается как ответ программы на запрос браузера или другого интерфейса.

void processRequest(string &destString){
    //int print=1;
    //TIME_START
    inputData.data["HTTPType"]="text/html";
    string ocrData=inputData.data["ocrData"];
    inputData.data["avatar"]="library";
    
    if(inputData.data["ocrData"]=="chat"){
            aiml::cAngel *angel;
            angel=aiml::cAngel::create(inputData.data["avatar"],inputData.data["user_name"],1);
            destString=angel->angel(inputData.data["user_input"],
                                    inputData.data["avatar"],
                                    inputData.data["user_name"],
                                    SINGLE_LINE);
            return;
    }
    if(inputData.data["ocrData"]=="reportAI"){
        aiml::cAngel *angel;
        string str="library";
        angel=aiml::cAngel::create(str,str,1);
        angel->angelReadFile(inputData.fileList[0],str);
        return;
    }
    
    


}