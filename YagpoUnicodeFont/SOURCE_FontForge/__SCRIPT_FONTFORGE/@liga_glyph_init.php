<?php

//базовые функции инициализации данных

function readText($filename){
		$fp = fopen("$filename", 'a+')or die("... no open $filename");
		$fx=fread($fp,filesize($filename));
		fclose($fp);
        return $fx;  
};
function writeText($text,$filename){
		$fp = fopen("$filename", 'w+')or die("... no open $filename");
		if (flock($fp, LOCK_EX)) { // do an exclusive lock
			fwrite($fp,$text); flock($fp, LOCK_UN); // release the lock
  	    } else {echo "Couldn't write temp file $filename"; exit(0); }
  	    fclose($fp);	
};  	    

function uniord($s) {
    return unpack('V', iconv('UTF-8', 'UCS-4LE', $s))[1];
}

// code point to UTF-8 string
function unichr($i) {
    return iconv('UCS-4LE', 'UTF-8', pack('V', $i));
}

function hUni($dec){
	$hex=dechex(uniord($dec));
	while(strlen($hex)<4)$hex="0".$hex;
	return $hex;
}

function uniReplace($in,$out,$t){
	if(strlen($out[0])>1){ //if $out is array of string
		for($i=0;$i<count($out);$i++){
			$uni.="/".hUni($out[$i]);
		}
		$uni.="/";
		//print $in." -> ".$out[0]." ".$uni."\n";
		$t=str_replace($in,$uni,$t);
		$t=str_replace("//","/",$t);
	}else{
		$t=str_replace($in,"/".hUni($out)."/",$t);
		$t=str_replace("//","/",$t);
	}
	
	return $t;

}



//print "@@@3=".$argv[1]."\n";
$path_list="/Volumes/WORK_DHARMA_3TB/SAMSUNG/MainYagpoOCR/Font/__SCRIPT_FONTFORGE/_namelist_uni1.liga";
$path_list_name=str_replace(".txt","",$path_list);
$uni=Array();
$ligaSort=Array(100);
$ligaTable="";

//читаем список имен букв которые необходимо построить и их порядок набора

if(!is_file($path_list)){
	print "run script liga_uni.php first\n";
	exit(0);
}	

	$text=readText($path_list);
	$text=str_replace("\r","\n",$text);
	$codeTable=explode("\n",$text);
	$ligaArray=Array();
	
	for($i=0;$i<count($codeTable);$i++){
		//
		$line=explode("\t",$codeTable[$i]);
		if(count($line)<3){  
			print $i." no line \n";
			continue;
		}
		$name=$line[1];
		$uni=$line[2];
		$l=str_replace("/","\t",$uni);
		$l=str_replace("\t\t","\t",$l);
		$l.=":|:";
		$l=str_replace("\t:|:","",$l);
		$l=str_replace(":|:","",$l);
		$line=explode("\t",$l);

		$ligaUni=Array();
		
		for($n=0;$n<count($line);$n++){
			if($line[$n]!="")$ligaUni[]=$line[$n];
		}
		$ligaArray[$name]=$ligaUni;
		
	}
	
 	print "ligaArray count:".count($ligaArray)."\n";
 	$codeText="";
