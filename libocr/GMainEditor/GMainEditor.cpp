//C-
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

# include "../config.h"	

#include "GMainEditor.h"
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

#ifdef COCOA
#import <Cocoa/Cocoa.h>
#endif



namespace ocr{
	
	
	void GMainEditor::destroy(void){
        delete this;
	}
	
	GMainEditor::GMainEditor(){
	}
	
	
	
	// ----- initialization
	
	void
	GMainEditor::init(){
		setHex();
	}

	void
	GMainEditor::setHex(){
		 //print hex code
		//cout<<"init HEX";
		/*unsigned short *s;
		 string str;
		 for(int i=0;i<256;i++){
		 str=charToHex(i);
		 s=(unsigned short*)&str[0];
		 cout<<"hexCode["<<*s<<"]="<<i<<"; ";
		 }
		*/ 
	
		hexCode[12336]=0; hexCode[12592]=1; hexCode[12848]=2; hexCode[13104]=3; hexCode[13360]=4; hexCode[13616]=5; 
		hexCode[13872]=6; hexCode[14128]=7; hexCode[14384]=8; hexCode[14640]=9; hexCode[16688]=10; hexCode[16944]=11; 
		hexCode[17200]=12; hexCode[17456]=13; hexCode[17712]=14; hexCode[17968]=15; hexCode[12337]=16; hexCode[12593]=17;
		hexCode[12849]=18; hexCode[13105]=19; hexCode[13361]=20; hexCode[13617]=21; hexCode[13873]=22; hexCode[14129]=23; 
		hexCode[14385]=24; hexCode[14641]=25; hexCode[16689]=26; hexCode[16945]=27; hexCode[17201]=28; hexCode[17457]=29;
		hexCode[17713]=30; hexCode[17969]=31; hexCode[12338]=32; hexCode[12594]=33; hexCode[12850]=34; hexCode[13106]=35;
		hexCode[13362]=36; hexCode[13618]=37; hexCode[13874]=38; hexCode[14130]=39; hexCode[14386]=40; hexCode[14642]=41; 
		hexCode[16690]=42; hexCode[16946]=43; hexCode[17202]=44; hexCode[17458]=45; hexCode[17714]=46; hexCode[17970]=47; 
		hexCode[12339]=48; hexCode[12595]=49; hexCode[12851]=50; hexCode[13107]=51; hexCode[13363]=52; hexCode[13619]=53; 
		hexCode[13875]=54; hexCode[14131]=55; hexCode[14387]=56; hexCode[14643]=57; hexCode[16691]=58; hexCode[16947]=59; 
		hexCode[17203]=60; hexCode[17459]=61; hexCode[17715]=62; hexCode[17971]=63; hexCode[12340]=64; hexCode[12596]=65; 
		hexCode[12852]=66; hexCode[13108]=67; hexCode[13364]=68; hexCode[13620]=69; hexCode[13876]=70; hexCode[14132]=71; 
		hexCode[14388]=72; hexCode[14644]=73; hexCode[16692]=74; hexCode[16948]=75; hexCode[17204]=76; hexCode[17460]=77; 
		hexCode[17716]=78; hexCode[17972]=79; hexCode[12341]=80; hexCode[12597]=81; hexCode[12853]=82; hexCode[13109]=83; 
		hexCode[13365]=84; hexCode[13621]=85; hexCode[13877]=86; hexCode[14133]=87; hexCode[14389]=88; hexCode[14645]=89; 
		hexCode[16693]=90; hexCode[16949]=91; hexCode[17205]=92; hexCode[17461]=93; hexCode[17717]=94; hexCode[17973]=95; 
		hexCode[12342]=96; hexCode[12598]=97; hexCode[12854]=98; hexCode[13110]=99; hexCode[13366]=100; hexCode[13622]=101; 
		hexCode[13878]=102; hexCode[14134]=103; hexCode[14390]=104; hexCode[14646]=105; hexCode[16694]=106; hexCode[16950]=107;
		hexCode[17206]=108; hexCode[17462]=109; hexCode[17718]=110; hexCode[17974]=111; hexCode[12343]=112; hexCode[12599]=113;
		hexCode[12855]=114; hexCode[13111]=115; hexCode[13367]=116; hexCode[13623]=117; hexCode[13879]=118; hexCode[14135]=119;
		hexCode[14391]=120; hexCode[14647]=121; hexCode[16695]=122; hexCode[16951]=123; hexCode[17207]=124; hexCode[17463]=125;
		hexCode[17719]=126; hexCode[17975]=127; hexCode[12344]=128; hexCode[12600]=129; hexCode[12856]=130; hexCode[13112]=131;
		hexCode[13368]=132; hexCode[13624]=133; hexCode[13880]=134; hexCode[14136]=135; hexCode[14392]=136; hexCode[14648]=137;
		hexCode[16696]=138; hexCode[16952]=139; hexCode[17208]=140; hexCode[17464]=141; hexCode[17720]=142; hexCode[17976]=143;
		hexCode[12345]=144; hexCode[12601]=145; hexCode[12857]=146; hexCode[13113]=147; hexCode[13369]=148; hexCode[13625]=149;
		hexCode[13881]=150; hexCode[14137]=151; hexCode[14393]=152; hexCode[14649]=153; hexCode[16697]=154; hexCode[16953]=155;
		hexCode[17209]=156; hexCode[17465]=157; hexCode[17721]=158; hexCode[17977]=159; hexCode[12353]=160; hexCode[12609]=161;
		hexCode[12865]=162; hexCode[13121]=163; hexCode[13377]=164; hexCode[13633]=165; hexCode[13889]=166; hexCode[14145]=167;
		hexCode[14401]=168; hexCode[14657]=169; hexCode[16705]=170; hexCode[16961]=171; hexCode[17217]=172; hexCode[17473]=173;
		hexCode[17729]=174; hexCode[17985]=175; hexCode[12354]=176; hexCode[12610]=177; hexCode[12866]=178; hexCode[13122]=179;
		hexCode[13378]=180; hexCode[13634]=181; hexCode[13890]=182; hexCode[14146]=183; hexCode[14402]=184; hexCode[14658]=185;
		hexCode[16706]=186; hexCode[16962]=187; hexCode[17218]=188; hexCode[17474]=189; hexCode[17730]=190; hexCode[17986]=191;
		hexCode[12355]=192; hexCode[12611]=193; hexCode[12867]=194; hexCode[13123]=195; hexCode[13379]=196; hexCode[13635]=197;
		hexCode[13891]=198; hexCode[14147]=199; hexCode[14403]=200; hexCode[14659]=201; hexCode[16707]=202; hexCode[16963]=203;
		hexCode[17219]=204; hexCode[17475]=205; hexCode[17731]=206; hexCode[17987]=207; hexCode[12356]=208; hexCode[12612]=209;
		hexCode[12868]=210; hexCode[13124]=211; hexCode[13380]=212; hexCode[13636]=213; hexCode[13892]=214; hexCode[14148]=215;
		hexCode[14404]=216; hexCode[14660]=217; hexCode[16708]=218; hexCode[16964]=219; hexCode[17220]=220; hexCode[17476]=221;
		hexCode[17732]=222; hexCode[17988]=223; hexCode[12357]=224; hexCode[12613]=225; hexCode[12869]=226; hexCode[13125]=227;
		hexCode[13381]=228; hexCode[13637]=229; hexCode[13893]=230; hexCode[14149]=231; hexCode[14405]=232; hexCode[14661]=233;
		hexCode[16709]=234; hexCode[16965]=235; hexCode[17221]=236; hexCode[17477]=237; hexCode[17733]=238; hexCode[17989]=239;
		hexCode[12358]=240; hexCode[12614]=241; hexCode[12870]=242; hexCode[13126]=243; hexCode[13382]=244; hexCode[13638]=245;
		hexCode[13894]=246; hexCode[14150]=247; hexCode[14406]=248; hexCode[14662]=249; hexCode[16710]=250; hexCode[16966]=251;
		hexCode[17222]=252; hexCode[17478]=253; hexCode[17734]=254; hexCode[17990]=255; 
	
	}
	
	
	
	
};


