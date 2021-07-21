#include "GMemory.h"
#include "../../OCRLib_main/OCRLib.h"

void GMemory::memoryRequest(string &destString)
{
	destString = "";
	uint rowsNum = 0;

	string str;
	string searchString;
	rowsNum = atoi(inputData.data["rows"].c_str());
	if (rowsNum > 1000 || rowsNum < 10)
		rowsNum = 100;

	string db = inputData.data["db"];
	string ln = inputData.data["ln"];
	string mode = inputData.data["mode"];
	if (inputData.data["c0"] != "")
	{
		inputData.data["field"] = "0";
		mode = "HASH_SEARCH";
		inputData.data["word"] = inputData.data["c0"];
		db = "mainDict";
	}
	//cout<<"Database name="<<db<<" text="<<inputData.data["text"]<<" ln="<<ln<<endl; exit(0);
	string strName;

	if (inputData.data["ocrData"] == "searchText")
	{
		destString = "";
		string text = inputData.data["text"];
		string key;
		key = str_replace("་", "", text);
		key = str_replace("།", "", key);
		if (key.find("་") != -1)
		{
			key = substr(3, (uint)key.size(), key);
		}

		loadTable(db);
		vector<TString> r;
		vector<int> f;
		f.push_back(1);
		select(db, f, key, FULL_TEXT_SEARCH, r, 0);
		cout << r.size() << endl;
		for (int i = 0; i < r.size(); i++)
		{
			//cout<<i<<" ->> "<<r[i].getInt(0)<<" "<<r[i].getInt(1)<<endl;
			TString s;
			table[db].data->getTStr(r[i].getInt(0), s);
			string st = s[1];
			int offset = r[i].getInt(1);
			st = substr(offset, 1024, st);
			if (st.find(text) != -1)
			{
				string rep = "<c>" + text + "</c>";
				st = str_replace(text, rep, st);
				ostringstream out_;
				out_ << "<a href=\"http://www.buddism.ru/ocr/ocr_book.php?db=DHARMABOOK&record=" << r[i].getInt(0) << "\" target=\"_blank\">" << st << "</a>\n";
				destString += out_.str();
			}
			if (i > 100)
				break;
		}
		cout << destString;
	}

	if (mode == "LIST_SEARCH")
	{
		int fieldIndex = atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
		vector<TString> result;
		//string query="Volumes";
		select(db, field, inputData.data["word"], LIST_SEARCH, result, 25);
		for (int i = 0; i < result.size(); i++)
			cout << result[i][0] << ":|:";
		remove(inputData.data["log"].c_str());
		exit(0);
	}
	if (mode == "BOOK_SEARCH")
	{
		int fieldIndex = atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
		vector<TString> result;
		string query = inputData.data["book"];
		//cout<<"book="<<query<<endl;

		select(db, field, query, TEXT_SEARCH, result, 25);
		if (result.size())
		{
			int l = 0;
			while (l < result.size())
			{
				TString st = result[l];
				st.reloadPtr();
				string str = st[0];
				if (str.find(query) != -1)
				{
					cout << str;
					break;
				}
				l++;
			}
		}
		remove(inputData.data["log"].c_str());
		exit(0);
	}
	if (mode == "CORPUS_ID_SEARCH")
	{
		int fieldIndex = 0;
		//atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
		vector<TString> result;
		//cout<<" id="<<inputData.data["corpusID"]<<endl;
		string str = "[[ID:" + inputData.data["corpusID"] + "]]";
		select(db, field, str, HASH_SEARCH, result, 25);
		//cout<<" res="<<result.size()<<endl;
		if (result.size())
		{
			cout << result[0][0];
			TString &st = result[0];
			string res;
			for (int n = 0; n < st.size(); n++)
			{
				string s = st[n];
				if (s.size() > 9)
					res += s + "\n";
			}
			cout << res;
		}
		remove(inputData.data["log"].c_str());
		exit(0);
	}
	if (mode == "CORPUS_TEXT_SEARCH")
	{
		int fieldIndex = atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
		vector<TString> result;
		//string query="Volumes";
		select(db, field, inputData.data["corpusID"], TEXT_SEARCH, result, 25);
		cout << "<table width=\"90%\" align=\"center\" border=\"1\">";
		for (int i = 0; i < result.size(); i++)
		{
			cout << "<tr><td width=\"200px\">";
			TString st = result[i];
			for (int n = 0; n < st.size(); n++)
			{
				cout << st[n] << "</td><td>";
			}
			cout << "</td></tr>";
		}
		cout << "</table>";
		remove(inputData.data["log"].c_str());
		exit(0);
	}
	if (mode == "HASH_SEARCH")
	{
		int fieldIndex = atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
		vector<TString> result;
		//string query="Volumes";
		select(db, field, inputData.data["word"], HASH_SEARCH, result, 25);
		cout << "<table width=\"90%\" align=\"center\" border=\"1\">";
		for (int i = 0; i < result.size(); i++)
		{
			cout << "<tr><td width=\"200px\">";
			TString st = result[i];
			for (int n = 0; n < st.size(); n++)
			{
				cout << st[n] << "</td><td>";
			}
			cout << "</td></tr>";
		}
		cout << "</table>";
		remove(inputData.data["log"].c_str());
		exit(0);
	}

	if (mode == "FULL_TEXT_SEARCH")
	{
		int fieldIndex = atoi(inputData.data["field"].c_str());
		vector<int> field;
		field.push_back(fieldIndex);
        //int limit = atoi(inputData.data["limit"].c_str());
		vector<TString> result;
		//string query="Volumes";
		pageRec page;
		page.index = 0;
		page.field = fieldIndex;
		page.db = db.c_str();
		page.text = inputData.data["word"].c_str();
		inputData.data["ocrData"] = "batchOCR";
		//searchDBTable(&page, FULL_TEXT_SEARCH);

		remove(inputData.data["log"].c_str());
		exit(0);
	}

	if (inputData.data["act"] == "process")
	{ //запрос на обработку базы
		cout_ << "start process";
		ulong recordNum = vectorDict->size();

		uint index = 0;
		while (index < recordNum)
		{
			string strDict;
			vectorDict->getStr(index, strDict);
			if (!strDict.size())
			{
				continue;
			}
			vector<string> vDict;
			explode(":|:", strDict,vDict);
			if (vDict.size() < 5)
				continue;

			for (int cellNum = 0; cellNum < 5; cellNum++)
			{
			}
			index++;
		}
		cout_ << " end process";
		inputData.data["HTTPType"] = "text/html";
	}
	if (inputData.data["record"] == "findDuplicate")
	{
		if (db == "letter")
		{
			vector<ulong> searchResult;

			for (uint i = 0; i < aliKali->size(); i++)
			{
				GLetter *letter = aliKali->getLetter(i);
				uint index = 0;
				int flag = 0;

				while (letter->key[index].correlation > 98 && letter->name != "*")
				{
					GLetter *letter_ = aliKali->getLetter(letter->key[index].letterIndex);
					if (letter->name == letter_->name)
					{
						//searchResult->push_back(letter->key[index].letterIndex);
						flag = 1;
						break;
					}
					letter_->destroy();
					index++;
				}
				if (flag)
					searchResult.push_back(i);

				//if(letter->keyCount()==0)searchResult->push_back(i);
				letter->destroy();
			}
			int mode = HTML;
			string result;

			//
			//for(itr = matchStat.begin(); itr != matchStat.end(); ++itr){
			//    if(matchStat[(*itr).first]>3)
			//       matchSort.insert(pair<int,string>((*itr).second, (*itr).first));
			//}

			multimap<string, int> list;
			for (uint i = 0; i < searchResult.size(); i++)
			{
				GLetter *letter = aliKali->getLetter(searchResult[i]);
				list.insert(pair<string, int>(letter->name, searchResult[i]));
				letter->destroy();
			}
			searchResult.resize(0);
			map<string, int>::const_iterator itr;
			for (itr = list.begin(); itr != list.end(); ++itr)
			{
				searchResult.push_back((*itr).second);
			}

			aliKali->drawOCRBasePict(searchResult, mode);
			aliKali->drawHTML(searchResult, result, mode);
			destString = result;
			if (result == "")
				destString = "no records found";
			inputData.data["HTTPType"] = "text/html";
		}
	}
	if (inputData.data["record"] == "findIncomplete")
	{
		if (db == "letter")
		{
			vector<ulong> searchResult;
			GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
			cout << "size map=" << logicProcessor->mainLetterTableUni.size();

			/*if(logicProcessor->mainLetterTableUni.find(letter->name)==logicProcessor->mainLetterTableUni.end()){
             cout<<"no record "<<letter->name<<endl;
             letter->name+="@";
             }
             */
			string re = "[\\p{Tibetan}]+";

			for (uint i = 0; i < aliKali->size(); i++)
			{
				GLetter *letter = aliKali->getLetter(i);
				if (letter->name == "*")
				{
					letter->destroy();
					continue;
				}
				//подсчитываем площадь буквы покрытой масками
				//letter->drawLetterPict(1);
				//aliKali->saveLetter(letter);
				//if(letter->onOffRate<50&&letter->name!="*")searchResult.push_back(i);
				/*
                 int flag=0;
                 for(uint n=0;n<letter->mask32Count();n++){
                 //cout<<"y0="<<letter->mask32[n].yMask<<" y01="<<letter->letterH/2<<endl;
                 if(letter->mask32[n].mH>letter->letterH){
                 letter->mask32[n].mH=letter->letterH+9;
                 flag=1;
                 }
                 if(letter->mask32[n].yMask<-letter->letterH/2-5){
                 letter->mask32[n].yMask=-letter->letterH/2-5;
                 flag=1;                        
                 }
                 if(letter->mask32[n].yMask+letter->mask32[n].mH>letter->letterH/2+5){
                 letter->mask32[n].yMask=letter->letterH/2+5-letter->mask32[n].mH;
                 flag=1;
                 }
                 }
                 if(flag){
                 searchResult.push_back(i);
                 letter->reloadMask();
                 aliKali->saveLetter(letter);
                 }
                 */
				letter->reloadMask();
				aliKali->saveLetter(letter);

				//if(letter->name.find("@")!=-1)searchResult.push_back(i);
				/*if(logicProcessor->mainLetterTableUni.find(letter->name)==logicProcessor->mainLetterTableUni.end()){
                 if(rexExpFind((char*)letter->name.c_str(),re)){
                 //cout<<"no record "<<letter->name<<endl;
                 //letter->name="*";
                 //aliKali->saveLetter(letter);
                 searchResult.push_back(i);
                 }
                 }
                 */

				letter->destroy();
			}
			int mode = HTML;
			string result;

			multimap<int, int> list;
			for (uint i = 0; i < searchResult.size(); i++)
			{
				GLetter *letter = aliKali->getLetter(searchResult[i]);
				list.insert(pair<int, int>(letter->onOffRate, searchResult[i]));
				letter->destroy();
			}
			searchResult.resize(0);
			map<int, int>::const_iterator itr;
			for (itr = list.begin(); itr != list.end(); ++itr)
			{
				searchResult.push_back((*itr).second);
			}

			/*
             multimap<string,int>list;
             for(uint i=0;i<searchResult.size();i++){
             GLetter *letter=aliKali->getLetter(searchResult[i]);
             list.insert(pair<string,int>(letter->name, searchResult[i]));
             letter->destroy();
             }
             searchResult.resize(0);
             map<string, int>::const_iterator itr;
             for(itr = list.begin(); itr != list.end(); ++itr){
             searchResult.push_back((*itr).second);
             }
             */

			aliKali->drawOCRBasePict(searchResult, mode);
			aliKali->drawHTML(searchResult, result, mode);
			destString = result;
			if (result == "")
				destString = "no records found";
			inputData.data["HTTPType"] = "text/html";
		}
	}
	if (inputData.data["record"] == "import")
	{ //запрос на импорт данных в базу
		cout_ << "memory record import request" << endl;

		if (inputData.data["db"] == "corpus")
		{
			string path = inputData.data["import"];
			path = "/_CORPUS/kannada_corpus_2011_08_16/Kannada_line_Uni.txt";
			string str;
			int mode = 0;

			if (!is_file(path))
			{
				destString = "no valid file path for import";
				return;
			}

			if (fileExt(path) == "txt")
				mode = READ_FROM_TXT;
			if (fileExt(path) == "tab")
				mode = READ_FROM_TAB;
			if (fileExt(path) == "xml")
				mode = READ_FROM_XML;

			vectorCorpus->import(path, mode);
			cout_ << "vectorDict->size()=" << vectorDict->size() << "vectorCorpus->size()=" << vectorCorpus->size() << endl;
			return;
		}
		if (inputData.data["db"] == "dict")
		{
			string path = inputData.data["import"];
			string str;
			int mode = 0;

			if (!is_file(path))
			{
				destString = "no valid file path for import";
				return;
			}

			if (fileExt(path) == "txt")
				mode = READ_FROM_TXT;
			if (fileExt(path) == "tab")
				mode = READ_FROM_TAB;
			if (fileExt(path) == "xml")
				mode = READ_FROM_XML;

			vectorDict->import(path, mode);
			cout_ << "vectorDict->size()=" << vectorDict->size() << endl;
			return;
		}
		if (inputData.data["db"] == "letter")
		{
			string path = inputData.data["OCRTables"] + "GFont.xml";
			cout_ << "@path=" << path << endl;
			if (!is_file(path))
			{
				destString = "no valid file path for import";
				return;
			}
			aliKali->readGFontXML();
			//string cmd="mv "+path +inputData.data["OCRTables"]+"GFont1.xml";
			//run (cmd);

			return;
		}
	}
	if (inputData.data["record"] == "export")
	{ //запрос на экспорт данных из базы
		destString = "database record export request";
		cout_ << "export " << inputData.data["db"] << endl;
		if (inputData.data["db"] == "letter")
		{
			aliKali->writeGFontXML();
			return;
		}
		string path = inputData.data["dictPath"] + "/TibetanGrammar1.tab";

		string str;
		ofstream c_out;
		c_out.open(path.c_str());
		if (!c_out)
		{
			destString = "no valid file path for export";
			cout_ << destString << " " << path << endl;
			return;
		}

		if (db == "dict")
		{
			string strRec;
			for (uint i = 0; i < vectorDict->size(); i++)
			{
				TString st;
				vectorDict->getTStr(i, st);
				strRec = st[0] + "\t" + st[1] + "\t" + st[2] + "\t" + st[3] + "\t" + st[4] + "\t";
				c_out << str_replace("\n", "•", strRec) << "\n";
			}
		}
		c_out.close();
	}
	if (inputData.data["record"] == "export_edited")
	{ //запрос на экспорт данных из базы
		destString = "database record edited export request";
		string path = inputData.data["dictPath"] + "/TibetanGrammar_edited.tab";

		string str;
		ofstream c_out;
		c_out.open(path.c_str());
		if (!c_out)
		{
			destString = "no valid file path for export";
			cout_ << destString << " " << path << endl;
			return;
		}

		if (inputData.data["db"] == "letter")
		{
			destString = ((GFontEditor *)(inputData.fontEditor))->exportGFontsDB(path);
			return;
		}

		if (db == "dict")
		{
			string strRec;
			for (uint i = 0; i < vectorDict->size(); i++)
			{
				TString st;
				vectorDict->getTStr(i, st);
				if (st[3].find("@") == -1)
					continue;
				strRec = st[0] + "\t" + st[1] + "\t" + st[2] + "\t" + st[3] + "\t" + st[4] + "\t";
				c_out << str_replace("\n", "•", strRec) << "\n";
			}
		}
		c_out.close();
	}
	if (inputData.data["record"] == "save")
	{ //запрос на сохранение записи
		destString = "<script>location.href=\"ocr.php?db=" + inputData.data["db"] + "&record=find&c1=" + inputData.data["c0"] + "\"</script>";
		string dataStr;
		if (inputData.data["c5"] == "")
		{
			destString = "save error. no valid data";
			cout_ << destString;
			return;
		}
		else
		{
			dataStr = "\tc0\t" + inputData.data["c0"] + "\tc1\t" + inputData.data["c1"] + "\tc2\t" + inputData.data["c2"] + "\tc3\t" + inputData.data["c3"] + "\tc4\t" + inputData.data["c4"] + "\tc5\t" + inputData.data["c5"];
			cout_ << "IndStr=" << inputData.data["c5"] << "/ destStr=" << dataStr << "/\n";
			TString st;

			st.push_back(inputData.data["c0"]);
			st.push_back(inputData.data["c1"]);
			st.push_back(inputData.data["c2"]);
			st.push_back(inputData.data["c3"]);
			st.push_back(inputData.data["c4"]);
			st.push_back(inputData.data["c5"]);
			uint index = atoi(inputData.data["c5"].c_str());
			if (inputData.data["db"] == "corpus")
			{
				vectorCorpus->putTStr(index, st);
				return;
			}
			if (inputData.data["db"] == "dict")
			{
				if (index >= vectorDict->size())
				{
					vectorDict->push_back(st);
				}
				else
				{
					vectorDict->putTStr(index, st);
				}
				return;
			}
		}
	}
	if (inputData.data["record"] == "copy")
	{ //запрос на сохранение записи
		destString = "<script>location.href=\"ocr.php?" + inputData.data["db"] + "&record=find&c1=" + inputData.data["c0"] + "\"</script>";
		string dataStr;
		if (inputData.data["c5"] == "")
		{
			destString = "save error. no valid data";
			cout_ << destString;
			return;
		}
		else
		{
			dataStr = "\tc0\t" + inputData.data["c0"] + "\tc1\t" + inputData.data["c1"] + "\tc2\t" + inputData.data["c2"] + "\tc3\t" + inputData.data["c3"] + "\tc4\t" + inputData.data["c4"] + "\tc5\t" + inputData.data["c5"];
			cout_ << "IndStr=" << inputData.data["c5"] << "/ destStr=" << dataStr << "/\n";
			TString st;

			st.push_back(inputData.data["c0"]);
			st.push_back(inputData.data["c1"]);
			st.push_back(inputData.data["c2"]);
			st.push_back(inputData.data["c3"]);
			st.push_back(inputData.data["c4"]);
			vectorDict->push_back(st);
			return;
		}
	}
}