if($needClean){
 	$codeText="#first clean all glyph we need to build";
 	foreach($ligaArray as $name=>$uniArray){
 		if($name=="")continue;
 		$codeText.="SelectMore(\"$name\");\n";
 	}
	$codeText.="\nClear();\n";
	
}	
 	$codeText.="#start build all glyphs for GB18030 and aditional table";

	$uni=Array();
	$uni["0f68"]="aa";
	$uni["0f00"]="oM";
	$uni["0f01"]="gter_yig";
	$uni["0f02"]="gter.yig.mgo.um.rnam.bcad.ma";
	$uni["0f03"]="gter.yig.mgo.um.gter.tsheg.ma";
	$uni["0f04"]="ini.yig.mgo.mdun.ma";
	$uni["0f05"]="close.yig.mgo.sgab.ma";
	$uni["0f06"]="caret.yig.mgo.phur.shad.ma";
	$uni["0f07"]="yig.mgo.tsheg.shad.ma";
	$uni["0f08"]="sbrul.shad";
	$uni["0f09"]="bskur.yig.mgo";
	$uni["0f0a"]="bka.shog.yig.mgo";
	$uni["0f0b"]="inter.tsheg";
	$uni["0f0d"]="shad";
	$uni["0f0e"]="nyis.shad";
	$uni["0f0f"]="tsheg.shad";
	$uni["0f10"]="nyis.tsheg.shad";
	$uni["0f11"]="rin.chen.spungs.shad";
	$uni["0f12"]="rgya.gram.shad";
	$uni["0f13"]="caret.dzud.rtags.me.long.can";
	$uni["0f14"]="gter.tsheg";
	$uni["0f15"]="chad.rtags";
	$uni["0f16"]="lhag.rtags";
	$uni["0f17"]="sgra.gcan.-char.rtags";
	$uni["0f18"]="khyud.pa";
	$uni["0f19"]="sdong.tshugs_";
	$uni["0f1a"]="rdel.dkar.gcig";
	$uni["0f1b"]="rdel.dkar.gnyis";
	$uni["0f1c"]="rdel.dkar.gsum";
	$uni["0f1d"]="rdel.nag.gcig";
	$uni["0f1e"]="rdel.nag.gnyis";
	$uni["0f1f"]="rdel.dkar.rdel.nag";
	$uni["0f20"]="zero";
	$uni["0f21"]="one";
	$uni["0f22"]="two";
	$uni["0f23"]="three";
	$uni["0f24"]="four";
	$uni["0f25"]="five";
	$uni["0f26"]="six";
	$uni["0f27"]="seven";
	$uni["0f28"]="eight";
	$uni["0f29"]="nine";
	$uni["0f2a"]="half.one";
	$uni["0f2b"]="half.two";
	$uni["0f2c"]="half.three";
	$uni["0f2d"]="half.four";
	$uni["0f2e"]="half.five";
	$uni["0f2f"]="half.six";
	$uni["0f30"]="half.seven";
	$uni["0f31"]="half.eight";
	$uni["0f32"]="half.nine";
	$uni["0f33"]="half.zero";
	$uni["0f34"]="bsdus.rtags";
	$uni["0f35"]="uni0F35";
	$uni["0f36"]="caret.dzud.rtags.bzhi.mig.can";
	$uni["0f37"]="_xa";
	$uni["0f38"]="che.mgo";
	$uni["0f7f"]="rnam.bchad";
	$uni["0f3a"]="gug.rtags.gyon";
	$uni["0f3b"]="gug.rtags.gyas";
	$uni["0f3c"]="ang.khang.gyon";
	$uni["0f3d"]="ang.khang.gyas";
	$uni["0f3e"]="yar.tshes";
	$uni["0f3f"]="mar.tshes";
	$uni["0f40"]="ka";
	$uni["0f41"]="kha";
	$uni["0f42"]="ga";
	$uni["0f43"]="gha";
	$uni["0f44"]="nga";
	$uni["0f45"]="ca";
	$uni["0f46"]="cha";
	$uni["0f47"]="ja";
	$uni["0f49"]="nya";
	$uni["0f4a"]="Ta";
	$uni["0f4b"]="Tha";
	$uni["0f4c"]="Da";
	$uni["0f4e"]="Na";
	$uni["0f4f"]="ta";
	$uni["0f50"]="tha";
	$uni["0f51"]="da";
	$uni["0f53"]="na";
	$uni["0f54"]="pa";
	$uni["0f55"]="pha";
	$uni["0f56"]="ba";
	$uni["0f58"]="ma";
	$uni["0f59"]="tsa";
	$uni["0f5a"]="tsha";
	$uni["0f5b"]="dza";
	$uni["0f5c"]="dzha";
	$uni["0f5d"]="wa";
	$uni["0f5e"]="zha";
	$uni["0f5f"]="za";
	$uni["0f60"]="achung";
	$uni["0f61"]="ya";
	$uni["0f62"]="ra";
	$uni["0f63"]="la";
	$uni["0f64"]="sha";
	$uni["0f65"]="Sha";
	$uni["0f66"]="sa";
	$uni["0f67"]="ha";
	$uni["0f6a"]="ra";
	$uni["0f71"]="_A";
	$uni["0f72"]="_i";
	$uni["0f73"]="_I";
	$uni["0f74"]="_u";
	$uni["0f75"]="_U";
	$uni["0f76"]="vocalic.r";
	$uni["0f77"]="vocalic.rr";
	$uni["0f78"]="vocalic.l";
	$uni["0f79"]="vocalic.ll";
	$uni["0f7a"]="_e";
	$uni["0f7b"]="_ai";
	$uni["0f7c"]="_o";
	$uni["0f7d"]="_au";
	$uni["0f7e"]="_M";
	$uni["0f80"]="__i";
	$uni["0f81"]="__I";
	$uni["0f84"]="halanta";
	$uni["0f85"]="paluta";
	$uni["0f88"]="lce_tsa_can";
	$uni["0f89"]="mchu_can";
	$uni["0f8a"]="gru_can_rgyings";
	$uni["0f8b"]="gru_med_rgyings";
	$uni["0f8d"]="uni0F8D";
	$uni["0f90"]="ka";
	$uni["0f91"]="kha";
	$uni["0f92"]="ga";
	$uni["0f93"]="gha";
	$uni["0f94"]="nga";
	$uni["0f95"]="ca";
	$uni["0f96"]="cha";
	$uni["0f97"]="ja";
	$uni["0f99"]="nya";
	$uni["0f9a"]="Ta";
	$uni["0f9b"]="Tha";
	$uni["0f9c"]="Da";
	$uni["0f9d"]="Dha";
	$uni["0f9e"]="Na";
	$uni["0f9f"]="ta";
	$uni["0fa0"]="tha";
	$uni["0fa1"]="da";
	$uni["0fa2"]="dha";
	$uni["0fa3"]="na";
	$uni["0fa4"]="pa";
	$uni["0fa5"]="pha";
	$uni["0fa6"]="ba";
	$uni["0fa7"]="bha";
	$uni["0fa8"]="ma";
	$uni["0fa9"]="tsa";
	$uni["0faa"]="tsha";
	$uni["0fab"]="dza";
	$uni["0fac"]="dzha";
	$uni["0fad"]="vasur";
	$uni["0fae"]="zha";
	$uni["0faf"]="za";
	$uni["0fb0"]="achung";
	$uni["0fb3"]="la";
	$uni["0fb4"]="sha";
	$uni["0fb5"]="Sha";
	$uni["0fb6"]="sa";
	$uni["0fb7"]="ha";
	$uni["0fb8"]="aa";
	$uni["0fba"]="wa";
	$uni["0fbb"]="ya";
	$uni["0fbc"]="ra";
	$uni["0fbe"]="ku.ru.kha";
	$uni["0fbf"]="ku.ru.kha.bzhi.mig.can";
	$uni["0fc0"]="heavy.beat";
	$uni["0fc3"]="sbub.chal";
	$uni["0fc4"]="dril.bu";
	$uni["0fc5"]="rdo.rje";
	$uni["0fc6"]="padma.gdan";
	$uni["0fc7"]="rdo.rje.rgya.gram";
	$uni["0fc8"]="phur.pa";
	$uni["0fc9"]="nor.bu";
	$uni["0fca"]="nor.bu.nyis.khyil";
	$uni["0fcb"]="nor.bu.gsum.khyil";
	$uni["0fcc"]="nor.bu.bzhi.khyil";
	$uni["0fcf"]="rdel.nag.gsum";
	$uni["0fd0"]="bska.shog.gi.mgo.rgyan";
	$uni["0fd1"]="mnyam.yig.gi.mgo.rgyan";
	$uni["0f70"]="uni0F70";
	$uni["0f6f"]="uni0F6F";
	$uni["0fb1"]="yatag";
	$uni["0fb2"]="ratag";
	$uni["0fb9"]="kSha";
	$uni["0fbd"]="u0x0FBD";
	$uni["0fc1"]="light.beat";
	$uni["0fc2"]="cang.te.u";
	$uni["0fce"]="_oM";
	$uni["0fd4"]="uni0FD4";
	$uni["0fd5"]="uni0FD5";
	$uni["0fd6"]="uni0FD6";
	$uni["0fd7"]="uni0FD7";
	$uni["0fd8"]="uni0FD8";
	$uni["0fd9"]="uni0FD9";
	$uni["0fda"]="uni0FDA";
	$uni["0fdb"]="uni0FDB";
	$uni["0fdc"]="uni0FDC";
	$uni["0fdd"]="uni0FDD";
	$uni["0fdf"]="u0x0FDF";
	$uni["0fe0"]="uni0FE0";
	$uni["0fe1"]="uni0FE1";
	$uni["0fe2"]="uni0FE2";
	$uni["0fe3"]="uni0FE3";
	$uni["0fe4"]="uni0FE4";
	$uni["0fe5"]="uni0FE5";
	$uni["0fe6"]="uni0FE6";
	$uni["0fe7"]="uni0FE7";
	$uni["0fe8"]="uni0FE8";
	$uni["0fe9"]="uni0FE9";
	$uni["0fea"]="uni0FEA";
	$uni["0feb"]="_aauM";
	$uni["0fec"]="uni0FEC";
	$uni["0fed"]="uni0FED";
	$uni["0fee"]="uni0FEE";
	$uni["0fef"]="_M1";
	$uni["0ff0"]="uni0FF0";
	$uni["0ff1"]="uni0FF1";
	$uni["0ff2"]="uni0FF2";
	$uni["0ff3"]="uni0FF3";
	$uni["0ff5"]="_iM1";
	$uni["0ff6"]="uni0FF6";
	$uni["0ff7"]="uni0FF7";
	$uni["0ff8"]="M_i1";
	$uni["0ff9"]="uni0FF9";
	$uni["0ffa"]="uni0FFA";
	$uni["0ffc"]="uni0FFC";
	$uni["0ffd"]="uni0FFD";
	$uni["0ffe"]="uni0FFE";
	$uni["0fff"]="_oMM";
	$uni["0f52"]="dha";
	$uni["0f39"]="tsa.phru_";
	$uni["0f83"]="_MM";
	$uni["0f82"]="_naa_da";
	$uni["0f57"]="bha";
	$uni["0f4d"]="Dha";
	
//читаем из файла _anchorList.txt координаты контрольных установочных точек Anchor Points для всех букв образующих лигатуры
	print "start ";
	$anchorArray=Array();
	$path="/Volumes/WORK_DHARMA_3TB/SAMSUNG/MainYagpoOCR/Font/__SCRIPT_FONTFORGE/_anchorList.txt";
	$text=readText($path);
	$text=str_replace("\r","\n",$text);
	$text=str_replace("   ","\t",$text);
	$text=str_replace(" \n","\n",$text);
	$anchorTable=explode("\n",$text);
	for($i=0;$i<count($anchorTable);$i++){
		if($anchorTable[$i]=="")continue;
		$line=explode("\t",$anchorTable[$i]);
		$name=$line[0];
		if(!isset($anchorArray[$name]))$anchorArray[$name]=Array();
		$anchorArray[$name][]=$line[1];
		$anchorArray[$name][]=$line[2];
		$anchorArray[$name][]=$line[3];
	}
	
	//print $anchorArray["tha"][0]."\n"; exit(0);

function isStopLetter($n){
	$n=str_replace("c0.","",$n);
	$n=str_replace("c1.","",$n);
	$n=str_replace("c2.","",$n);
	$n=str_replace("c3.","",$n);
	if($n[0]=="_"||$n=="ratag"||$n=="ratag_1"||$n=="yatag"||$n=="vasur"||$n=="halanta")return 1;
	return 0;
}	
function isTopLetter($n){
	$n=str_replace("c_.","",$n);
	$n=str_replace("c0.","",$n);
	$n=str_replace("c1.","",$n);
	$n=str_replace("c2.","",$n);
	$n=str_replace("c3.","",$n);
	//print "@@@ $n \n";
	if($n[0]=="_"&&$n!="_A"&&$n!="_u")return 1;
	return 0;
}	

function isJoined($b,$n,$nextNext){
	$b=str_replace("c_.","",$b);
	$b=str_replace("c0.","",$b);
	$b=str_replace("c1.","",$b);
	$b=str_replace("c2.","",$b);
	$b=str_replace("c3.","",$b);
	
	$n=str_replace("c_.","",$n);
	$n=str_replace("c0.","",$n);
	$n=str_replace("c1.","",$n);
	$n=str_replace("c2.","",$n);
	$n=str_replace("c3.","",$n);
	
	global $joinStackLength;
	
	//print "@@@ join  b:$b n:$n nextNext:$nextNext\n";
	
	if($nextNext!=""){
		if($b=="ra"&&$n=="dza"&&$nextNext=="ratag"){
			$joinStackLength=2;
			return "rdzra";
		}	
		if($b=="ra"&&$n=="dza"&&$nextNext=="yatag"){
			$joinStackLength=2;
			return "rdzya";
		}	
	}
	

	if($b=="ka"||$b=="ga"||$b=="ha"||$b=="ta"||$b=="Sha"||$b=="na"||$b=="nya"||$b=="sha"||$b=="Na"||$b=="Tha"||$b=="Da"
		||$b=="Ta"||$b=="da"||$b=="ja"||$b=="dza"||$b=="nga"||$b=="ra"||$b=="kha"||$b=="achung"||$b=="rdza"){
		if($n=="yatag"){
			if($b!="achung")$b=substr($b,0,strlen($b)-1);
			$b.=".ya";
			$joinStackLength=1;
			return $b;
		}	
	}
	if($b=="ka"||$b=="ga"||$b=="kha"||$b=="da"||$b=="Da"||$b=="zha"||$b=="na"||$b=="nya"||$b=="ha"||$b=="ta"||$b=="ja"||$b=="dza"
	||$b=="ra"||$b=="achung"||$b=="nga"||$b=="Ta"||$b=="Na"||$b=="Sha"||$b=="rdza"){
		if($n=="ratag"){
			if($b!="achung")$b=substr($b,0,strlen($b)-1);
			$b.=".ra";
			$joinStackLength=1;
			return $b;
		}	
	}
	if($b=="_i"||$b=="__i"||$b=="_ai"||$b=="_au"||$b=="_e"||$b=="_o"){
		if($n=="_M"||$n=="_MM"||$n=="_naa_Da"){
			$n=str_replace("_","",$n);
			$b.=$n;
			$joinStackLength=1;
			return $b;
		}	
	}
	if($b=="ra"){
		if($n=="tsa"){$joinStackLength=1; return "rtsa";}
		if($n=="dza"){$joinStackLength=1; return "rdza";}
	}
	if($b=="_MM"&&$n=="__i"){$joinStackLength=1; return "_MM_i";}
	if($b=="_M"&&$n=="__i"){$joinStackLength=1; return "_M_i";}

	return "";
}	

function setLetter($name,$glyph){
	global $codeText;
	$codeText.="
		Select(\"$glyph\");
		CopyReference();
		Select(\"$name\");
		Paste();";
}


?>
